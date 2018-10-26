#pragma once

#include "1.h"
#include "2.h"

#include <utility>
using std::forward;

namespace ml_variadic
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
        t.init(forward<Args>(args)...);
    }

    template <typename ...Args>
    int set(Args ...args)
    {
        t.set(forward<Args>(args)...);
    }

    template <typename ...Args>
    int process(Args ...args)
    {
        t.process(forward<Args>(args)...);
    }

    template <typename ...Args>
    int unit(Args ...args)
    {
        t.unit(forward<Args>(args)...);
    }

private:
    void* common;
    T t;
};
} // ml_variadic
