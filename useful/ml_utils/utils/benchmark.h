#ifndef ML_BENCHMARK_H_
#define ML_BENCHMARK_H_

#include <string>
#include <memory>

#include <sys/time.h>

#include "misc.h"
#include "macro.h"

using std::string;
using std::shared_ptr;

class BenchMark;
typedef shared_ptr<BenchMark> BenchMarkPtr;

#ifdef NDEBUG

#define DISABLE_BENCHMARK //dont touch it

#else

//#define DISABLE_BENCHMARK

#endif // NDEBUG

class BenchMark
{
public:
    BenchMark(string tag, bool show = true, char frequency = 1, char notify_frequency = 10)
    :tag_(tag), size_(0), count_(0), show_(show), frequency_(frequency),
    notify_frequency_(notify_frequency), first_(true), times_(0),
    total_count_(0), total_size_(0), start_time_(0)
    {
#ifdef DISABLE_BENCHMARK
        return;
#endif
    }

    ~BenchMark()
    {
#ifdef DISABLE_BENCHMARK
        return;
#endif
        if(!show_) return;

        rprintf("%s stop benchmark", tag_.c_str());
    }

    void benchData(int size, bool bench_count = false)
    {
#ifdef DISABLE_BENCHMARK
        return;
#endif
        if(!show_) return;

        size_ += size;
        total_size_ += size;

        struct timeval t;
        gettimeofday(&t, 0);
        long timeNow = t.tv_sec*1000 + t.tv_usec/1000;

        if(first_)
        {
            gprintf("%s start benchmark", tag_.c_str());

            lastT_ = timeNow;
            first_ = false;
            start_time_ = timeNow;
        }
        else
        {
            long time_diff = timeNow - lastT_;

            if(time_diff > frequency_*1000)
            {
                if(times_ == notify_frequency_)
                {
                    long time_diff_from_start = timeNow - start_time_;

                    if(!bench_count)
                    {
                        yprintf("%s average comsume %.3f kb data in %.3f seconds",
                                    tag_.c_str(), ((double)total_size_)/1024, ((double)time_diff_from_start)/1000);
                    }
                    else
                    {
                        yprintf("%s average comes %d times in %.3f seconds",
                                    tag_.c_str(), total_count_, ((double)time_diff_from_start)/1000);
                    }

                    times_ = 0;
                }
                else
                {
                    if(!bench_count)
                    {
                        yprintf("%s comsume %.3f kb data in %.3f seconds",
                                    tag_.c_str(), ((double)size_)/1024, ((double)time_diff)/1000);
                        size_ = 0;
                    }
                    else
                    {
                        yprintf("%s comes %d times in %.3f seconds",
                                    tag_.c_str(), count_, ((double)time_diff)/1000);
                        count_ = 0;
                    }
                }

                lastT_ = timeNow;
                times_++;
            }
        }
    }

    void benchCount(void)
    {
#ifdef DISABLE_BENCHMARK
        return;
#endif
        total_count_++;
        count_++;
        benchData(0, true);
    }

private:
    const string tag_;
    int size_;
    int count_;
    bool show_;
    const char frequency_; // how many seconds a notify
    const char notify_frequency_;
    long lastT_; // ms
    bool first_;
    char times_;
    int total_count_;
    int total_size_;
    long start_time_;

DELETE_NOUSE_TRIVIAL_FUNC(BenchMark)
};

#endif // ML_BENCHMARK_H_
