#include "udp_receiver.hpp"

#include "core/log/log.hpp"

#include "utils/objects_connector.hpp"

// #include "ellipse_generated.h"
// #include "line_generated.h"
// #include "rect_generated.h"
// #include "triangle_generated.h"
#include "shapes_generated.h"

#include <QNetworkDatagram>
#include <QUdpSocket>

#include <random>

namespace gamma::proc {

struct UDPReceiver::Impl
{
    std::unique_ptr<QUdpSocket> udp_socket;
    std::queue<std::shared_ptr<types::IShape>> shape_queue;

    Impl(const std::string& ip, int port)
    {
        udp_socket = std::make_unique<QUdpSocket>(); // no parent, single socket, deleting by unique_ptr
        udp_socket->bind(QHostAddress(QString::fromStdString(ip)), port);
        GAMMA_LOG(L_INFO, "Create and bind UDP Socket: host = {}, port = {}", ip, port);
    }

    std::shared_ptr<types::IShape> parse_data(QNetworkDatagram&& datagram)
    {
        QByteArray data = datagram.data();
        
        //flatbuffers::Verifier verifier(reinterpret_cast<unsigned char*>(data.data()), data.size());

        static std::random_device rd;
        static std::mt19937 rng{rd()}; 
        std::uniform_int_distribution<int> type_rnd(1, 4);
        std::uniform_int_distribution<int> radius_rnd(1, 10);
        std::uniform_int_distribution<int> coord_rnd(-20, 20);

        auto shape_type = static_cast<types::IShape::ShapeType>(type_rnd(rng));
        switch(shape_type)
        {
            case types::IShape::ShapeType::Ellipse:
                {
                    types::Ellipse shape;
                    shape.x = coord_rnd(rng);
                    shape.y = coord_rnd(rng);
                    shape.r1 = radius_rnd(rng);
                    shape.r2 = radius_rnd(rng);
                    return std::make_shared<types::Ellipse>(shape);
                }
                break;
            case types::IShape::ShapeType::Line:
                {
                    types::Line shape;
                    shape.x1 = coord_rnd(rng);
                    shape.y1 = coord_rnd(rng);
                    shape.x2 = coord_rnd(rng);
                    shape.y2 = coord_rnd(rng);
                    return std::make_shared<types::Line>(shape);
                }
                break;
            case types::IShape::ShapeType::Rect:
                {
                    types::Rect shape;
                    shape.x = coord_rnd(rng);
                    shape.y = coord_rnd(rng);
                    shape.width = radius_rnd(rng);
                    shape.height = radius_rnd(rng);
                    return std::make_shared<types::Rect>(shape);
                }
                break;
            case types::IShape::ShapeType::Triangle:
                {
                    types::Triangle shape;
                    shape.x1 = coord_rnd(rng);
                    shape.y1 = coord_rnd(rng);
                    shape.x2 = coord_rnd(rng);
                    shape.y2 = coord_rnd(rng);
                    shape.x3 = coord_rnd(rng);
                    shape.y3 = coord_rnd(rng);
                    return std::make_shared<types::Triangle>(shape);
                }
                break;
            default:
                return nullptr;
        }

        return nullptr;
    }
};

UDPReceiver::UDPReceiver(const std::string& ip, int port)
    : m_impl(std::make_unique<Impl>(ip, port))
{
    QObject::connect(m_impl->udp_socket.get(), &QUdpSocket::readyRead, this, &UDPReceiver::on_read_udp);

    utils::ObjectsConnector::register_emitter(utils::ObjectsConnectorID::SHAPE_PROCESSED, this, SIGNAL(shape_processed(std::shared_ptr<gamma::types::IShape>)));
}

UDPReceiver::~UDPReceiver()
{
}

void UDPReceiver::worker_thread()
{
    while(!m_need_stop)
    {
        std::shared_ptr<types::IShape> shape;
        {
            std::unique_lock lock(m_guard);
            m_cond.wait(lock, [this](){ return m_need_stop || !m_impl->shape_queue.empty(); });
            
            if(m_need_stop)
                break;
            
            shape = m_impl->shape_queue.front();
            m_impl->shape_queue.pop();
        }
        GAMMA_LOG(L_DEBUG, "UDP processed shape {}", shape);
        shape_processed(shape);
    }

    if(m_need_stop)
    {
        GAMMA_LOG(L_INFO, "UDP Receiver has been forcely stopped");
    }
}

void UDPReceiver::on_read_udp()
{
    GAMMA_LOG(L_DEBUG, "Read UDP Data");
    while (m_impl->udp_socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = m_impl->udp_socket->receiveDatagram();
        auto shape = m_impl->parse_data(std::move(datagram));

        {
            std::scoped_lock lock(m_guard);
            m_impl->shape_queue.push(std::move(shape));
        }
        m_cond.notify_one();
    }
}

}