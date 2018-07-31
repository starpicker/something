#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ifaddrs.h>

#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: mlhttpd/0.0.1\r\n"

void* accept_request(void*);
void  cat(int, FILE*);
void  error_die(const char*);
int   get_line(int, char*, int);
void  headers(int, const char*);
void  not_found(int);
void  serve_file(int, const char*);
int   startup(u_short*);
void  unimplemented(int);
char* getIPAddress(char* ip);

/**********************************************************************/
/* A request has caused a call to accept() on the server port to
 * return.  Process the request appropriately.
 * Parameters: the socket connected to the client */
/**********************************************************************/
void* accept_request(void* param)
{
    char   buf[1024];
    int    numchars;
    char   method[255];
    char   url[255];
    char   path[255];
    size_t i, j;
    struct stat st;
    int    client = *((int*)param);
    char*  domain;
    char*  fileAddress;
    
    numchars = get_line(client, buf, sizeof(buf));
    
    i = 0;
    j = 0;
    while(!ISspace(buf[j]) && (i < sizeof(method) - 1))
    {
        method[i] = buf[j];
        i++;
        j++;
    }
    method[i] = '\0';

    if(strcasecmp(method, "GET"))
    {
        unimplemented(client);
        return NULL;
    }

    i = 0;
    while(ISspace(buf[j]) && (j < sizeof(buf)))
    {
        j++;
    }
    
    while(!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
    {
        url[i] = buf[j];
        i++;
        j++;
    }
    url[i] = '\0';
    
    domain = strstr(url, "://");
    if(domain)
    {
        domain += strlen("://");
        fileAddress = strstr(domain, "/");
        if(fileAddress)
        {
            memcpy(path, fileAddress+1, strlen(fileAddress+1)+1);
        }
    }
    else
    {
        fileAddress = strstr(url, "/");
        if(fileAddress)
        {
            memcpy(path, fileAddress+1, strlen(fileAddress+1)+1);
        }
    }
    
    printf("url: %s, path: %s\n", url, path);
    
    if(stat(path, &st) == -1)
    {
        while((numchars > 0) && strcmp("\n", buf))
        {
            /* read & discard headers */
            numchars = get_line(client, buf, sizeof(buf));
        }
        not_found(client);
    }
    else
    {
        serve_file(client, path);
    }

    close(client);

    return NULL;
}

/**********************************************************************/
/* Put the entire contents of a file out on a socket.  This function
 * is named after the UNIX "cat" command, because it might have been
 * easier just to do something like pipe, fork, and exec("cat").
 * Parameters: the client socket descriptor
 *             FILE pointer for the file to cat */
/**********************************************************************/
void cat(int client, FILE* resource)
{
    char buf[1024];
    int n;
    
    while((n = fread(buf, 1, sizeof(buf), resource)) != 0)
    {
        send(client, buf, n, 0);
    }
}

/**********************************************************************/
/* Print out an error message with perror() (for system errors; based
 * on value of errno, which indicates system call errors) and exit the
 * program indicating an error. */
/**********************************************************************/
void error_die(const char* sc)
{
    perror(sc);
    exit(1);
}

/**********************************************************************/
/* Get a line from a socket, whether the line ends in a newline,
 * carriage return, or a CRLF combination.  Terminates the string read
 * with a null character.  If no newline indicator is found before the
 * end of the buffer, the string is terminated with a null.  If any of
 * the above three line terminators is read, the last character of the
 * string will be a linefeed and the string will be terminated with a
 * null character.
 * Parameters: the socket descriptor
 *             the buffer to save the data in
 *             the size of the buffer
 * Returns: the number of bytes stored (excluding null) */
/**********************************************************************/
int get_line(int sock, char* buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;

    while((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if(n > 0)
        {
            if(c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                if((n > 0) && (c == '\n'))
                recv(sock, &c, 1, 0);
                else
                c = '\n';
            }
            buf[i] = c;
            i++;
        }
        else
        {
            c = '\n';   
        }
    }
    buf[i] = '\0';

    return(i);
}

/**********************************************************************/
/* Return the informational HTTP headers about a file. */
/* Parameters: the socket to print the headers on
 *             the name of the file */
/**********************************************************************/
void headers(int client, const char* filename)
{
    char buf[1024];
    (void)filename;  /* could use filename to determine file type */
    struct stat st;
    stat(filename, &st);
    
    strcpy(buf, "HTTP/1.1 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: image/jpeg\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Length: %ld\r\n", st.st_size);
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Give a client a 404 not found status message. */
/**********************************************************************/
void not_found(int client)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.1 404 NOT FOUND\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: image/jpeg\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "your request because the resource specified\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Send a regular file to the client.  Use headers, and report
 * errors to client if they occur.
 * Parameters: a pointer to a file structure produced from the socket
 *              file descriptor
 *             the name of the file to serve */
/**********************************************************************/
void serve_file(int client, const char* filename)
{
    FILE* resource = NULL;
    int numchars = 1;
    char buf[1024];

    buf[0] = 'A'; buf[1] = '\0';
    while((numchars > 0) && strcmp("\n", buf))
    {
        /* read & discard headers */
        numchars = get_line(client, buf, sizeof(buf));
    } 

    resource = fopen(filename, "r");
    if(resource == NULL)
    {
        not_found(client);   
    }
    else
    {
        headers(client, filename);
        cat(client, resource);
    }
    fclose(resource);
}

/**********************************************************************/
/* This function starts the process of listening for web connections
 * on a specified port.  If the port is 0, then dynamically allocate a
 * port and modify the original port variable to reflect the actual
 * port.
 * Parameters: pointer to variable containing the port to connect on
 * Returns: the socket */
/**********************************************************************/
int startup(u_short* port)
{
    int httpd = 0;
    struct sockaddr_in name;

    httpd = socket(PF_INET, SOCK_STREAM, 0);
    if(httpd == -1)
    {
        error_die("socket");   
    }
    
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
    {
          error_die("bind");  
    }

    if(*port == 0)  /* if dynamically allocating a port */
    {
        unsigned int namelen = sizeof(name);
        if(getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
        {
            error_die("getsockname");
        }
        *port = ntohs(name.sin_port);
    }
    if(listen(httpd, 5) < 0)
    {
        error_die("listen");
    }

    return(httpd);
}

/**********************************************************************/
/* Inform the client that the requested web method has not been
 * implemented.
 * Parameter: the client socket */
/**********************************************************************/
void unimplemented(int client)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.1 501 Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: image/jpeg\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}

char* getIPAddress(char* ip)
{
    struct ifaddrs* ifAddrStruct = NULL;
    void* tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    while(ifAddrStruct != NULL)
    {
        if(ifAddrStruct->ifa_addr->sa_family == AF_INET)
        { 
            // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr = &((struct sockaddr_in*)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if(strcmp(addressBuffer, "127.0.0.1"))
            {
                memcpy(ip, addressBuffer, strlen(addressBuffer));
                ip[strlen(addressBuffer)] = '\0';
                return ip;
            }
            //printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 
        }
        // else
        // {
            // if(ifAddrStruct->ifa_addr->sa_family == AF_INET6)
            // { 
                // // check it is IP6
                // // is a valid IP6 Address
                // tmpAddrPtr = &((struct sockaddr_in*)ifAddrStruct->ifa_addr)->sin_addr;
                // char addressBuffer[INET6_ADDRSTRLEN];
                // inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
                // printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 
            // }   
        // }

        ifAddrStruct=ifAddrStruct->ifa_next;
    }
    return "";
}
/**********************************************************************/

int main(void)
{
    int server_sock = -1;
    u_short port    = 8080;
    int client_sock = -1;
    struct sockaddr_in client_name;
    unsigned int client_name_len = sizeof(client_name);
    pthread_t newthread;
    char ip[INET_ADDRSTRLEN];
    
    server_sock = startup(&port);
    printf("httpd running on %s:%d\n", getIPAddress(ip), port);

    while(1)
    {
        client_sock = accept(server_sock, (struct sockaddr *)&client_name, &client_name_len);
        if(client_sock == -1)
        {
            error_die("accept");  
        }

        /* accept_request(client_sock); */
        if(pthread_create(&newthread , NULL, accept_request, &client_sock) != 0)
        {
            perror("pthread_create");
        }

        pthread_detach(newthread);
    }

    close(server_sock);

    return(0);
}
