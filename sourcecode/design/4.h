#pragma once

#include <iostream>
using std::cout;
using std::endl;

struct Data;
Data* special4(int a){cout << "special return 4" << endl; return nullptr;}
