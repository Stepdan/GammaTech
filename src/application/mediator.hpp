#pragma once

#include <memory>

#include <QObject>

#include "proc/udp_receiver.hpp"

#include "ui/MainWindow.hpp"

namespace gamma {

class Mediator : public QObject
{
    Q_OBJECT

public:
    Mediator(const std::shared_ptr<MainWindow>& main_window);
    ~Mediator();

private:
    std::shared_ptr<MainWindow> m_main_window;

    std::unique_ptr<proc::UDPReceiver> m_udp_receiver;
};

}