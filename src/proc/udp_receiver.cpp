#include "udp_receiver.hpp"

#include "core/log/log.hpp"

#include <QNetworkDatagram>
#include <QUdpSocket>

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

    std::shared_ptr<types::IShape> parse_data(QNetworkDatagram&& data)
    {
        return nullptr;
    }
};

UDPReceiver::UDPReceiver(const std::string& ip, int port)
    : m_impl(std::make_unique<Impl>(ip, port))
{
    QObject::connect(m_impl->udp_socket.get(), &QUdpSocket::readyRead, this, &UDPReceiver::read_udp_slot);
}

UDPReceiver::~UDPReceiver() = default;

void UDPReceiver::worker_thread()
{
    while(!m_need_stop)
    {
        std::shared_ptr<types::IShape> shape;
        {
            std::unique_lock lock(m_guard);
            m_cond.wait(lock, [this](){ return !m_impl->shape_queue.empty() || m_need_stop; });
            
            if(m_need_stop)
                break;
            
            shape = m_impl->shape_queue.front();
            m_impl->shape_queue.pop();
        }
        shape_processed(shape);
    }

}

void UDPReceiver::read_udp_slot()
{
    GAMMA_LOG(L_INFO, "read_udp_slot");
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