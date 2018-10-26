#pragma once

#include "1.h"
#include "2.h"

#include <utility>
using std::forward;

namespace ml_delegate
{
template <typename T>
class A;

class A1
{
    friend A<A1>;
private:
    int init(int a){return init1(a);}
    int set(int a){return set1(a);}
    int process(Data* a){return process1(a);}
    int unit(void){return unit1();}
};

class A2
{
    friend A<A2>;
private:
    int init(int a, int b){return init2(a, b);}
    int set(int a, int b){return set2(a, b);}
    int process(Data* a, Data* b){return process2(a, b);}
    int unit(void){return unit2();}
};

template <typename T>
class A
{
public:
    template <typename ...Args>
    int init(Args&& ...args)
    {
        createDelegate(&t, &T::init)(forward<Args>(args)...);
    }

    template <typename ...Args>
    int set(Args ...args)
    {
        createDelegate(&t, &T::set)(forward<Args>(args)...);
    }

    template <typename ...Args>
    int process(Args ...args)
    {
        createDelegate(&t, &T::process)(forward<Args>(args)...);
    }

    template <typename ...Args>
    int unit(Args ...args)
    {
        createDelegate(&t, &T::unit)(forward<Args>(args)...);
    }

private:
    void* common;
    T t;

    template <typename U, typename R, typename ...Args>
    class Delegate
    {
    private:
        U* t;
        R (U::*f)(Args ...);
    public:
        Delegate(U* t, R(U::*f)(Args ...args)):t(t), f(f){}
        R operator ()(Args&& ...args)
        {
            return (t->*f)(forward<Args>(args)...);
        }
    };

    template<class U, class R, typename...Args>
    Delegate<U, R, Args...> createDelegate(U* t, R (T::*f)(Args...))
    {
        Delegate<U, R, Args...> delegate = Delegate<U, R, Args...>(t, f);

        return delegate;
    }
};
} // ml_delegate

