#include <string>
#include <iostream>

#include <stdlib.h>

using namespace std;
int main(int argc, char** argv)
{
    char* path = argv[1];
    string str(string(path).substr(string(path).find_last_of("/") + 1));
    std::size_t found = str.find_last_of("x");
    if(found == std::string::npos)
    {
        return 0;
    }
    size_t startW = str.find_last_of("_", found) + 1;
    if(startW == std::string::npos)
        startW = 0;

    const char* widstr = str.substr(startW, found-startW).c_str();
    int width = atoi(widstr);
    
    cout << width; 
    return 0;    
}
