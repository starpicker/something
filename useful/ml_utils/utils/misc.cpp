#include "misc.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

int is_dir(char* filename)
{
    struct stat buf;
    int ret = stat(filename,&buf);
    if(0 == ret)
    {
            if(buf.st_mode & S_IFDIR)
            {
                    //printf("%s is folder\n",filename);
                    return 0;
            }
            else
            {
                    //printf("%s is file\n",filename);
                    return 1;
            }
    }

    return -1;
}

int delete_dir(const char* dirname)
{
    char chBuf[256];
    DIR* dir = NULL;
    struct dirent* ptr;
    int ret = 0;
    dir = opendir(dirname);
    if(NULL == dir)
    {
        return -1;
    }
    while((ptr = readdir(dir)) != NULL)
    {
        ret = strcmp(ptr->d_name, ".");
        if(0 == ret)
        {
            continue;
        }

        ret = strcmp(ptr->d_name, "..");
        if(0 == ret)
        {
            continue;
        }

        snprintf(chBuf, 256, "%s/%s", dirname, ptr->d_name);

        ret = is_dir(chBuf);
        if(0 == ret)
        {
            //printf("%s is dir\n", chBuf);
            ret = delete_dir(chBuf);
            if(0 != ret)
            {
                return -1;
            }
        }
        else if(1 == ret)
        {
            //printf("%s is file\n", chBuf);
            ret = remove(chBuf);
            if(0 != ret)
            {
                return -1;
            }
        }
    }

    (void)closedir(dir);

    ret = remove(dirname);
    if(0 != ret)
    {
        return -1;
    }

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/stat.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define ERR_EXIT(msg,flag)  {perror(msg);goto flag;}

int check_file_status(const char* dir2check)
{
        delete_dir(dir2check);
        mkdir(dir2check, S_IRWXU);

        int length;
        int fd;
        int wd;
        char buffer[BUF_LEN];

        if((fd = inotify_init()) < 0)
            ERR_EXIT("inotify_init", inotify_init_err);

        if((wd = inotify_add_watch(fd, dir2check, IN_MODIFY | IN_CREATE | IN_DELETE )) < 0)
            ERR_EXIT("inofity_add_watch", inotify_add_watch_err);

        fd_set rfd;
        struct timeval tv;

        while(true)
        {
            int retval;
            FD_ZERO(&rfd);
            FD_SET(fd, &rfd);

            /*
                On Linux, select() modifies timeout to reflect the amount of  time  not
                slept;  most  other  implementations  do not do this.  (POSIX.1 permits
                either behavior.)  This causes problems  both  when  Linux  code  which
                reads  timeout  is  ported to other operating systems, and when code is
                ported to Linux that reuses a struct timeval for multiple select()s  in
                a  loop  without  reinitializing  it.  Consider timeout to be undefined
                after select() returns.

                If not reset timeval, cause [sched_delayed] sched: RT throttling activated issue occurs
            */

            tv.tv_sec = 1;
            tv.tv_usec = 0;
            retval = select(fd + 1, &rfd, NULL, NULL, &tv);
            if(retval == 0) continue;
            else if(retval == -1)
                ERR_EXIT("select", select_err);

            // retval > 0
            length = read(fd, buffer, BUF_LEN);
            if(length < 0)
                ERR_EXIT("read", read_err);

            //length >= 0
            int i = 0;
            while(i < length)
            {
                struct inotify_event* event = (struct inotify_event*)&buffer[i];
                if(event->len)
                {
                    if(event->mask & IN_CREATE)
                    {
                        std::string tag(event->name);
                    }
                    else if(event->mask & IN_DELETE)
                    {
                        std::string tag(event->name);
                    }
                    else if(event->mask & IN_MODIFY)
                    {
                    }
                }else
                {
                }

                i += EVENT_SIZE + event->len;
            }
        }
success_exit:
        (void)inotify_rm_watch(fd, wd);
        (void)close(fd);
        return 0;

read_err:
select_err:
inotify_add_watch_err:
        (void)inotify_rm_watch(fd, wd);
        (void)close(fd);
inotify_init_err:
        ;

    return 0;
}

#include <map>
void every_yellow_printf(const char* file, const int line, int interval, const char* format, ...)
{
    static std::map<std::string, int> m;

    struct timeval tv_current;
    gettimeofday(&tv_current, NULL);

    const std::string key = std::string(file) + std::to_string(line);
    if(m.find(key) != m.end() && m[key] + interval > tv_current.tv_sec)
    {
        return;
    }

    if(m.find(key) != m.end())
    {
        yprintf(format);
    }

    m[key] = tv_current.tv_sec;
}

#include <chrono>

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

const std::string getCurrentSystemTime(void)
{
  auto tt = std::chrono::system_clock::to_time_t
  (std::chrono::system_clock::now());
  struct tm* ptm = localtime(&tt);
  struct timeval t;
  gettimeofday(&t, 0);
  char date[60] = {0};
  sprintf(date, "%d-%02d-%02d %02d:%02d:%02d:%02d",
    (int)ptm->tm_year + 1900,(int)ptm->tm_mon + 1,(int)ptm->tm_mday,
    (int)ptm->tm_hour,(int)ptm->tm_min,(int)ptm->tm_sec,(int)(t.tv_usec/1000));

  return std::string(date);
}

unsigned long get_timepoint(void)
{
    struct timeval t;
    gettimeofday(&t, NULL);

    return (t.tv_sec*1000+t.tv_usec/1000);
}

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const std::string getLocalIP(const char* device)
{
    int sock_get_ip;
    char ipaddr[50];

    struct sockaddr_in* sin;
    struct ifreq ifr_ip;

    if((sock_get_ip = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return "";
    }

    memset(&ifr_ip, 0, sizeof(ifr_ip));
    strncpy(ifr_ip.ifr_name, device, sizeof(ifr_ip.ifr_name) - 1);

    if(ioctl(sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0)
    {
        close(sock_get_ip);
        return "";
    }
    sin = (struct sockaddr_in*)&ifr_ip.ifr_addr;
    strcpy(ipaddr, inet_ntoa(sin->sin_addr));

    close(sock_get_ip);

    return ipaddr;
}

const std::string getLocalMac(const char* device)
{
    int sock_mac;
    struct ifreq ifr_mac;
    char mac_addr[30];
    sock_mac = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_mac == -1)
    {
        return "";
    }

    memset(&ifr_mac, 0, sizeof(ifr_mac));
    strncpy(ifr_mac.ifr_name, device, sizeof(ifr_mac.ifr_name) - 1);

    if(ioctl(sock_mac, SIOCGIFHWADDR, &ifr_mac) < 0)
    {
        close(sock_mac);
        return "";
    }

    sprintf(mac_addr, "%02x%02x%02x%02x%02x%02x",
        (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],
        (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],
        (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],
        (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],
        (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],
        (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]
    );

    close(sock_mac);

    return mac_addr;
}
