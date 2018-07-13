#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <string>

using namespace std;

string getdate(void)
{
    time_t timer;
    struct tm* t_tm;
    time(&timer);
    t_tm = localtime(&timer);
    char date[128];
    sprintf(date, "%4d-%02d-%02d", t_tm->tm_year + 1900, t_tm->tm_mon + 1, t_tm->tm_mday);

    return date;
}

int main()
{
    string date = getdate();
    //cout << date << endl;

    system("curl http://xx.xx.xx.xxx/dailybuild/xxx/xxx_xxx_xxx/ > /tmp/1.txt");

    ifstream in("/tmp/1.txt");

    string filename;
    string line;

    getline(in, line);

    string startmark = R"(/dailybuild/xxx/xxxxxxxxxxx/xxxxxxxxxxxxxxxxxxxxxxx)";
    string endmark = date;

    auto startpos = line.rfind(startmark);
    auto endpos = line.rfind(endmark);

    string workstring = line.substr(startpos, endpos-startpos);
    startpos = workstring.find(endmark);
    workstring = workstring.substr(0, startpos);
    startpos = workstring.find(R"(">)");
    workstring = workstring.substr(0, startpos);


    cout << workstring << endl;
    workstring = string(R"(curl http://xx.xx.xx.xxx)") + workstring + R"(fastboot/ > /tmp/2.txt)";
    system(workstring.c_str());

    ifstream in1("/tmp/2.txt");
    getline(in1, line);
    auto zipmark = string("userdebug_fastboot.7z");
    endpos = line.rfind(zipmark);
    endpos = line.rfind(zipmark, endpos -1);
    startpos = line.rfind(startmark);
    string rompath = string("/mnt/z/xxxxxxxxxx/") + date;
    string cmd = string("mkdir ") + rompath;
    system(cmd.c_str());

    workstring = string(R"(wget http://xx.xx.xx.xxx)") + line.substr(startpos, endpos - startpos + zipmark.size()) + string(" -P ") + rompath;
    //cout << workstring << endl;
    //
    system(workstring.c_str());
    //
    return 0;
}
