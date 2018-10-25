#pragma once

#include "1.h"
#include "2.h"

#include <type_traits>
using std::is_same_v;

namespace ml_policy
{
template <typename T>
class A;

class A1
{
    friend A<A1>;
public:
    struct A1_P1{int a;};
    struct A1_P2{Data* a;};

private:
    static const int id = 1;

    int init(int a){return init1(a);}
    int set(int a){return set1(a);}
    int process(Data* a){return process1(a);}
    ~A1(){unit1();}
};

class A2
{
    friend A<A2>;
public:
    struct A2_P1{int a; int b;};
    struct A2_P2{Data* a; Data* b;};

private:
    static const int id = 2;

   int init(int a, int b){return init2(a, b);}
   int set(int a, int b ){return set2(a, b);}
   int process(Data* a, Data* b){return process2(a, b);}
    ~A2(){unit2();}
};

template <typename T>
class A
{
public:
    constexpr int getid(void){return T::id;};

    int init(void* a)
    {
        if constexpr(is_same_v<A1, T>)
            return t.init(((A1::A1_P1*)(a))->a);
        else if constexpr(is_same_v<A2, T>)
            return t.init(((A2::A2_P1*)(a))->a, ((A2::A2_P1*)(a))->b);
    }

    int set(void* a)
    {
        if constexpr(is_same_v<A1, T>)
            return t.set(((A1::A1_P1*)(a))->a);
        else if constexpr(is_same_v<A2, T>)
            return t.set(((A2::A2_P1*)(a))->a, ((A2::A2_P1*)(a))->b);
    }

    int process(void* a)
    {
        if constexpr(is_same_v<A1, T>)
            return t.process(((A1::A1_P2*)(a))->a);
        else if constexpr(is_same_v<A2, T>)
            return t.process(((A2::A2_P2*)(a))->a, ((A2::A2_P2*)(a))->b);
    }

private:
    void* common;
    T t;
};
} // ml_policy
