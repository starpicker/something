#pragma once

#include <iostream>
using std::cout;
using std::endl;

struct Data;
int init2(int, int){cout << "init2" << endl; return 2;}
int set2(int, int){cout << "set2" << endl; return 2;}
int process2(Data*, Data*){cout << "process2" << endl; return 2;}
int unit2(void){cout << "unit2" << endl; return 2;}
