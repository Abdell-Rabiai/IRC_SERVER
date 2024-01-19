# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <netdb.h>

# define MAXLINE 4096 // size of the buffer that is used to read the data back from the socket 
# define SERVER_PORT 80 // default port for http that server is listening on
# define LISTENQ 1024 // max number of connections that can be queued up at once

# define SA struct sockaddr // struct sockaddr is used to store socket address information 

void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}

int main(int ac, char **av)
{
    int sockfd, readbytes; // sockfd is the socket descriptor, readbytes is the number of bytes read from the socket
    int sendbytes; // number of bytes sent to the socket
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];
    char recvline[MAXLINE];

    if (ac != 2)
        err_sys("usage: miniClient <IPaddress>");
    // av[0] ==> miniClient the name of the program
    // av[1] ==> <IPaddress> the IP address of the server we want to connect to and grap data from
    // here i'm gonna create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        err_sys("socket error");
    // so a socket is basically a file descriptor (Endpoint) that is used to read and write data to and from the socket
    // AF_INET is the address family that is used for IPv4 addresses (AF_INET6 is used for IPv6 addresses)
    // SOCK_STREAM is the type of socket that is used for TCP connections (SOCK_DGRAM is used for UDP connections)
    // SOCK_STREAM is used to create a connection and send a stream of data to the server a get a stream of data back
    // in the other hand SOCK_DGRAM is used to send a packet of data to the server and get a packet of data back

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_port = htons(SERVER_PORT); // port number of the server we want to connect to
    // htons is used to convert the port number from host byte order to network byte order
    // so basically it's used to convert the port number from little endian to big endian and othewise
    // so many machines can connect to the server and send data to it without worrying about the endianness of the machine

    if (inet_pton(AF_INET, av[1], &servaddr.sin_addr) <= 0)
        err_sys("inet_pton error ");
    // inet_pton is used to convert the IP address from text to binary form
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) // connect to the server using the socket descriptor
        err_sys("connect error");

    // now we are connected to the server and we can send data to it and get data back from it
    // so we are gonna send a GET request to the server and get the data back from it
    sprintf(sendline, "GET / HTTP/1.1\r\n\r\n"); // GET request
    sendbytes = strlen(sendline);
    // sprintf is used to write formatted data to a string (sendline)
    // GET / HTTP/1.1\r\n\r\n is the GET request that we are gonna send to the server
    // i'm sending a GET request and the slash (/) means that i want to get the index.html the root home page of the server
    // HTTP/1.1 is the version of the HTTP protocol that we are using

    if (write(sockfd, sendline, sendbytes) != sendbytes) // send the GET request to the server
        err_sys("write error");
    
    memset(recvline, 0, MAXLINE); // clear the buffer
    while ((readbytes = read(sockfd, recvline, MAXLINE - 1)) > 0) // read the data back from the server
    { 
        recvline[readbytes] = 0; // null terminate the string
        if (fputs(recvline, stdout) == EOF) // print the data to the screen
            err_sys("fputs error");
        memset(recvline, 0, MAXLINE); // clear the buffer
    }
    
}