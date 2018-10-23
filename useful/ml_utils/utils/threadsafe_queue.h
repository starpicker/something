#ifndef ML_THREADSAFE_QUEUE_H_
#define ML_THREADSAFE_QUEUE_H_

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue(){}
    threadsafe_queue(threadsafe_queue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue=other.data_queue;
    }
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value=data_queue.front();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    T try_pop_value()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return T();
        T res(data_queue.front());
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

    T& front()
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.front();
    }

    const T& front() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.front();
    }

    T& back()
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.back();
    }

    const T& back()const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.back();
    }

    void pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.pop();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.size();
    }
};

#endif // ML_THREADSAFE_QUEUE_H_
