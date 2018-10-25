#pragma once

#include "1.h"
#include "2.h"

namespace ml_oo
{
class A
{
public:
    A(int id):id(id){}
    virtual int init(void*) = 0;
    virtual int set(void*) = 0;
    virtual int process(void*) = 0;
    virtual ~A(){}
    int getid(void){return id;}
private:
    int id;
    void* common;
};

class A1 : public A
{
public:
    struct A1_P1{int a;};
    struct A1_P2{Data* a;};

    A1():A(1){}
    int init(void* a){return init1(((A1_P1*)(a))->a);}
    int set(void* a){return set1(((A1_P1*)(a))->a);}
    int process(void* a){return process1(((A1_P2*)(a))->a);}
    ~A1(){unit1();}
};

class A2 : public A
{
public:
    struct A2_P1{int a; int b;};
    struct A2_P2{Data* a; Data* b;};

    A2():A(2){}
    int init(void* a){return init2(((A2_P1*)(a))->a, ((A2_P1*)(a))->b);}
    int set(void* a){return set2(((A2_P1*)(a))->a, ((A2_P1*)(a))->b);}
    int process(void* a){return process2(((A2_P2*)(a))->a, ((A2_P2*)(a))->b);}
    ~A2(){unit2();}
};
} // ml_oo
