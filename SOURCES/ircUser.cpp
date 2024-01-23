# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/client.hpp"

IrcUser::IrcUser() {
	this->socketfd = -1;
	this->nickname = "";
	this->username = "";
	this->isOperator = false;
	this->isAuthenticated = false;
}

IrcUser::IrcUser(int socketfd) {
	this->socketfd = socketfd;
	this->nickname = "";
	this->username = "";
	this->isOperator = false;
	this->isAuthenticated = false;
}

IrcUser::~IrcUser() {

}

// getters 
int IrcUser::getSocketfd() {
	return this->socketfd;
}

std::string IrcUser::getNickname() {
	return this->nickname;
}

std::string IrcUser::getUsername() {
	return this->username;
}

bool IrcUser::getIsOperator() {
	return this->isOperator;
}

bool IrcUser::getIsAuthenticated() {
	return this->isAuthenticated;
}

std::vector<Channel> IrcUser::getCurrentChannels() {
	return this->currentChannels;
}

std::map<std::string, Channel> IrcUser::getNameToChannel() {
	return this->nameToChannel;
}

// setters

void IrcUser::setSocketfd(int socketfd) {
	this->socketfd = socketfd;
}

void IrcUser::setNickname(std::string nickname) {
	this->nickname = nickname;
}

void IrcUser::setUsername(std::string username) {
	this->username = username;
}

void IrcUser::setIsOperator(bool isOperator) {
	this->isOperator = isOperator;
}

void IrcUser::setIsAuthenticated(bool isAuthenticated) {
	this->isAuthenticated = isAuthenticated;
}

void IrcUser::setCurrentChannels(std::vector<Channel> currentChannels) {
	this->currentChannels = currentChannels;
}

void IrcUser::setNameToChannel(std::map<std::string, Channel> nameToChannel) {
	this->nameToChannel = nameToChannel;
}

// methods, we'l add them later
