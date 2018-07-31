#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <unistd.h>
#include <linux/tcp.h>

#define HELLO_WORLD_SERVER_PORT    8079 
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

int fileNameServerInit(int* ss)
{
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);

    int server_socket = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket < 0)
    {
        printf("Create Socket Failed!");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    
    const char chOpt = 1;
    int nErr = setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));

    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT); 
        exit(1);
    }

    if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("Server Listen Failed!"); 
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);


    int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
    
    *ss = server_socket;
    return new_server_socket;
}

void fileNameServerSend(int server_socket, char* filename, int len)
{
    send(server_socket, filename, len, 0);
}

void fileNameServerUnit( int server_socket, int new_server_socket)
{
    close(new_server_socket);
        
    close(server_socket);
}

int main()
{
    int ss;
    int s = fileNameServerInit(&ss);
    char* filepath = (char*)"/sdcard/lma/1.jpg";
    fileNameServerSend(s, filepath, strlen(filepath));
    fileNameServerUnit(s, ss);

    return 0;
} 


