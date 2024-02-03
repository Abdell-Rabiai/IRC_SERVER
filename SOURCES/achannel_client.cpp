# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"

void Server::addClient(Client client, int socketfd) {
	this->clients.push_back(client);
	this->clientSocketsfds.push_back(socketfd);
	this->fdToClient.insert(std::pair<int, Client>(socketfd, client));
}

void Server::removeChannel(std::string channelName) {
	// erase the channel from the channels vector
	for (size_t i = 0; i < this->channels.size(); i++) {
		if (this->channels[i].getName() == channelName) {
			this->channels.erase(this->channels.begin() + i);
			break;
		}
	}
	// erase the channel from the nameToChannel map
	this->nameToChannel.erase(channelName);
}

void Server::removeClient(Client client) {
	for (size_t i = 0; i < this->clients.size(); i++) {
		if (this->clients[i].getSocketfd() == client.getSocketfd()) {
			this->clients.erase(this->clients.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < this->clientSocketsfds.size(); i++) {
		if (this->clientSocketsfds[i] == client.getSocketfd()) {
			this->clientSocketsfds.erase(this->clientSocketsfds.begin() + i);
			break;
		}
	}
	this->fdToClient.erase(client.getSocketfd());
}

bool Server::isChannelExist(std::string channelName) {
	if (this->nameToChannel.find(channelName) != this->nameToChannel.end())
		return true;
	return false;
}

Channel Server::getChannelByName(std::string channelName) {
	if (this->isChannelExist(channelName))
		return this->nameToChannel[channelName];
	return Channel();
}

Channel &Server::getChannelRefByName(std::string channelName) {
	if (this->isChannelExist(channelName))
		return this->nameToChannel[channelName];
	return this->nameToChannel[""];
}

bool Server::isClientInChannel(Client &client, std::string channelName) {
	if (this->isChannelExist(channelName)) {
		return this->nameToChannel[channelName].isUserInChannel(client);
	}
	return false;
}

bool Server::isChannelhasTopic(std::string channelName) {
	if (this->isChannelExist(channelName)) {
		return this->nameToChannel[channelName].getTopic() != "";
	}
	return false;
}

bool Server::isChannelHasPassword(std::string channelName) {
	if (this->isChannelExist(channelName)) {
		return this->nameToChannel[channelName].getKey() != "";
	}
	return false;
}

bool Server::isChannelPasswordCorrect(std::string channelName, std::string password) {
	if (this->isChannelExist(channelName)) {
		return this->nameToChannel[channelName].getKey() == password;
	}
	return false;
}