# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include <string.h> // strjoin

Server::Server() {
    this->serverPort = 6667;
    this->password = "pass";
    this->serverSocketfd = serverSocket.getFd();
}

Server::Server(int port, std::string password) {
    this->serverPort = port;
    this->password = password;
    this->serverSocketfd = serverSocket.getFd();
}

Server::Server(int port, std::string password, Socket serverSocket) {
    this->serverPort = port;
    this->password = password;
    this->serverSocket = serverSocket;
    this->serverSocketfd = serverSocket.getFd();
}

void Server::addClient(Client client, int socketfd) {
    this->clients.push_back(client);
    this->clientSocketsfds.push_back(socketfd);
}

void Server::stopServer() {
    // close the server socket
    close(this->serverSocket.getFd());
    // close all client sockets
    for (Client client : this->clients) {
        close(client.getSocketfd());
    }
}

int  Server::acceptNewConnection() {
    // char host[NI_MAXHOST]; // this is used to get the hostname of the client
    // char srvce[NI_MAXSERV]; // this is used to get the port number of the client
    int clientSocketfd = this->serverSocket.acceptSocket();
    Client client(clientSocketfd);
    this->addClient(client, clientSocketfd);
    this->fdToClient.insert(std::pair<int, Client>(clientSocketfd, client));
    // send a welcome message to the client
    std::string clientSocketfdStr = std::to_string(clientSocketfd);
    std::string welcome_msg = "Welcome to the Server number " + clientSocketfdStr + " \n";
    send(clientSocketfd, welcome_msg.c_str(), welcome_msg.length(), 0);
    return (clientSocketfd);
}

void Server::acceptNewMessage(int socketfd) {
    char buffer[4096];
    while(true) {
        memset(buffer, 0, 4096);
        int bytesReceived = recv(socketfd, buffer, 4096, 0);
        if (bytesReceived == -1) {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (bytesReceived == 0) {
            std::cout << "Client disconnected " << std::endl;
            close(socketfd);
            return;
        }
        if (buffer[0] == '\n') {
            continue;
        }
        // display message
        std::cout << "Received: " << std::string(buffer, 0, bytesReceived - 1) << " from " << socketfd << std::endl;
        // echo message back to the client
        send(socketfd, buffer, bytesReceived + 1, 0);
        // echo a thank you message back to the client
        std::string thankYouMsg = "Thank you !! Your message has been received\n";
        send(socketfd, thankYouMsg.c_str(), thankYouMsg.size() + 1, 0);
    }
}

void Server::handleEvents() {
    //this is only for testing
    // it handles one client at a time
    while(true) {
    int clientSocketfd;
    // there are 2 types of events that we need to handle
    // 1. a client wants to connect to the server
    clientSocketfd = this->acceptNewConnection();
    // 2. a client wants to send a message to the server
    this->acceptNewMessage(clientSocketfd);
    // 3. now based in that message we need to do something
    // 3.1. if the message is a command then we need to execute it
    // 3.2. if the message is a normal message then we need to broadcast it to all the clients
    // 3.3. if the message is a private message then we need to send it to the client
    // 3.4. if the message is a channel message then we need to broadcast it to all the clients in that channel
    // 3.5. if the message is a channel private message then we need to send it to all the clients in that channel
    close(clientSocketfd);
    }
}


void Server::startServer() {
    // create a server socket
    Socket serverSocket(AF_INET, SOCK_STREAM, 0, this->serverPort, INADDR_ANY);

    this->serverSocket = serverSocket;
    // bind the server socket to the server address
    this->serverSocket.bindSocket();
    // start listening for connections
    this->serverSocket.listenSocket(this->MAX_CLIENTS);
    std::cout << "Server started, Listening on port " << this->serverPort << std::endl;
    this->handleEvents();
    // close the server socket
}


Server::~Server()
{
    this->stopServer();
}

    // // create a new client socket this means accept a new connection
    // int clientSocketfd = this->serverSocket.acceptSocket();
    // // create a new client object
    // Client client(clientSocketfd);
    // this->addClient(client, clientSocketfd);
    // this->fdToClient.insert(std::pair<int, Client>(clientSocketfd, client));
    // // send a welcome message to the client
    // std::string clientSocketfdStr = std::to_string(clientSocketfd);
    // std::string welcome_msg = "Welcome to the Server number " + clientSocketfdStr;

    // send(clientSocketfd, welcome_msg.c_str(), welcome_msg.length(), 0);
    // }

    // this is used to handle the events on the server 
    // while(true) {
    //     // create a set of socket descriptors
    //     fd_set readfds;
    //     // clear the socket set
    //     FD_ZERO(&readfds);
    //     // add server socket to set
    //     FD_SET(this->serverSocket.getFd(), &readfds);
    //     // add client sockets to set
    //     for (int socketfd : this->clientSocketsfds) {
    //         FD_SET(socketfd, &readfds);
    //     }
    //     // wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely
    //     int activity = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);
    //     if ((activity < 0) && (errno!=EINTR)) {
    //         std::cerr << "Error selecting" << std::endl;
    //     }
    //     // if something happened on the server socket, then it's an incoming connection
    //     if (FD_ISSET(this->serverSocket.getFd(), &readfds)) {
    //         this->acceptNewConnection();
    //     }
    //     // else it's some IO operation on some other socket
    //     for (int socketfd : this->clientSocketsfds) {
    //         if (FD_ISSET(socketfd, &readfds)) {
    //             // check if it was for closing, and also read the incoming message
    //             this->processClientData(this->fdToClient[socketfd]);
    //         }
    //     }
    // }
//


/*
void Server::acceptNewMessage(int socketfd) {
    char buffer[4096];
    while(true) {
        memset(buffer, 0, 4096);
        int bytesReceived = recv(socketfd, buffer, 4096, 0);
        if (bytesReceived == -1) {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (bytesReceived == 0) {
            std::cout << "Client disconnected " << std::endl;
            close(socketfd);
            return;
        }
        if (buffer[0] == '\n') {
            continue;
        }
        // display message
        std::string message = std::string(buffer, 0, bytesReceived - 1);
        int i = message.find_first_of(' ');
        std::string command = message.substr(0, i);
        std::string password = message.substr(i + 1, message.length() - i - 1);
        std::cout << "pasword: {" << password <<"}"<< std::endl;
        std::string msg = "You need to authenticate first\n";
        if (command != "PASS" || password != this->password) {
            send(socketfd, msg.c_str(), msg.length(), 0);
            continue;
        }
        msg = "You are authenticated successfully \nNow set a nickname using the command NICK <nickname>\n";
        send(socketfd, msg.c_str(), msg.length(), 0);

        std::cout << "Received: " << message << " from " << socketfd << std::endl;
        int bytesnickname = recv(socketfd, buffer, 4096, 0);
        if (bytesnickname == -1) {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (bytesnickname == 0) {
            std::cout << "Client disconnected " << std::endl;
            close(socketfd);
            return;
        }
        if (buffer[0] == '\n') {
            continue;
        }

        std::string n = "your nickname is: " + std::string(buffer, 0, bytesnickname - 1) + "\n";
        send(socketfd, n.c_str(), n.length(), 0);
        // echo a thank you message back to the client
        std::string thankYouMsg = "Thank you !! Your message has been received\n";
        send(socketfd, thankYouMsg.c_str(), thankYouMsg.size() + 1, 0);
    }
}
*/