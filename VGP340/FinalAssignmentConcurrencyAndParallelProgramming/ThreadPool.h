#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

class ThreadPool
{
public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    template<class F, class... Args>
    auto Enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::invoke_result<F, Args...>::type>
    {
        using return_type = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mQueueMutex);

            // Don't allow enqueueing after stopping the pool
            if (mStop)
            {
                throw std::runtime_error("Enqueue on stopped ThreadPool");
            }

            mTasks.emplace([task]() { (*task)(); });
        }
        mCondition.notify_one();
        return res;
    }

    size_t GetQueueSize();
    size_t GetActiveThreadCount();
    size_t GetTotalThreadCount() const { return mWorkers.size(); }

private:
    std::vector<std::thread> mWorkers;
    std::queue<std::function<void()>> mTasks;
    
    std::mutex mQueueMutex;
    std::condition_variable mCondition;
    std::atomic<bool> mStop;
    std::atomic<size_t> mActiveThreads;
};
