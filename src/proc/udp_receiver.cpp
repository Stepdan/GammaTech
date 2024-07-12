#include "udp_receiver.hpp"

#include "core/log/log.hpp"

#include "utils/objects_connector.hpp"

#include "shapes_generated.h"
#include "ellipse_generated.h"
#include "line_generated.h"
#include "rect_generated.h"
#include "triangle_generated.h"

#include <QColor>
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
        flatbuffers::Verifier verifier(reinterpret_cast<unsigned char*>(data.data()), data.size());
        if(!VerifyFigureBuffer(verifier))
        {
            GAMMA_LOG(L_ERROR, "Invalid flatbuffers main data verifing");
            return nullptr;
        }

        const auto* figure = GetFigure(data.data());
        
        auto payload_str = figure->payload()->str();
        flatbuffers::Verifier payload_verifier(reinterpret_cast<unsigned char*>(payload_str.data()), data.size());

        const auto figure_type = figure->type();
        switch(figure_type)
        {
            case FigureType::FigureType_Rect:
            {
                if(!VerifyRectDataBuffer(payload_verifier))
                {
                    GAMMA_LOG(L_ERROR, "Invalid Rect verifing");
                    return nullptr;
                }

                const auto rect = GetRectData(payload_str.data());

                types::Rect shape;
                shape.x = rect->x();
                shape.y = rect->y();
                shape.width = rect->width();
                shape.height = rect->height();
                shape.hex_color_str = rect->color_hex()->str();
                GAMMA_LOG(L_INFO, "Get Rect from UDP: {}", shape);
                return std::make_shared<types::Rect>(shape);
            }

            case FigureType::FigureType_Line:
            {
                if(!VerifyLineDataBuffer(payload_verifier))
                {
                    GAMMA_LOG(L_ERROR, "Invalid Line verifing");
                    return nullptr;
                }
                const auto line = GetLineData(payload_str.data());

                types::Line shape;
                shape.x1 = line->x1();
                shape.y1 = line->y1();
                shape.x2 = line->x2();
                shape.y2 = line->y2();
                shape.hex_color_str = line->color_hex()->str();
                GAMMA_LOG(L_INFO, "Get Line from UDP: {}", shape);
                return std::make_shared<types::Line>(shape);
            }

            case FigureType::FigureType_Ellipse:
            {
                if(!VerifyEllipseDataBuffer(payload_verifier))
                {
                    GAMMA_LOG(L_ERROR, "Invalid Ellipse verifing");
                    return nullptr;
                }
                const auto ellipse = GetEllipseData(payload_str.data());

                types::Ellipse shape;
                shape.x = ellipse->x();
                shape.y = ellipse->y();
                shape.r1 = ellipse->r1();
                shape.r2 = ellipse->r2();
                shape.hex_color_str = ellipse->color_hex()->str();
                GAMMA_LOG(L_INFO, "Get Ellipse from UDP: {}", shape);
                return std::make_shared<types::Ellipse>(shape);
            }

            case FigureType::FigureType_Triangle:
            {
                if(!VerifyTriangleDataBuffer(payload_verifier))
                {
                    GAMMA_LOG(L_ERROR, "Invalid Triangle verifing");
                    return nullptr;
                }
                const auto triangle = GetTriangleData(payload_str.data());

                types::Triangle shape;
                shape.x1 = triangle->x1();
                shape.y1 = triangle->y1();
                shape.x2 = triangle->x2();
                shape.y2 = triangle->y2();
                shape.x3 = triangle->x3();
                shape.y3 = triangle->y3();
                shape.hex_color_str = triangle->color_hex()->str();
                GAMMA_LOG(L_INFO, "Get Triangle from UDP: {}", shape);
                return std::make_shared<types::Triangle>(shape);
            }

            default:
                GAMMA_LOG(L_ERROR, "Invalid FB Figure type");
                return nullptr;
        }
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
    while (m_impl->udp_socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = m_impl->udp_socket->receiveDatagram();
        auto shape = m_impl->parse_data(std::move(datagram));
        if(!shape)
        {
            GAMMA_LOG(L_WARN, "Invalid shape parsing, skip");
            return;
        }

        {
            std::scoped_lock lock(m_guard);
            m_impl->shape_queue.push(std::move(shape));
        }
        m_cond.notify_one();
    }
}

}