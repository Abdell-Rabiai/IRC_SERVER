# include "../HEADERS/client.hpp"

Client::Client() {
	this->socketfd = -1;
	this->nickname = "";
	this->username = "";
	this->realname = "";
	this->hostname = "";
	this->isOperator = false;
	this->isAuthenticated = false;
}

Client::Client(int socketfd) {
    this->socketfd = socketfd;
    this->nickname = "";
    this->username = "";
	this->realname = "";
	this->hostname = "";
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

Client::Client(int socketfd, std::string nickname, std::string username, std::string realname) {
	this->socketfd = socketfd;
    this->nickname = nickname;
    this->username = username;
	this->realname = realname;
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
            send(this->getSocketfd(), msg.c_str(), msg.length(), 0);
            return false;
        }
		msg = "You are authenticated successfully\n";
		send(this->getSocketfd(), msg.c_str(), msg.length(), 0);
		this->setIsAuthenticated(true);
        return true;
}

void Client::becomeOperator() {
	this->isOperator = true;
}

void Client::printClientInfo() {
	std::cout << "Client info: " << std::endl;
	std::cout << "socketfd: [" << this->socketfd << "]" << std::endl;
	std::cout << "nickname: [" << this->nickname << "]" << std::endl;
	std::cout << "username: [" << this->username << "]" << std::endl;
	std::cout << "realname: [" << this->realname << "]" << std::endl;
	std::cout << "hostname: [" << this->hostname << "]" << std::endl;
	std::cout << "port: [" << this->port << "]" << std::endl;
	std::cout << "isOperator: [" << (this->isOperator ? "YES" : "NO") << "]" << std::endl;
	std::cout << "isAuthenticated: [" << (this->isAuthenticated ? "YES" : "NO") << "]" << std::endl;
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

std::string Client::getRealname() {
	return this->realname;
}

bool Client::getIsOperator() {
	return this->isOperator;
}

bool Client::getIsAuthenticated() {
	return this->isAuthenticated;
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

void Client::setRealname(std::string realname) {
	this->realname = realname;
}

void Client::setIsOperator(bool isOperator) {
	this->isOperator = isOperator;
}

void Client::setIsAuthenticated(bool isAuthenticated) {
	this->isAuthenticated = isAuthenticated;
}

void Client::setHostName(std::string hostname) {
    this->hostname = hostname;
}

void Client::setPort(std::string port) {
    this->port = port;
}

// methods, we'l add them later
