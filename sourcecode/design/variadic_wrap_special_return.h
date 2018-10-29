#pragma once

#include "1.h"
#include "2.h"
#include "3.h"
#include "4.h"

#include <utility>
using std::forward;

#include <string>
#include <sstream>

#ifndef ML_ARGS_STRING
#define ML_ARGS_STRING

template <typename T>
inline void args_string(std::ostringstream& stream, T last)
{
    stream << last;
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

namespace ml_variadic_wrap_special_return
{
namespace detail
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
};

class A2
{
    friend A<A2>;
private:
    decltype(auto) init(int a, int b){return init2(a, b);}
    decltype(auto) set(int a, int b){return set2(a, b);}
    decltype(auto) process(Data* a, Data* b){return process2(a, b);}
    decltype(auto) unit(void){return unit2();}
};

class A3
{
    friend A<A3>;
private:
    decltype(auto) special(int a){return ::special(a);}
};

class A4
{
    friend A<A4>;
private:
    Data* special(int a){return ::special4(a);}
};

template <typename T>
class A
{
public:
    template <typename ...Args>
    decltype(auto) init(Args&& ...args)
    {
        return t.init(forward<Args>(args)...);
    }

    template <typename ...Args>
    decltype(auto) set(Args ...args)
    {
        return t.set(forward<Args>(args)...);
    }

    template <typename ...Args>
    decltype(auto) process(Args ...args)
    {
        return t.process(forward<Args>(args)...);
    }

    template <typename ...Args>
    decltype(auto) unit(Args ...args)
    {
        return t.unit(forward<Args>(args)...);
    }

    template <typename ...Args>
    decltype(auto) special(Args ...args)
    {
        return t.special(forward<Args>(args)...);
    }

private:
    void* common;
    T t;
};
} // detail

#include <type_traits>
using std::conditional_t;

template <auto ID>
class A
{
    using T = conditional_t<ID == 1, detail::A1,
              conditional_t<ID == 2, detail::A2,
              conditional_t<ID == 3, detail::A3,
              conditional_t<ID == 4, detail::A4,
              void                         >>>>;

public:
    template <typename ...Args>
    decltype(auto) init(Args&& ...args)
    {

        cout<< "init" << ID << ": "<< args_string(args...) << endl;

        auto&& _ = a.init(forward<Args>(args)...);
        cout << "init return " << _ << endl;

        return _;
    }

    template <typename ...Args>
    decltype(auto) set(Args ...args)
    {
        return a.set(forward<Args>(args)...);
    }

    template <typename ...Args>
    decltype(auto) process(Args ...args)
    {
        return a.process(forward<Args>(args)...);
    }

    template <typename ...Args>
    decltype(auto) unit(Args ...args)
    {
        return a.unit(forward<Args>(args)...);
    }

    template <typename ...Args>
    decltype(auto) special(Args ...args)
    {
        return a.special(forward<Args>(args)...);
    }

private:
    detail::A<T> a;
    constexpr static auto id = ID;
};

} // ml_variadic_wrap_special_return
