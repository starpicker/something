#pragma once

namespace ml_ob
{
#include "1.h"
#include "2.h"

class A
{
private:
    void* common;
protected:
    ~A(){}
};

class A1 : public A
{
public:
    A1(int a){init1(a);}
    int set(int a){return set1(a);}
    int process(Data* a){return process1(a);}
    ~A1(){unit1();}
};

class A2 : public A
{
public:
    A2(int a, int b){init2(a, b);}
    int set(int a, int b ){return set2(a, b);}
    int process(Data* a, Data* b){return process2(a, b);}
    ~A2(){unit2();}
};
} // ml_ob
