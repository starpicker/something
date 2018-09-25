#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#include <signal.h>
#include <fcntl.h>

#include "simulate_input.cpp"

#define bool  int
#define false 0
#define true  1
#define PORT        1883
#define MAXSIZE     1024
#define LISTENQ     512
#define FDSIZE      1024
#define EPOLLEVENTS 60000

static int socket_bind(int port);
static void do_epoll(int listenfd);
static void handle_events(int epollfd,
                          struct epoll_event* events, int num, int listenfd,
                          char* buf);
static void handle_accpet(int epollfd, int listenfd);
static void do_read(int epollfd, int fd, char* buf);
static void do_write(int epollfd, int fd, char* buf);
static void add_event(int epollfd, int fd, int state);
static void modify_event(int epollfd, int fd, int state);
static void delete_event(int epollfd, int fd, int state);

void init_signal(void)
{
    signal(SIGCHLD, SIG_DFL);
    signal(SIGPIPE, SIG_IGN);
}

void daemon_run_method1()//来自https://github.com/baloonwj/flamingo
{
    int pid;
    signal(SIGCHLD, SIG_IGN);
    //1）在父进程中，fork返回新创建子进程的进程ID；
    //2）在子进程中，fork返回0；
    //3）如果出现错误，fork返回一个负值；
    pid = fork();

    if(pid < 0)
    {
        //std::cout << "fork error" << std::endl;
        exit(-1);
    }
    //父进程退出，子进程独立运行
    else if(pid > 0)
    {
        exit(0);
    }

    //之前parent和child运行在同一个session里,parent是会话（session）的领头进程,
    //parent进程作为会话的领头进程，如果exit结束执行的话，那么子进程会成为孤儿进程，并被init收养。
    //执行setsid()之后,child将重新获得一个新的会话(session)id。
    //这时parent退出之后,将不会影响到child了。
    setsid();
    int fd;
    fd = open("/dev/null", O_RDWR, 0);

    if(fd != -1)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
    }

    if(fd > 2)
    {
        close(fd);
    }
}

bool daemon_run_method2() //Linux高性能服务器编程.pdf,游双
{
    //创建子进程，关闭父进程，这样可以使程序在后台进行
    pid_t pid = fork();

    if(pid < 0)
    {
        return false;
    }
    else if(pid > 0)
    {
        exit(0);
    }

    //设置文件权限掩码。当进程创建新文件时，文件的权限将是mode & 0777
    //umask( 0 );
    //创建新的会话，设置本进程为进程组的首领
    pid_t sid = setsid();

    if(sid < 0)
    {
        return false;
    }

    if((chdir("/")) < 0)
    {
        return false;
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    //将标准输入、输出和错误输出都定向到/dev/null文件
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);
    return true;
}

int g_kbfd = -1;
int main(int argc, char* argv[])
{
    init_signal();
    g_kbfd = simulate_input_init();
    //守护者进程
    bool bdaemon = false;

    if(bdaemon)
    {
        daemon_run_method1();
    }

    int  listenfd;
    listenfd = socket_bind(PORT);
    listen(listenfd, LISTENQ);
    do_epoll(listenfd);
    simulate_input_unit(g_kbfd);
    return 0;
}

static int socket_bind(int port)
{
    int  listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd == -1)
    {
        perror("socket error:");
        exit(1);
    }

    //一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。
    int reuse_addr = 1;

    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                  &reuse_addr, sizeof(reuse_addr)) == -1)
    {
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if(bind(listenfd, (struct sockaddr*)&servaddr,
            sizeof(servaddr)) == -1)
    {
        perror("bind error: ");
        exit(1);
    }

    //printf("listen on: %d,listenfd=%d\n",PORT,listenfd);
    printf("listen on: %d\n", PORT);
    return listenfd;
}

static void do_epoll(int listenfd)
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    int ret;
    char buf[MAXSIZE];
    memset(buf, 0, MAXSIZE);
    //创建一个描述符
    epollfd = epoll_create(FDSIZE);
    //添加监听描述符事件
    add_event(epollfd, listenfd, EPOLLIN);

    for(; ;)
    {
        //获取已经准备好的描述符事件
        ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
        handle_events(epollfd, events, ret, listenfd, buf);
    }

    close(epollfd);
}

static void handle_events(int epollfd,
                          struct epoll_event* events, int num, int listenfd,
                          char* buf)
{
    int i;
    int fd;

    //进行选好遍历
    for(i = 0; i < num; i++)
    {
        fd = events[i].data.fd;

        //根据描述符的类型和事件类型进行处理
        if((fd == listenfd) && (events[i].events & EPOLLIN))
        {
            handle_accpet(epollfd, listenfd);
        }
        else if(events[i].events & EPOLLIN)
        {
            do_read(epollfd, fd, buf);
        }

        //else if (events[i].events & EPOLLOUT)
        //do_write(epollfd,fd,buf);
    }
}

static void handle_accpet(int epollfd, int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t  cliaddrlen = sizeof(cliaddr);
    clifd = accept(listenfd, (struct sockaddr*)&cliaddr,
                   &cliaddrlen);

    if(clifd == -1)
    {
        perror("accpet error:");
    }
    else
    {
        //printf("accept a new client: %s:%d,fd=%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port,clifd);
        printf("accept a new client: %s:%d\n",
               inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
        int rcvlowat = 1;
        setsockopt(clifd, SOL_SOCKET, SO_RCVLOWAT,
                   (const void*) &rcvlowat, sizeof(int));
        //添加一个客户描述符和事件
        add_event(epollfd, clifd, EPOLLIN);
    }
}

static void do_read(int epollfd, int fd, char* buf)
{
    int nread;
    nread = read(fd, buf, MAXSIZE);

    if(nread == -1)
    {
        perror("read error:");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    }
    else if(nread == 0)
    {
        fprintf(stderr, "\nclient close\n");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    }
    else
    {
        //printf("read message is: %s,fd=%d\n",buf,fd);
        // printf("%s", buf);
        // fflush ( stdout );
        simulate_input_char(g_kbfd, buf[0]);
        modify_event(epollfd, fd, EPOLLIN);
        //修改描述符对应的事件，由读改为写
        //modify_event(epollfd,fd,EPOLLOUT);
    }
}

static void do_write(int epollfd, int fd, char* buf)
{
    int nwrite;
    nwrite = write(fd, buf, strlen(buf));

    if(nwrite == -1)
    {
        perror("write error:");
        close(fd);
        delete_event(epollfd, fd, EPOLLOUT);
    }
    else
    {
        modify_event(epollfd, fd, EPOLLIN);
    }

    memset(buf, 0, MAXSIZE);
}

static void add_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;//LT
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

static void delete_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

static void modify_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}
