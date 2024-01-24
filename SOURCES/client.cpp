# include "../HEADERS/client.hpp"

Client::Client() {
	this->socketfd = -1;
	this->nickname = "";
	this->username = "";
	this->isOperator = false;
	this->isAuthenticated = false;
}

Client::Client(int socketfd) {
    this->socketfd = socketfd;
    this->nickname = "";
    this->username = "";
    this->isOperator = false;
    this->isAuthenticated = false;
}

Client::Client(int socketfd, std::string nickname, std::string username) {
    this->socketfd = socketfd;
    this->nickname = nickname;
    this->username = username;
    this->isOperator = false;
    this->isAuthenticated = false;
}

Client::~Client() {

}

// methods
bool Client::authenticate(std::string _password, std::string password, std::string data) {
		int i = data.find_first_of(' ');
		std::string command = data.substr(0, i);
		// std::string password = data.substr(i + 1, data.length() - i - 1);
		std::string msg = "You need to authenticate first\n";
		if (command != "PASS"){
            send(this->getSocketfd(), msg.c_str(), msg.length(), 0);
            return false;
        }
        else if (password != _password) {
            msg = "Wrong password\n";
            send(this->getSocketfd(), msg.c_str(), msg.length(), 0);
            return false;
        }
		msg = "You are authenticated successfully\n";
		send(this->getSocketfd(), msg.c_str(), msg.length(), 0);
		this->setIsAuthenticated(true);
        return true;
}

// getters 
int Client::getSocketfd() {
	return this->socketfd;
}

std::string Client::getNickname() {
	return this->nickname;
}

std::string Client::getUsername() {
	return this->username;
}

bool Client::getIsOperator() {
	return this->isOperator;
}

bool Client::getIsAuthenticated() {
	return this->isAuthenticated;
}

std::vector<Channel> Client::getCurrentChannels() {
	return this->currentChannels;
}

std::map<std::string, Channel> Client::getNameToChannel() {
	return this->nameToChannel;
}

std::string Client::getHostName() {
    return this->hostname;
}

std::string Client::getPort() {
    return this->port;
}

// setters

void Client::setSocketfd(int socketfd) {
	this->socketfd = socketfd;
}

void Client::setNickname(std::string nickname) {
	this->nickname = nickname;
}

void Client::setUsername(std::string username) {
	this->username = username;
}

void Client::setIsOperator(bool isOperator) {
	this->isOperator = isOperator;
}

void Client::setIsAuthenticated(bool isAuthenticated) {
	this->isAuthenticated = isAuthenticated;
}

void Client::setCurrentChannels(std::vector<Channel> currentChannels) {
	this->currentChannels = currentChannels;
}

void Client::setNameToChannel(std::map<std::string, Channel> nameToChannel) {
	this->nameToChannel = nameToChannel;
}

void Client::setHostName(std::string hostname) {
    this->hostname = hostname;
}

void Client::setPort(std::string port) {
    this->port = port;
}

// methods, we'l add them later
