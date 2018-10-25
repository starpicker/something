#pragma once

#include <iostream>
using std::cout;
using std::endl;

struct Data;
int init1(int){cout << "init1" << endl; return 1;}
int set1(int){cout << "set1" << endl; return 1;}
int process1(Data*){cout << "process1" << endl; return 1;}
int unit1(void){cout << "unit1" << endl; return 1;}
