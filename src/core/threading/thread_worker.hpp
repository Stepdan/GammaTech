#pragma once

#include <atomic>
#include <condition_variable>
#include <exception>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>

namespace gamma::threading {

class ThreadWorker
{
public:
    ThreadWorker();
    ~ThreadWorker();

    virtual void run_worker();
    virtual void stop_worker();

    bool is_running() const { return m_is_running; }

    bool has_exception() const { return !!m_exception_ptr; }
    std::exception_ptr get_exception() { return m_exception_ptr; }
    void reset_exception() { m_exception_ptr = nullptr; }

protected:
    virtual void worker_thread() = 0;

protected:
    std::atomic_bool m_is_running{false};
    std::atomic_bool m_need_stop{false};
    std::exception_ptr m_exception_ptr;
    std::condition_variable m_cond;

private:
    std::thread m_worker;
};

}  // namespace gamma::threading