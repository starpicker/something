#pragma once

#include "1.h"
#include "2.h"

#include <type_traits>
using std::is_same_v;

namespace ml_crtp
{
template <typename T>
class A
{
public:
    virtual ~A(){}
    constexpr int getid(void){return T::id;}

    int init(void* a);
    int set(void* a);
    int process(void* a);

private:
    A(){}
    friend T;
private:
    void* common;
};

class A1 : public A<A1>
{
public:
    friend A<A1>;
    struct A1_P1{int a;};
    struct A1_P2{Data* a;};

private:
    int init(int a){return init1(a);}
    int set(int a){return set1(a);}
    int process(Data* a){return process1(a);}
    ~A1(){unit1();}

    const static int id = 1;
};

class A2 : public A<A2>
{
public:
    friend A<A2>;
    struct A2_P1{int a; int b;};
    struct A2_P2{Data* a; Data* b;};

private:
    int init(int a, int b){return init2(a, b);}
    int set(int a, int b ){return set2(a, b);}
    int process(Data* a, Data* b){return process2(a, b);}
    ~A2(){unit2();}

    const static int id = 2;
};

template <typename T>
int A<T>::init(void* a)
{
    if constexpr(is_same_v<A1, T>)
        return ((T*)this)->init(((A1::A1_P1*)(a))->a);
    else if constexpr(is_same_v<A2, T>)
        return ((T*)this)->init(((A2::A2_P1*)(a))->a, ((A2::A2_P1*)(a))->b);
}

template <typename T>
int A<T>::set(void* a)
{
    if constexpr(is_same_v<A1, T>)
        return ((T*)this)->set(((A1::A1_P1*)(a))->a);
    else if constexpr(is_same_v<A2, T>)
        return ((T*)this)->set(((A2::A2_P1*)(a))->a, ((A2::A2_P1*)(a))->b);
}

template <typename T>
int A<T>::process(void* a)
{
    if constexpr(is_same_v<A1, T>)
        return ((T*)this)->process(((A1::A1_P2*)(a))->a);
    else if constexpr(is_same_v<A2, T>)
        return ((T*)this)->process(((A2::A2_P2*)(a))->a, ((A2::A2_P2*)(a))->b);
}
} // ml_crtp
