#ifndef ML_MISC_H_
#define ML_MISC_H_

#ifndef MLLOG
#define MLLOG //printf
#endif

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
            MLLOG("%s cost time is %ld ms", label, time_diff);
        }
private:
        MLTIME operator =(const MLTIME&);

private:
    struct timeval t_start;
    struct timeval t_end;
    const char* label;
};

#define MLTIME t = __func__;

struct EnterExit {
        EnterExit(const char* l = 0):label(l)
        {
            MLLOG("%s enter", label);
        }

        ~EnterExit()
        {
            MLLOG("%s exit", label);
        }

private:
    const char* label;
};
#define EE  EnterExit ee = __func__;

int delete_dir(const char* dirname);
int check_file_status(const char* dir2check);

#include <string>
const std::string getLocalIP(const char* device);
const std::string getLocalMac(const char* device);
unsigned long get_timepoint(void);

#include <memory>
template <typename T>
std::shared_ptr<T> make_shared_array(size_t size)
{
    return std::shared_ptr<T>(new T[size], std::default_delete<T[]>());
}

#define print_none              printf("\033[0m");fflush(stdout);
#define print_r/*ed*/           printf("\033[1;31;40m");
#define print_g/*reen*/         printf("\033[1;32;40m");
#define print_y/*ellow*/        printf("\033[1;33;40m");
#define print_b/*lue*/          printf("\033[1;34;40m");
#define print_p/*urple*/        printf("\033[1;35;40m");
#define print_c/*yan*/          printf("\033[1;36;40m");

#include <stdarg.h>
#define colorprintf(c)          inline void c##printf(const char* format, ...) \
                                {                                      \
                                    va_list args;                      \
                                    va_start(args, format);            \
                                    print_##c                          \
                                    vprintf(format, args);             \
                                    print_none                         \
                                    va_end(args);                      \
                                }

colorprintf(r)
colorprintf(g)
colorprintf(y)
colorprintf(b)
colorprintf(p)
colorprintf(c)

void every_yellow_printf(const char* file, const int line, int interval, const char* format, ...);

#endif // ML_MISC_H_

