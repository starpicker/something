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


#include <sys/time.h>

struct MLTIME {
        MLTIME(const char* l = 0):label(l)
        {
            gettimeofday(&t_start, 0);
        }

        ~MLTIME()
        {
            gettimeofday(&t_end, 0);
            long time_diff = (t_end.tv_sec - t_start.tv_sec)*1000 + (t_end.tv_usec - t_start.tv_usec)/1000;
            logToFile("%s cost time is %ld ms", label, time_diff);
        }
private:
        MLTIME operator =(const MLTIME&);

private:
    struct timeval t_start;
    struct timeval t_end;
    const char* label;
};

#define T MLTIME t = __func__;
