#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <stdlib.h>
using namespace std;

string getCurrentTimeStr(void)
{
    time_t t = time(NULL);
    char ch[64] = {0};
    strftime(ch, sizeof(ch) - 1, "%Y-%m-%d", localtime(&t));
    return ch;
}

string readFileIntoString(const char* filename)
{
    ifstream ifile(filename);
    ostringstream buf;
    char ch;

    while(buf && ifile.get(ch))
    {
        buf.put(ch);
    }

    return buf.str();
}
