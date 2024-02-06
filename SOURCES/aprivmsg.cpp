# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"

void Server::sendMessageToClient(std::string message , Client client) {
	int bytessent = send(client.getSocketfd(), message.c_str(), message.size(), 0);
	if (bytessent == -1) {
		std::cerr << "Error: in send() << " << strerror(errno) << std::endl;
		return;
	}
}

Client Server::getClientByNickname(std::string nickname) {
	for (size_t i = 0; i < this->clients.size(); i++) {
		if (this->clients[i].getNickname() == nickname) {
			return this->clients[i];
		}
	}
	return Client();
}

void Server::sendPrivateMessageToClient(Client &client, std::string recipientNickname, std::string message) {

	if (this->getClientByNickname(recipientNickname).getSocketfd() != -1) {
		std::string msg;
		msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + this->serverHostName + " PRIVMSG " + recipientNickname + " :" + message + "\r\n";
		this->sendMessageToClient(msg, this->getClientByNickname(recipientNickname));
	}
	else {
		std::string msg = "401" + recipientNickname + " No such user with this nickname\r\n";
		this->sendMessageToClient(msg, client);
	}
}

void Server::broadcastMessageOnChannel(Channel channel, std::string message, Client sender) {
	// except the sender
	if (this->isChannelExist(channel.getName())) {
		for (size_t i = 0; i < channel.getUsers().size(); i++) {
			if (channel.getUsers()[i].getSocketfd() >= 0) {
				if (channel.getUsers()[i].getSocketfd() != sender.getSocketfd())
					this->sendMessageToClient(message, channel.getUsers()[i]);
			}
		}
	}
}

void Server::sendPrivateMessageToChannel(Client &client, std::string channelName, std::string message) {
	if (this->nameToChannel.find(channelName) != this->nameToChannel.end()) {
		if (this->nameToChannel[channelName].isUserInChannel(client)) {
			std::string msgToLimChat = ":" + client.getNickname() + "!" + client.getUsername() + "@" + this->serverHostName + " PRIVMSG " + channelName + " :" + message + "\r\n";
			this->broadcastMessageOnChannel(this->nameToChannel[channelName], msgToLimChat, client);//HERE
		}
		else {
			std::string msg = "404 " + channelName + " Cannot send to channel\r\n";
			this->sendMessageToClient(msg, client);
		}
	}
	else {
		std::string msg = "403 " + channelName + " No such channel\r\n";
		this->sendMessageToClient(msg, client);
	}
}

void Server::handlePrivateMessageCommand(Client &client) {
	std::string receivers = this->fdToClient[client.getSocketfd()].getParameters()[0];
	std::string message = this->fdToClient[client.getSocketfd()].getParameters()[1];
	std::vector<std::string> recipients = split(receivers, ",");
	for (size_t i = 0; i < recipients.size(); i++) {
		if (recipients[i][0] != '#') {
			this->sendPrivateMessageToClient(client, recipients[i], message);
		}
		else
			this->sendPrivateMessageToChannel(client, recipients[i], message);
	}
}