#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

template<typename _Func>
class thread_pool
{
public:
    inline static const auto THREADS = std::thread::hardware_concurrency();

public:
    using job_t = _Func;

private:
    void work();

public:
    thread_pool(size_t threads_num = THREADS);
    ~thread_pool();

    template<typename _F, typename... _Args>
    auto add_job(_F&& f, _Args&&... args)->std::future<typename std::result_of<_F(_Args...)>::type>;

    void shutdown();

    void wait_all() noexcept;

private:
    std::vector<std::thread> m_pool;
    std::queue<job_t>        m_queue;
    std::mutex               m_queue_mutex;
    std::condition_variable  m_cond;
    bool                     m_stopped;
};

template<typename _Func>
inline void thread_pool<_Func>::work()
{
    while (true)
    {
        job_t job;
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_cond.wait(lock, [this] { return !(m_queue.empty() && m_stopped); });
            if (m_stopped || m_queue.empty())
                return;

            job = std::move(m_queue.front());

            m_queue.pop();
        }

        job();
    }
}

template<typename _Func>
inline thread_pool<_Func>::thread_pool(size_t threads_num /* = THREADS */) :
    m_pool(),
    m_queue(),
    m_queue_mutex(),
    m_cond(),
    m_stopped(false)
{
    for (size_t i{}; i < threads_num; ++i)
        m_pool.emplace_back(std::thread{ [this]() { this->work(); } });
}

template<typename _Func>
inline thread_pool<_Func>::~thread_pool()
{
    if (!m_stopped)
        this->shutdown();
}

template<typename _Func>
template<typename _F, typename... _Args>
inline auto thread_pool<_Func>::add_job(_F&& f, _Args&&... args) -> std::future<typename std::result_of<_F(_Args...)>::type>
{
    using result_t = typename std::result_of<_F(_Args...)>::type;

    auto job = std::make_shared<std::packaged_task<result_t()>>(std::bind(std::forward<_F>(f), std::forward<_Args>(args)...));

    std::future<result_t> res = job->get_future();
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);

        if (m_stopped)
            throw std::runtime_error("Trying to add work to stopped thread_poll");

        m_queue.emplace([job]() { (*job)(); });
    }

    m_cond.notify_one();

    return res;
}

template<typename _Func>
inline void thread_pool<_Func>::shutdown()
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_stopped = true;
    }

    m_cond.notify_all();

    this->wait_all();
}

template<typename _Func>
inline void thread_pool<_Func>::wait_all() noexcept
{
    for (auto&& t : m_pool)
        if (t.joinable())
            t.join();
}