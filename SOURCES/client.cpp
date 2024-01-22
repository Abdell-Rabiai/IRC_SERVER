# include "../HEADERS/client.hpp"

Client::Client(int socketfd) : IrcUser() {
    this->socketfd = socketfd;
    this->isOperator = false;
    this->isAuthenticated = false;
}

int Client::getSocketfd() {
    return this->socketfd;
}

Client::~Client() {
    
}

std::string Client::getNickname() {
    return this->nickname;
}