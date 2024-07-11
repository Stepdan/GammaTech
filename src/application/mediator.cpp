#include "mediator.hpp"

#include "core/log/log.hpp"

namespace gamma {

Mediator::Mediator(const std::shared_ptr<MainWindow>& mw)
    : m_main_window(mw)
    , m_udp_receiver(std::make_unique<proc::UDPReceiver>("127.0.0.1", 10002))
{
    m_udp_receiver->run_worker();
}


Mediator::~Mediator()
{
    GAMMA_LOG(L_DEBUG, "Mediator destruction");
}

}