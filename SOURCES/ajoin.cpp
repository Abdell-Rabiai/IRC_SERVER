# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"

void Server::broadcastMessageOnChannel(Channel channel, std::string message) {
	if (this->isChannelExist(channel.getName())) {
		for (size_t i = 0; i < channel.getUsers().size(); i++) {
			if (channel.getUsers()[i].getSocketfd() >= 0) {
				this->sendMessageToClient(message, channel.getUsers()[i]);
			}
		}
	}
}

void Server::JoinResponse(Client &client, std::string channelName) {
	std::string msg = "";
	std::string broadcastMessage = ":" + client.getNickname() + "!" + client.getUsername() + "@" + this->getServerHostName() + " JOIN " + channelName + "\r\n";
	this->broadcastMessageOnChannel(this->nameToChannel[channelName], broadcastMessage);

	// 2. Channel topic if available
	if (this->isChannelhasTopic(channelName)) {
		std::string topic;
		std::string topicMessage;
		std::string timeSetTopicByWhoMessage;
		topic = this->nameToChannel[channelName].getTopic();
		topicMessage = "332 " + client.getNickname() + " " + channelName + " :" + topic + "\r\n";
		this->sendMessageToClient(topicMessage, client);
		timeSetTopicByWhoMessage = "333 " + client.getNickname() + " " + channelName + " " + this->nameToChannel[channelName].getCreatorNickname() + " " + this->nameToChannel[channelName].getCreationTime() + "\r\n";
		this->sendMessageToClient(timeSetTopicByWhoMessage, client);
	}
	// 3. list of users in the channel
	std::string ListMessage;
	std::vector<Client> users = this->nameToChannel[channelName].getUsers();
	std::string joinedUsers = "";
	for (size_t i = 0; i < users.size(); i++) {
		joinedUsers += users[i].getNickname() + " ";
	}
	ListMessage = "353 " + client.getNickname() + " = " + channelName + " :" + joinedUsers + "\r\n";
	this->sendMessageToClient(ListMessage, client);
	// 4. end of list
	std::string endOfListMessage = "366 " + channelName + " :End of NAMES list\r\n";
	this->sendMessageToClient(endOfListMessage, client);
}

void Server::createChannel(std::string name, std::string password, Client &creator) {
	Channel channel(name);
	if (password != "")
		channel.setKey(password);
	channel.setCreatorNickname(creator.getNickname());
	channel.setCreationTime(Server::getCurrentTime());
	channel.addUser(creator);
	channel.addOperator(creator);
	creator.setIsOperator(true);
	this->channels.push_back(channel);
	this->nameToChannel.insert(std::pair<std::string, Channel>(name, channel));
	std::string msg = "330 " + creator.getNickname() + "!" + creator.getUsername() + "@" + this->serverHostName + " : Your have created the channel " + name + " as an Operator\r\n";
	this->sendMessageToClient(msg, creator);
}

void Server::join_channel(std::string channelName, std::string key, Client &creator) {
	std::string msg = "";
	// if the channel doesn't exist create it
	if (this->nameToChannel.find(channelName) == this->nameToChannel.end()) {
		if (!key.empty())
			this->createChannel(channelName, key, creator);
		else
			this->createChannel(channelName, "", creator);
	}
	// if the channel exists
	else {
		// check if the channel is invite only
		Channel channel = this->nameToChannel[channelName];
		if (channel.getIsInviteOnly()) {
			// check if the client is invited
			msg = "473 " + creator.getNickname() + " " + channelName + " :Cannot join channel (+i) - You are not invited\r\n";
			this->sendMessageToClient(msg, creator);
			return ;
		}
		// check if the channel is full (limit)
		size_t size = channel.getUsers().size();
		if (size >= channel.getLimit()) {
			msg = "471 " + creator.getNickname() + " " + channelName + " :Cannot join channel (+l) - Channel is full\r\n";
			this->sendMessageToClient(msg, creator);
			return ;
		}
		// check if the client is already in the channel
		if (this->nameToChannel[channelName].isUserInChannel(creator)) {
			// msg = "443 " + creator.getNickname() + " " + channelName + " :is already on channel\r\n"; 
			msg = "443 " + creator.getNickname() + "!" + creator.getUsername() + "@" + this->serverHostName + " " + channelName + " :is already on channel\r\n";
			this->sendMessageToClient(msg, creator);
			return ;
		}
		// check if the channel has a password
		if (this->nameToChannel[channelName].getKey() == key || this->nameToChannel[channelName].getKey() == "") {
			this->nameToChannel[channelName].addUser(creator);
			msg = "121 " + creator.getNickname() + "!" + creator.getUsername() + "@" + this->serverHostName + " You have joined the channel " + channelName + " as a Regular User\r\n";
			this->sendMessageToClient(msg, creator);
		}
		else {
			msg = "475 " + channelName + " Cannot join channel (+k) - bad key\r\n";
			this->sendMessageToClient(msg, creator);
			return ;
		}
	}
	this->JoinResponse(creator, channelName);
}

void Server::handleJoinCommand(Client &client) {
	// JOIN #channel1,#channel2,#channel3 key1,key2,key3
	std::string data = client.getParameters().size() >= 1 ? this->fdToClient[client.getSocketfd()].getParameters()[0] : "";
	std::string data1 = client.getParameters().size() >= 2 ? this->fdToClient[client.getSocketfd()].getParameters()[1] : "";
	std::vector<std::string> channels = split(data, ",");
	std::vector<std::string> keys = split(data1, ",");
	for (size_t i = 0; i < channels.size(); i++) {
		if (i < keys.size()) {
			this->join_channel(channels[i], keys[i], client);
		}
		else {
			this->join_channel(channels[i], "", client);
		}
	}
}

