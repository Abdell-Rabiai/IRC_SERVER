# pragma once
# include <string>
# include <vector>
# include <map>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>

class Socket
{
    private :
        int _domain; // AF_INET
        int _type; // SOCK_STREAM
        int _protocol; // IPPROTO_TCP for TCP/IP or IPPROTO_UDP for UDP
        int fd;
        struct sockaddr_in address;

    public :
        Socket();
        Socket(int domain, int type, int protocol, int port, unsigned long ip);
        Socket(const Socket &socket);
        Socket &operator=(const Socket &socket);
        ~Socket();

       // methods
        void bindSocket();
        void listenSocket(int maxConnections);
        int acceptSocket();
        void connectSocket();
        void closeSocket();

        // getters
        int getDomain();
        int getType();
        int getProtocol();
        int getFd();
        const struct sockaddr_in &getAddress();

        // setters
        void setDomain(int domain);
        void setType(int type);
        void setProtocol(int protocol);
        void setFd(int fd);
        void setAddress(struct sockaddr_in address);
};