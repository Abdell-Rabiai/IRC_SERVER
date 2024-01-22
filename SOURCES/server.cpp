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

void Server::handleEvents() {
    while(true) {
    sockaddr_in clAd;  // clientaddress
    socklen_t clAdSize = sizeof(clAd);

    // event dial server ---> new connection
    // event dial client ---> new message || client disconnected

    // create a new client socket
    int clientSocketfd = accept(this->serverSocketfd, (struct sockaddr *)&clAd, &clAdSize);
    if (clientSocketfd < 0)
    {
        std::cerr << "Error accepting new connection" << std::endl;
        return ;
    }
    // create a new client object
    Client client(clientSocketfd);
    // add the client to the clients vector
    this->addClient(client, clientSocketfd);
    // add the client to the client map
    this->fdToClient.insert(std::pair<int, Client>(clientSocketfd, client));
    // TODO
    // send a welcome message to the client
    std::string clientSocketfdStr = std::to_string(clientSocketfd);
    std::string welcome_msg = "Welcome to the Server " + clientSocketfdStr;

    send(clientSocketfd, welcome_msg.c_str(), welcome_msg.length(), 0);
    }
}

bool Server::startServer() {
    // create a server socket
    this->serverSocket = Socket(AF_INET, SOCK_STREAM, 0, this->serverPort, INADDR_ANY);
    // bind the server socket to the server address
    this->serverSocket.bindSocket(this->serverPort);
    // start listening for connections
    this->serverSocket.listenSocket(this->MAX_CLIENTS);
    std::cout << "Server started, Listening on port " << this->serverPort << std::endl;
    this->handleEvents();
}

Server::~Server()
{
    this->stopServer();
}

