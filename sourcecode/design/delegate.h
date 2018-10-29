#pragma once

#include "1.h"
#include "2.h"
#include "3.h"

#include <utility>
using std::forward;

#define ML_BENCHMARK

/*{{{*/
#ifdef ML_BENCHMARK

#define MLLOG printf
#ifndef MLLOG
#error "please define MLLOG"
#endif

#include <sys/time.h>
#include <string>
#include <memory>
using std::string;
using std::move;

struct MLTIME {
        explicit MLTIME(string& l):label(l)
        {
            gettimeofday(&t_start, 0);
        }

        explicit MLTIME(string&& l):label(move(l))
        {
            gettimeofday(&t_start, 0);
        }

        ~MLTIME()
        {
            gettimeofday(&t_end, 0);
            long time_diff = (t_end.tv_sec - t_start.tv_sec)*1000 + (t_end.tv_usec - t_start.tv_usec)/1000;
            MLLOG("%s cost time is %ld ms\n", label.c_str(), time_diff);
        }
private:
        MLTIME operator =(const MLTIME&);

private:
    struct timeval t_start;
    struct timeval t_end;
    string label;
};

struct EnterExit {
        explicit EnterExit(string& l):label(l)
        {
            MLLOG("%s enter\n", label.c_str());
        }

        explicit EnterExit(string&& l):label(move(l))
        {
            MLLOG("%s enter\n", label.c_str());
        }

        ~EnterExit()
        {
            MLLOG("%s exit\n", label.c_str());
        }

private:
    string label;
};

#endif // ML_BENCHMARK
/*}}}*/

#ifndef ML_ARGS_STRING
#define ML_ARGS_STRING

#include <string>
#include <sstream>

template <typename T>
inline void args_string(std::ostringstream& stream, T last)
{
    stream << last << "\n";
}

template<typename T, typename ...Args>
inline void args_string(std::ostringstream& stream, T first, Args... rest)
{
    stream << first << ", ";
    args_string(stream, rest...);
}

template<typename ...Args>
inline const char* args_string(Args... rest)
{
    std::ostringstream stream;
    args_string(stream, rest...);

    static thread_local std::string str;
    str = stream.str();

    return str.c_str();
}

#endif // ML_ARGS_STRING

#include <string>
using std::to_string;

namespace ml_delegate
{
template <typename T>
class A;

class A1
{
    friend A<A1>;
private:
    decltype(auto) init(int a){return init1(a);}
    decltype(auto) set(int a){return set1(a);}
    decltype(auto) process(Data* a){return process1(a);}
    decltype(auto) unit(void){return unit1();}

private:
    inline constexpr static auto id {1};
};

class A2
{
    friend A<A2>;
private:
    decltype(auto) init(int a, int b){return init2(a, b);}
    decltype(auto) set(int a, int b){return set2(a, b);}
    decltype(auto) process(Data* a, Data* b){return process2(a, b);}
    decltype(auto) unit(void){return unit2();}

private:
    inline constexpr static auto id {2};
};

class A3
{
    friend A<A3>;
private:
    decltype(auto) special(int a){return ::special(a);}

private:
    inline constexpr static auto id {3};
};

template <typename T>
class A
{
public:

#define CREATE_DELEGATE(x)  template<typename ...Args> \
                            decltype(auto) x(Args&& ...args) \
                            { \
                                auto&& _ = createDelegate(&t, &T::x, #x) \
                                           (forward<Args>(args)...); \
                                return _; \
                            }

    CREATE_DELEGATE(init)
    CREATE_DELEGATE(set)
    CREATE_DELEGATE(process)
    CREATE_DELEGATE(unit)
    CREATE_DELEGATE(special) // for A3

private:
    void* common;
    T t;

    template <typename U, typename R, typename M, typename ...Args>
    class Delegate
    {
    private:
        U* t;
        M m;
        R (U::*f)(Args ...);
    public:
        Delegate(U* t, R(U::*f)(Args ...args), M m):t(t), f(f), m(m){}
        R operator ()(Args&& ...args)
        {
#ifdef ML_BENCHMARK
            EnterExit ee {to_string(U::id) + ":" + m};
            MLTIME tt {to_string(U::id) + ":" + m};
            if constexpr(sizeof ...(args))
                MLLOG((to_string(U::id) +  ":" + m  + "'s params: " + args_string(args...)).c_str());
#endif
            return (t->*f)(forward<Args>(args)...);
        }
    };

    template<class U, typename R, typename M, typename...Args>
    Delegate<U, R, M, Args...> createDelegate(U* t, R (T::*f)(Args...), M m)
    {
        Delegate<U, R, M, Args...> delegate = Delegate<U, R, M, Args...>(t, f, m);

        return delegate;
    }
};
} // ml_delegate

