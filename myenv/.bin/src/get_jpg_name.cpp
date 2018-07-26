#include <string>
#include <iostream>

#include <stdlib.h>

using namespace std;
int main(int argc, char** argv)
{
    char* path = argv[1];
    string str(path);

    std::size_t found = str.find_last_of("_");
    if(found == std::string::npos)
    {
        return 0;
    }
    size_t endH = str.find_last_of(".");
    if(endH == std::string::npos)
    {
        return 0;
    }

    string index = str.substr(found+1, endH-found-1);
    
    string id;
    
   if(str.find("tele") != std::string::npos) 
       id = "_0";

   if(str.find("wide") != std::string::npos) 
       id = "_1";
    
   if(str.find("main") != std::string::npos) 
       id = "_0";

   if(str.find("aux") != std::string::npos) 
       id = "_1";

    string name = string("IMG_") + index + id + ".jpg";
    cout << name;

    return 0;
}
