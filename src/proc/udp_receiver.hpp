#pragma once

#include "core/threading/thread_worker.hpp"

#include "core/types/shape.hpp"

#include <memory>

#include <QObject>

namespace gamma::proc {

class UDPReceiver : public QObject, public threading::ThreadWorker
{
    Q_OBJECT

public:
    UDPReceiver(const std::string& ip, int port);
    ~UDPReceiver();

signals:
    void shape_processed(std::shared_ptr<gamma::types::IShape>);

private slots:
    void on_read_udp();

private:
    void worker_thread() override;

private:
    std::mutex m_guard;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}