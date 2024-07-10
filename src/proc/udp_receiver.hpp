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

    void worker_thread() override;

signals:
    void shape_processed(std::shared_ptr<types::IShape>);

private slots:
    void read_udp_slot();

private:
    std::mutex m_guard;
    std::condition_variable m_cond;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}