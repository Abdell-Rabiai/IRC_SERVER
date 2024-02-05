# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"

Client::Client() {
	this->socketfd = -1;
	this->nickname = "";
	this->username = "";
	this->realname = "";
	this->hostname = "";
	this->isOperator = false;
	this->isAuthenticated = false;
	this->port = "";
	this->command = "";
	this->parameters = std::vector<std::string>();
	this->trailing = "";
	this->isUserRegistered = false;
}

Client::Client(int socketfd) {
    this->socketfd = socketfd;
    this->nickname = "";
    this->username = "";
	this->realname = "";
	this->hostname = "";
    this->isOperator = false;
    this->isAuthenticated = false;
	this->port = "";
	this->command = "";
	this->parameters = std::vector<std::string>();
	this->trailing = "";
	this->isUserRegistered = false;
}

Client::Client(int socketfd, std::string nickname, std::string username) {
    this->socketfd = socketfd;
    this->nickname = nickname;
    this->username = username;
    this->isOperator = false;
    this->isAuthenticated = false;
	this->realname = "";
	this->hostname = "";
	this->port = "";
	this->command = "";
	this->parameters = std::vector<std::string>();
	this->trailing = "";
	this->isUserRegistered = false;
}

Client::Client(int socketfd, std::string nickname, std::string username, std::string realname) {
	this->socketfd = socketfd;
    this->nickname = nickname;
    this->username = username;
	this->realname = realname;
    this->isOperator = false;
    this->isAuthenticated = false;
	this->hostname = "";
	this->port = "";
	this->command = "";
	this->parameters = std::vector<std::string>();
	this->trailing = "";
	this->isUserRegistered = false;
}

Client::~Client() {

}

bool Client::operator==(const Client &client) const {
	return this->socketfd == client.socketfd;
}

void Client::parseIrcMessage(std::string buffer) {
	std::string command;
	std::vector<std::string> parameters;
	std::string trailing;

	// clear existing data
	this->command.clear();
	this->parameters.clear();
	this->trailing.clear();

	int i = buffer.find_first_of(' ');
	if (i != -1) {
		command = buffer.substr(0, i);
		buffer = buffer.substr(i + 1, buffer.length() - i - 1);
	}
	else {
		command = buffer;
		buffer = "";
	}

	i = buffer.find_first_of(':');
	if (i != -1) {
		trailing = buffer.substr(i + 1, buffer.length() - i - 1);
		buffer = buffer.substr(0, i - 1);
	}

	while (buffer.length() > 0) {
		int j = buffer.find_first_of(':');
		if (j != -1)
			break ;
		i = buffer.find_first_of(' ');
		if (i == -1) {
			parameters.push_back(buffer);
			buffer = "";
		}
		else {
			std::string str = buffer.substr(0, i);
			if (str.length() > 0){
				parameters.push_back(str);
			}
			buffer = buffer.substr(i + 1, buffer.length() - i - 1);
		}
	}
	if (trailing.length() > 0) {
		parameters.push_back(trailing);
	}
	this->command = command;
	this->parameters = parameters;
	this->trailing = trailing;
}

// methods
bool Client::authenticate(std::string ServerPassword) {
	std::string response;
	std::string password = this->parameters[0];
	if (password != ServerPassword) {
		response = "464 ERROR :Password incorrect.\n";
		send(this->getSocketfd(), response.c_str(), response.length(), 0);
		return false;
	}
	response = "Congrats!! You are authenticated successfully\nto Confirm tour registration enter: \nNICK <nickname>\nUSER <username>\n";
	send(this->getSocketfd(), response.c_str(), response.length(), 0);
	this->setIsAuthenticated(true);
	return true;
}

bool Client::confirmRegistration() {
	if (this->getIsAuthenticated() == true) {
		if (this->getNickname() != "" && this->getUsername() != "") {
			return true;
		}
	}
	return false;
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

std::string Client::getBuffer() {
	return this->buffer;
}

std::string Client::getCommand() {
	return this->command;
}

std::vector<std::string> Client::getParameters() {
	return this->parameters;
}

std::string Client::getTrailing() {
	return this->trailing;
}

bool Client::getIsUserRegistered() {
	return this->isUserRegistered;
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

void Client::setBuffer(std::string buffer) {
	this->buffer = buffer;
}

void Client::setCommand(std::string command) {
	this->command = command;
}

void Client::setParameters(std::vector<std::string> parameters) {
	this->parameters = parameters;
}

void Client::setTrailing(std::string trailing) {
	this->trailing = trailing;
}

void Client::setIsUserRegistered(bool isUserRegistered) {
	this->isUserRegistered = isUserRegistered;
}

// methods, we'l add them later
