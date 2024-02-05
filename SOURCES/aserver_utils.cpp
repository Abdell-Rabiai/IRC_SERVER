# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"

std::string Server::getCurrentTime() {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::string year = std::to_string(1900 + ltm->tm_year);
	std::string month = std::to_string(1 + ltm->tm_mon);
	std::string day = std::to_string(ltm->tm_mday);
	std::string hour = std::to_string(ltm->tm_hour);
	std::string min = std::to_string(ltm->tm_min);
	std::string sec = std::to_string(ltm->tm_sec);
	std::string currentTime = year + "-" + month + "-" + day + " " + hour + ":" + min + ":" + sec;
	return currentTime;
}

std::string Server::getServerIp()
{
	char IP[256];
	if (gethostname(IP, sizeof(IP)) != 0) {
	   std::cerr << "Error in gethostname" << std::endl;
	   exit(EXIT_FAILURE);
	}
	return std::string(IP);
}

std::vector<std::string>  split(std::string str, std::string delimiter) {
	std::vector<std::string> result;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		result.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	result.push_back(str);
	return result;
}

void Server::stopServer() {
	// close the server socket
	close(this->serverSocket.getFd());
	// close all client sockets
	for (size_t i = 0; i < this->clientSocketsfds.size(); i++) {
		close(this->clientSocketsfds[i]);
	}
}

void Server::broadcastMessageOnServer(std::string &buffer, int senderSocketfd) {
	std::string msg = "<"+ this->fdToClient[senderSocketfd].getHostName() + "> [" 
						+ this->fdToClient[senderSocketfd].getPort() + "]: "
						+ std::string(buffer, 0, buffer.length()) + "\r\n";
	std::string thankYouMsg = "Sent\r\n";
	send(senderSocketfd, thankYouMsg.c_str(), thankYouMsg.size() + 1, 0);
	for (size_t i = 1; i < this->pollfds.size(); i++) {
		if (this->pollfds[i].fd != senderSocketfd && this->pollfds[i].fd >= 0) {
			sendMessageToClient(msg, this->fdToClient[this->pollfds[i].fd]);
		}
	}
}

void Server::printAllClients(std::string data) {
	std::istringstream str(data);
	std::string token;
	str >> token;
	if (token != "PRINT")
		return;
	std::cout << "All clients: " << std::endl;
	for (size_t i = 0; i < this->clients.size(); i++) {
		std::cout << "Client number " << i << "------------------------------------------"  << std::endl;
		this->clients[i].printClientInfo();
	}
}

void Server::sendConfimationMessage(Client &client) {
	std::string response;
	response =  ":" + this->getServerName() + " 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName() + "\r\n";
	response += ":" + this->getServerName() + " 002 " + client.getNickname() + " :Your host is " + this->serverName + ", running version 5.3\r\n";
	response += ":" + this->getServerName() + " 003 " + client.getNickname() + " :This server was created " + this->serverCreationTime + "\r\n";
	response += ":" + this->getServerName() + " 004 " + client.getNickname() + " :" + this->serverName + " 5.3 o o\r\n";
	response += ":" + this->getServerName() + " 005 " + client.getNickname() + " are supported by this server\r\n";
	response += "This server is running on port " + std::to_string(this->serverPort) + "\r\n";
	response += "Created by Tabi3a && Blackhole\r\n";
	this->sendMessageToClient(response, client);
	client.setIsUserRegistered(true);
}