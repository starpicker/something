#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static void mllog(const char * fmt, ...)
{
    FILE* fp = fopen("/data/misc/camera/mllog2.txt", "a+");
    if(fp)
    {
        va_list arg;
        char buf[1024 * 4] = {0};
        va_start(arg, fmt);
        vsprintf(buf, fmt,arg);
        va_end(arg);
        fwrite(buf,1,strlen(buf),fp);
        if(buf[strlen(buf)-1] != '\n')
            fwrite("\n",1,1,fp);
        fflush(fp);
        fclose(fp);
    }
}

extern "C" long long GetCurrentTime()
{
    struct timeval time;
    gettimeofday(&time,NULL);
    long long curtime = ((long long)(time.tv_sec))*1000 + time.tv_usec/1000;
    return curtime;
}

