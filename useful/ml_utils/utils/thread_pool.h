#ifndef ML_THREAD_POOL_H_
#define ML_THREAD_POOL_H_

#include <deque>
#include <vector>
#include <future>
#include <thread>
#include <functional>

#include "threadsafe_queue.h"

class join_threads
{
std::vector<std::thread> &threads;
public:
    explicit join_threads(std::vector<std::thread> &threads_):threads(threads_){}
    ~join_threads()
    {
        for(unsigned long i = 0 ; i < threads.size();++i)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
    }
};

class thread_pool;
typedef std::shared_ptr<thread_pool> thread_pool_ptr;

class thread_pool
{
public:
    thread_pool(char count):
    joiner(threads)
    {
        done.store(false);
        unsigned const thread_count = count;//std::thread::hardware_concurrency();
        try
        {
            for(unsigned i = 0; i < thread_count; ++i)
            {
                threads.push_back(std::thread(&thread_pool::worker_thread, this));
            }
        }
        catch(...)
        {
            done = true;
            rprintf("create thread_pool failed");

            throw;
        }
    }

    ~thread_pool()
    {
        done = true;
        data_cond.notify_all();
    }

    template<typename FunctionType>
    void submit(FunctionType f, bool high_priority = true)
    {
        if(!high_priority)
            work_queue.push(std::function<void()>(f));
        else
            work_queue_high_priority.push(std::function<void()>(f));

        data_cond.notify_one();
    }

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
    submit4result(FunctionType f)
    {
        typedef typename std::result_of<FunctionType()>::type result_type;

        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.push(std::move(task));

        data_cond.notify_one();

        return res;
    }

private:
    std::atomic_bool done;

    threadsafe_queue<std::function<void()>> work_queue;
    threadsafe_queue<std::function<void()>> work_queue_high_priority;
    std::vector<std::thread> threads;
    join_threads joiner;
    mutable std::mutex mut;
    std::condition_variable data_cond;

    void worker_thread()
    {
        while(!done)
        {
            std::function<void()> task;
            if(!work_queue_high_priority.try_pop(task))
            {
                if(!work_queue.try_pop(task))
                {
                    std::unique_lock<std::mutex> lk(mut);
                    data_cond.wait(lk);
                    continue;
                }
            }

            task();
        }
    }
};

#endif // ML_THREAD_POOL_H_
