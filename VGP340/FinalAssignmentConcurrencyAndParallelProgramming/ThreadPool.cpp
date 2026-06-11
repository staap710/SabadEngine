#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threads)
    : mStop(false)
    , mActiveThreads(0)
{
    for (size_t i = 0; i < threads; ++i)
    {
        mWorkers.emplace_back(
            [this]
            {
                for (;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->mQueueMutex);
                        this->mCondition.wait(lock,
                            [this] { return this->mStop || !this->mTasks.empty(); });
                        
                        if (this->mStop && this->mTasks.empty())
                        {
                            return;
                        }

                        task = std::move(this->mTasks.front());
                        this->mTasks.pop();
                    }

                    ++mActiveThreads;
                    task();
                    --mActiveThreads;
                }
            }
        );
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        mStop = true;
    }
    mCondition.notify_all();
    for (std::thread& worker : mWorkers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

size_t ThreadPool::GetQueueSize()
{
    std::unique_lock<std::mutex> lock(mQueueMutex);
    return mTasks.size();
}

size_t ThreadPool::GetActiveThreadCount()
{
    return mActiveThreads.load();
}
