# include "../HEADERS/client.hpp"

Client::Client(int socketfd) : IrcUser() {
    this->socketfd = socketfd;
    this->isOperator = false;
    this->isAuthenticated = false;
}


Client::~Client() {
    
}

