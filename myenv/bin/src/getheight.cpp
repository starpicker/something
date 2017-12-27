#include <string>
#include <iostream>

#include <stdlib.h>

using namespace std;
int main(int argc, char** argv)
{
    char* path = argv[1];
    string str(path);

    std::size_t found = str.find_last_of("x");
    if(found == std::string::npos)
    {
        return 0;
    }
    size_t endH = str.find("_", found);
    if(endH == std::string::npos)
        endH = str.find(".", found);

    const char* heistr = str.substr(found+1, endH-found-1).c_str();
    int height = atoi(heistr);
       
    cout << height; 

    return 0;    
}
