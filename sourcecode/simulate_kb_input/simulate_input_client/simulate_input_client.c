//参考了Linux高性能服务器编程,chapter-16,16-4stress_client.cpp
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h> //daemonize
#include <termio.h>
#include <netinet/tcp.h>

#define bool  int
#define false 0
#define true  1

int stop = 0;

struct termios stored_settings;
void initkeyboard(void)
{
    struct termios new_settings;
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= (~ECHO);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
}

void unitkeyboard(void)
{
    tcsetattr(0, TCSANOW, &stored_settings);
}

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epoll_fd, int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    //event.events = EPOLLOUT | EPOLLET | EPOLLERR;
    event.events = EPOLLOUT | EPOLLERR;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

bool write_nbytes(int sockfd, const char* buffer, int len)
{
    int bytes_write = 0;

    while(1)
    {
        bytes_write = send(sockfd, buffer, len, 0);

        if(bytes_write == -1)
        {
            return false;
        }
        else if(bytes_write == 0)
        {
            return false;
        }

        len -= bytes_write;
        buffer = buffer + bytes_write;

        if(len <= 0)
        {
            return true;
        }
    }
}

bool read_once(int sockfd, char* buffer, int len)
{
    int bytes_read = 0;
    memset(buffer, '\0', len);
    bytes_read = recv(sockfd, buffer, len, 0);

    if(bytes_read == -1)
    {
        return false;
    }
    else if(bytes_read == 0)
    {
        return false;
    }

    printf("read in %d bytes from socket %d with content: %s\n",
           bytes_read, sockfd, buffer);
    return true;
}

void start_conn(int epoll_fd, const char* ip, int port)
{
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        printf("socket fail\n");
    }

    int on = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (void*)&on,
               sizeof(on));
    int sndlowat = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDLOWAT,
               (const void*) &sndlowat, sizeof(int));

    if(connect(sockfd, (struct sockaddr*)&address,
               sizeof(address)) == 0)
    {
        //printf( "build connection\n");
        addfd(epoll_fd, sockfd);
    }
    else
    {
        perror("create fail\n");
    }
}

void close_conn(int epoll_fd, int sockfd)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sockfd, 0);
    close(sockfd);
}

void signal_exit_func(int signo)
{
    printf("exit sig is %d\n", signo);
    stop = 1;
}

void signal_exit_handler()
{
    if(0)
    {
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = signal_exit_func;
        sigaction(SIGINT, &sa,
                  NULL);//当按下ctrl+c时，它的效果就是发送SIGINT信号
        sigaction(SIGTERM, &sa, NULL);//kill pid
        sigaction(SIGQUIT, &sa, NULL);//ctrl+\代表退出SIGQUIT
        //SIGSTOP和SIGKILL信号是不可捕获的,所以下面两句话写了等于没有写
        sigaction(SIGKILL, &sa, NULL);//kill -9 pid
        sigaction(SIGSTOP, &sa, NULL);//ctrl+z代表停止
    }

    //#define    SIGTERM        15
    //#define    SIGKILL        9
    //kill和kill -9，两个命令在linux中都有杀死进程的效果，然而两命令的执行过程却大有不同，在程序中如果用错了，可能会造成莫名其妙的现象。
    //执行kill pid命令，系统会发送一个SIGTERM信号给对应的程序。
    //执行kill -9 pid命令，系统给对应程序发送的信号是SIGKILL，即exit。exit信号不会被系统阻塞，所以kill -9能顺利杀掉进程。
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("usage: %s ip port \n", argv[0]);
        exit(-1);
    }

    //signal(SIGHUP, SIG_IGN); //开启的话，就捕获不到终端窗口关闭的信号了。即窗口关闭，进程仍然进行。
    signal(SIGPIPE, SIG_IGN);
    initkeyboard();
    signal_exit_handler();
    int maxclients = 1;
    int epoll_fd = epoll_create(
                   1024);  /* 1024 is just a hint for the kernel */
    start_conn(epoll_fd, argv[1], atoi(argv[ 2 ]));
    //epoll_event events[ 10000 ];
    struct epoll_event* events = (struct epoll_event*)malloc(
                                 sizeof(struct epoll_event) * (maxclients));
    char buffer[ 2048 ];

    while(!stop)
    {
        int fds = epoll_wait(epoll_fd, events, maxclients, 2000);

        for(int i = 0; i < fds; i++)
        {
            int sockfd = events[i].data.fd;

            if(events[i].events & EPOLLIN)
            {
                if(! read_once(sockfd, buffer, 2048))
                {
                    close_conn(epoll_fd, sockfd);
                }

                struct epoll_event event;

                //event.events = EPOLLOUT | EPOLLET | EPOLLERR;
                event.events = EPOLLOUT | EPOLLERR;

                event.data.fd = sockfd;

                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sockfd, &event);
            }
            else if(events[i].events & EPOLLOUT)
            {
                char c = getchar();

                if(! write_nbytes(sockfd, &c, 1))
                {
                    close_conn(epoll_fd, sockfd);
                }

                struct epoll_event event;

                //event.events = EPOLLIN | EPOLLET | EPOLLERR;
                //event.events = EPOLLIN | EPOLLERR;
                event.events = EPOLLOUT | EPOLLERR;

                event.data.fd = sockfd;

                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sockfd, &event);
            }
            else if(events[i].events & EPOLLERR)
            {
                close_conn(epoll_fd, sockfd);
            }
        }
    }

    close(epoll_fd);

    if(events)
    {
        free(events);
    }

    unitkeyboard();
    printf("exit!\n");
    return 0;
}
