# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"

Server::Server() {
	this->serverPort = 6667;
	this->password = "pass";
	this->serverSocketfd = serverSocket.getFd();
}

bool Server::isOperatorInChannel(Client client, Channel channel) {
	for (int i = 0; i < channel.getOperators().size(); i++) {
		if (channel.getOperators()[i].getNickname() == client.getNickname())
			return true;
	}
	return false;
}

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

Server::Server(int port, std::string password) {
	this->serverPort = port;
	this->password = password;
	this->serverName = ":IRC_SERVER";
	this->serverCreationTime = Server::getCurrentTime();
	this->serverSocketfd = serverSocket.getFd();
	this->serverHostName = getServerIp();
}

Server::Server(int port, std::string password, Socket serverSocket) {
	this->serverPort = port;
	this->password = password;
	this->serverSocket = serverSocket;
	this->serverSocketfd = serverSocket.getFd();
}

// getters

std::string Server::getServerName() {
	return this->serverName;
}

std::string Server::getServerCreationTime() {
	return this->serverCreationTime;
}

std::string Server::getServerHostName() {
	return this->serverHostName;
}

int Server::getServerPort() {
	return this->serverPort;
}

std::vector<Client> Server::getClients() {
	return this->clients;
}

std::vector<int> Server::getClientSocketsfds() {
	return this->clientSocketsfds;
}

std::vector<Channel> Server::getChannels() {
	return this->channels;
}

std::vector<Client> Server::getOperators() {
	return this->operators;
}

std::map<int, Client> Server::getFdToClient() {
	return this->fdToClient;
}

std::map<std::string, Channel> Server::getNameToChannel() {
	return this->nameToChannel;
}

std::string Server::getPassword() {
	return this->password;
}

// setters

void Server::setServerName(std::string serverName) {
	this->serverName = serverName;
}

void Server::setServerCreationTime(std::string serverCreationTime) {
	this->serverCreationTime = serverCreationTime;
}

void Server::setServerHostName(std::string serverHostName) {
	this->serverHostName = serverHostName;
}

void Server::setServerPort(int serverPort) {
	this->serverPort = serverPort;
}

void Server::setClients(std::vector<Client> clients) {
	this->clients = clients;
}

void Server::setClientSocketsfds(std::vector<int> clientSocketsfds) {
	this->clientSocketsfds = clientSocketsfds;
}

void Server::setChannels(std::vector<Channel> channels) {
	this->channels = channels;
}

void Server::setOperators(std::vector<Client> operators) {
	this->operators = operators;
}

void Server::setFdToClient(std::map<int, Client> fdToClient) {
	this->fdToClient = fdToClient;
}

void Server::setNameToChannel(std::map<std::string, Channel> nameToChannel) {
	this->nameToChannel = nameToChannel;
}

void Server::setPassword(std::string password) {
	this->password = password;
}

void Server::setServerSocketfd(int serverSocketfd) {
	this->serverSocketfd = serverSocketfd;
}

// methods

void Server::addClient(Client client, int socketfd) {
	this->clients.push_back(client);
	this->clientSocketsfds.push_back(socketfd);
	this->fdToClient.insert(std::pair<int, Client>(socketfd, client));
}

void Server::removeClient(Client client) {
	for (int i = 0; i < this->clients.size(); i++) {
		if (this->clients[i].getSocketfd() == client.getSocketfd()) {
			this->clients.erase(this->clients.begin() + i);
			break;
		}
	}
	for (int i = 0; i < this->clientSocketsfds.size(); i++) {
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


void Server::stopServer() {
	// close the server socket
	close(this->serverSocket.getFd());
	// close all client sockets
	for (int i = 0; i < this->clientSocketsfds.size(); i++) {
		close(this->clientSocketsfds[i]);
	}
}

std::string printHostInfos(const struct sockaddr_in &address, Client *client) {
	char hostname[NI_MAXHOST];      // The remote host name
	char service[NI_MAXSERV];   // The port the remote host is connected to
	socklen_t addr_len = sizeof(address);
	int result = getnameinfo((struct sockaddr *)&address, addr_len, hostname, NI_MAXHOST, service, NI_MAXSERV, 0);
	if (result) {
		std::cout << "Error in getnameinfo: " << gai_strerror(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << hostname << " connected on port " << service << std::endl;
	client->setHostName(hostname);
	client->setPort(service);
	return std::string(hostname);
}

int  Server::acceptNewConnection() {
	int clientSocketfd = this->serverSocket.acceptSocket();
	Client client(clientSocketfd);
	std::string hostname = printHostInfos(this->serverSocket.getAddress(), &client); 
	client.setHostName(hostname);
	this->addClient(client, clientSocketfd);
	// send a welcome message to the client
	std::string clientSocketfdStr = std::to_string(clientSocketfd);
	std::string welcome_msg = "Welcome to the Server number " + clientSocketfdStr + " \r\n";
	send(clientSocketfd, welcome_msg.c_str(), welcome_msg.length(), 0);
	return (clientSocketfd);
}


void Server::removeDisconnectedClient(int &socketfd) {
	// Remove the disconnected client from pollfds
	std::cout << "Client disconnected " << std::endl;
	close(socketfd);
	for (int i = 0; i < this->pollfds.size(); i++) {
		if (this->pollfds[i].fd == socketfd) {
			this->pollfds.erase(this->pollfds.begin() + i);
			break ;
		}
	}
	this->removeClient(this->fdToClient[socketfd]);
}

void Server::sendMessageToClient(std::string message , Client client) {
	send(client.getSocketfd(), message.c_str(), message.length(), 0);
}

void Server::broadcastMessageOnServer(std::string &buffer, int senderSocketfd) {
	std::string msg = "<"+ this->fdToClient[senderSocketfd].getHostName() + "> [" 
						+ this->fdToClient[senderSocketfd].getPort() + "]: "
						+ std::string(buffer, 0, buffer.length()) + "\r\n";
	std::string thankYouMsg = "Sent\r\n";
	send(senderSocketfd, thankYouMsg.c_str(), thankYouMsg.size() + 1, 0);
	for (int i = 1; i < this->pollfds.size(); i++) {
		if (this->pollfds[i].fd != senderSocketfd && this->pollfds[i].fd >= 0) {
			sendMessageToClient(msg, this->fdToClient[this->pollfds[i].fd]);
		}
	}
}

void Server::broadcastMessageOnChannel(Channel channel, std::string message) {
	if (this->isChannelExist(channel.getName())) {
		for (int i = 0; i < channel.getUsers().size(); i++) {
			if (channel.getUsers()[i].getSocketfd() >= 0) {
				sendMessageToClient(message, channel.getUsers()[i]);
			}
		}
	}
}

void Server::broadcastMessageOnChannel(Channel channel, std::string message, Client sender) {
	// except the sender
	if (this->isChannelExist(channel.getName())) {
		for (int i = 0; i < channel.getUsers().size(); i++) {
			if (channel.getUsers()[i].getSocketfd() >= 0) {
				if (channel.getUsers()[i].getSocketfd() != sender.getSocketfd())
					sendMessageToClient(message, channel.getUsers()[i]);
			}
		}
	}
}


void Server::handleNickCommand(Client &client) {
	std::string nickname = this->fdToClient[client.getSocketfd()].getParameters()[0];
	if (this->getClientByNickname(nickname).getSocketfd() != -1) {
		std::string msg = "433 " + nickname + " :Nickname is already in use\r\n";
		this->sendMessageToClient(msg, client);
		close(client.getSocketfd());
	}
	if (nickname != "") {
		client.setNickname(nickname);
		std::string msg = "your nickname is set as: " + client.getNickname() + "\r\n";
		this->sendMessageToClient(msg, client);
	}
}

void Server::handleUserCommand(Client &client) {
	// USER username 0 * :ABDELOUAHED RABIAI
	std::string username = this->fdToClient[client.getSocketfd()].getParameters()[0];
	std::string realname = "";
	if (this->fdToClient[client.getSocketfd()].getParameters().size() >= 4) {
		realname = this->fdToClient[client.getSocketfd()].getParameters()[3];
	}
	if (username != "") {
		client.setUsername(username);
		std::string response = "your username is set as: [" + client.getUsername() + "]\r\n";
		this->sendMessageToClient(response, client);
	}
	if (realname != "") {
		client.setRealname(realname);
		std::string response = "your realname is set as: [" + client.getRealname() + "]\r\n";
		this->sendMessageToClient(response, client);
	}
}

void parseData(std::string &password, std::string &nickname, std::string &username, std::string &realname, std::string data) {
	std::istringstream str(data);
    // Assuming the data format is {PASS password\nNICK nickname\nUSER username 0 * :ABDELOUAHED RABIAI\n}
    std::string token;
    while (str >> token) {
        if (token == "PASS") {
            str >> password;
        } else if (token == "NICK") {
            str >> nickname;
        } else if (token == "USER") {
			str >> username;
			str >> token;
			str >> token;
			std::string buf;
			while (str >> token) {
				buf += token + " ";
			}
			if (buf.length() >= 2)
				realname = buf.substr(1, buf.length() - 2);
			else
				realname = "unknown";
        }
    }
}

// toify users ==> std::string notifyusers = ":" + this->_joinedUsers.find(fd)->second.getNICKNAME() + "!" + this->_joinedUsers.find(fd)->second.getUSERNAME() + "@" +  this->_HostName + " JOIN " + this->getChannelName() + "\r\n";

/*

RPL_NAMREPLY(fd, nickName,  chName, members)
{
	std::string message = ":IrcTheThreeMusketeers 353 " + nickName + " = " + chName + " :" + members + "\r\n";
	writeMessageToClient_fd(fd, message);
}
RPL_ENDOFNAMES(fd, nickName,  chName)
{
	std::string message = ":IrcTheThreeMusketeers 366 " + nickName + " " + chName + " :End of /NAMES list\r\n";
	writeMessageToClient_fd(fd, message);
}
*/
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
		if (channel.getUsers().size() >= channel.getLimit()) {
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
	for (int i = 0; i < users.size(); i++) {
		joinedUsers += users[i].getNickname() + " ";
	}
	ListMessage = "353 " + client.getNickname() + " = " + channelName + " :" + joinedUsers + "\r\n";
	this->sendMessageToClient(ListMessage, client);
	// 4. end of list
	std::string endOfListMessage = "366 " + channelName + " :End of NAMES list\r\n";
	this->sendMessageToClient(endOfListMessage, client);
}

void Server::handleJoinCommand(Client &client) {
	// JOIN #channel1,#channel2,#channel3 key1,key2,key3
	std::string data = client.getParameters().size() >= 1 ? this->fdToClient[client.getSocketfd()].getParameters()[0] : "";
	std::string data1 = client.getParameters().size() >= 2 ? this->fdToClient[client.getSocketfd()].getParameters()[1] : "";
	std::vector<std::string> channels = split(data, ",");
	std::vector<std::string> keys = split(data1, ",");
	for (int i = 0; i < channels.size(); i++) {
		if (i < keys.size()) {
			this->join_channel(channels[i], keys[i], client);
		}
		else {
			this->join_channel(channels[i], "", client);
		}
	}
}

Client Server::getClientByNickname(std::string nickname) {
	for (int i = 0; i < this->clients.size(); i++) {
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
		std::string msg = "401 ERR_NOSUCHNICK" + recipientNickname + " No such user with this nickname\r\n";
		this->sendMessageToClient(msg, client);
	}
}

void Server::sendPrivateMessageToChannel(Client &client, std::string channelName, std::string message) {
	if (this->nameToChannel.find(channelName) != this->nameToChannel.end()) {
		if (this->nameToChannel[channelName].isUserInChannel(client)) {
			std::string msgToLimChat = ":" + client.getNickname() + "!" + client.getUsername() + "@" + this->serverHostName + " PRIVMSG " + channelName + " :" + message + "\r\n";
			this->broadcastMessageOnChannel(this->nameToChannel[channelName], msgToLimChat, client);
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
	for (int i = 0; i < recipients.size(); i++) {
		if (recipients[i][0] != '#') {
			this->sendPrivateMessageToClient(client, recipients[i], message);
		}
		else
			this->sendPrivateMessageToChannel(client, recipients[i], message);
	}
}


void Server::printAllClients(std::string data) {
	std::istringstream str(data);
	std::string token;
	str >> token;
	if (token != "PRINT")
		return;
	std::cout << "All clients: " << std::endl;
	for (int i = 0; i < this->clients.size(); i++) {
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

bool Server::processClientData(Client &client, std::string data) {

	this->fdToClient[client.getSocketfd()].parseIrcMessage(data);
	std::string command = this->fdToClient[client.getSocketfd()].getCommand();
	// print parameters
	std::cout << "Activity detected: Client number " << client.getSocketfd() << "------------------------------------------"  << std::endl;
	std::cout << "command: [" << command <<"]"<< std::endl;
	std::cout << "parameters: ";
	for (int i = 0; i < this->fdToClient[client.getSocketfd()].getParameters().size(); i++) {
		std::cout << "[" << this->fdToClient[client.getSocketfd()].getParameters()[i] << "] ";
	}
	std::cout << std::endl;
	// check not enough parameters
	if (command != "" && this->fdToClient[client.getSocketfd()].getParameters().size() == 0
		&& command != "PART" && command != "part" &&  command != "!quiz" && command != "!QUIZ"
		&& command != "!date" && command != "!DATE")
	{
		std::string response = "461 " + client.getNickname() + " " + command + " :Not enough parameters\r\n";
		sendMessageToClient(response, client);
		return false;
	}
	// 1.AUTHENTICATION
	if (command == "PASS" || command == "pass") {
		if (!client.getIsAuthenticated()) {
			if (!client.authenticate(this->password)){
				return false;
			}
		}
		else {
			std::string response = "462 You may not reregister\r\n";
			sendMessageToClient(response, client);
		}
	}
	if (!client.getIsAuthenticated()) {
		std::string response = "451 You have not registered yet\r\n";
		sendMessageToClient(response, client);
		return false;
	}
	// 2. NICKNAME
	if (command == "NICK" || command == "nick")
		this->handleNickCommand(client);
	// 3. USERNAME
	if (command == "USER" || command == "user")
		this->handleUserCommand(client);
	// 4. JOIN
	if (command == "JOIN" || command == "join")
		this->handleJoinCommand(client);
	// 5. PRIVMSG
	if (command == "PRIVMSG" || command == "privmsg")
		this->handlePrivateMessageCommand(client);
	// 6. KICK
	if (command == "KICK" || command == "kick")
		this->handleKickCommand(client);
	// 7. INVITE
	if (command == "INVITE" || command == "invite")
		this->handleInviteCommand(client);
	// 8. TOPIC
	if (command == "TOPIC" || command == "topic")
		this->handleTopicCommand(client);
	// 8. MODE
	if (command == "MODE" || command == "mode")
		this->handleModeCommand(client);
	// 7. PART
	if (command == "PART" || command == "part")
		this->handlePartCommand(client);
	if (command == "NOTICE" || command == "notice")
		this->handleNoticeCommand(client);
	if (command == "!quiz" || command == "!QUIZ" || command == "!date" || command == "!DATE")
		this->launchBot(client);
	// std::cout << "client info: " << std::endl;
	// std::cout << "nickname: [" << client.getNickname() << "]" << std::endl;
	// std::cout << "username: [" << client.getUsername() << "]" << std::endl;
	// std::cout << "realname: [" << client.getRealname() << "]" << std::endl;
	// std::cout << "isAuthenticated: [" << (client.getIsAuthenticated() ? "YES" : "NO") << "]" << std::endl;
	// std::cout << "isOperator: [" << (client.getIsOperator() ? "YES" : "NO") << "]" << std::endl;
	if (!client.getIsUserRegistered() && client.getIsAuthenticated() && client.getNickname() != "" && client.getUsername() != "")
		this->sendConfimationMessage(client);
	return true;
}

bool Server::handleRecievedData(Client &client, std::string data) {
	// Append racieved data to the buffer of the client
	this->fdToBuffer[client.getSocketfd()] += data;
	client.setBuffer(this->fdToBuffer[client.getSocketfd()]);
	// check if the buffer contains a complete message
	std::string buffer = this->fdToBuffer[client.getSocketfd()];
	size_t pos = buffer.find("\r\n");
	size_t pos2 = buffer.find("\n");
	if (pos != std::string::npos) {
		std::vector<std::string> cmds = split(buffer, "\r\n");
		for (int i = 0; i < cmds.size() - 1; i++) {
			std::cout << "(" << cmds[i] << ")" << std::endl;
			if (!this->processClientData(client, cmds[i])) {
				this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next commands
				return false;
			}
		}
		this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next commands
	}
	else if (pos2 != std::string::npos) {
		std::vector<std::string> cmds = split(buffer, "\n");
		for (int i = 0; i < cmds.size() - 1; i++) {
			std::cout << "(" << cmds[i] << ")" << std::endl;
			if (!this->processClientData(client, cmds[i])) {
				this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next commands
				return false;
			}
		}
		this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next commands
	}
	return true;
}

bool Server::acceptNewMessage(int socketfd) {
	char buffer[4096];
	memset(buffer, 0, 4096);
	int bytesReceived = recv(socketfd, buffer, 4096, 0);
	if (bytesReceived == -1) {
		std::cerr << "Error in recv(). Quitting" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bytesReceived == 0) {
		this->removeDisconnectedClient(socketfd);
		return false;
	}
	if (buffer[0] == '\n') {
		return false;
	}
	// process the message
	std::string data = std::string(buffer, 0, bytesReceived);
	if (!this->handleRecievedData(this->fdToClient[socketfd], data)) {
		return false;
	}
	return true;
}

void Server::handleEvents() {
	// create a poll set and add the server socket to it
	this->pollfds.push_back({this->serverSocket.getFd(), POLLIN, 0});
	while(true) {
		// poll for events hangs indefinitely till an event occurs on any of the sockets in the poll set
		int beReady = poll(this->pollfds.data(), this->pollfds.size(), -1);
		if (beReady == -1) {
			std::cerr << "Error in poll" << std::endl;
			exit(EXIT_FAILURE);
		}
		// check if the server socket is ready to accept a new connection
		if (this->pollfds[0].revents & POLLIN) {
			int clientSocketfd = this->acceptNewConnection();
			this->pollfds.push_back({clientSocketfd, POLLIN, 0});
		}
		// check if any of the client sockets is ready to receive data
		for (int i = 1; i < this->pollfds.size(); i++) {
			if (this->pollfds[i].revents & POLLIN) {
				if (!this->acceptNewMessage(this->pollfds[i].fd)) {
					continue;
				}
				for (int j = 0; j < this->clients.size(); j++) {
					if (this->clients[j].getSocketfd() == this->pollfds[i].fd) {
						this->clients[j] = this->fdToClient[this->pollfds[i].fd];
						break;
					}
				}
			}
		}
	}
}

void Server::startServer() {
	// create a server socket
	Socket serverSocket(AF_INET, SOCK_STREAM, 0, this->serverPort, INADDR_ANY);
	this->serverSocket = serverSocket;
	// bind the server socket to the server address
	this->serverSocket.bindSocket();
	// start listening for connections
	this->serverSocket.listenSocket(this->MAX_CLIENTS);
	std::cout << "Server started, Listening on port " << this->serverPort << std::endl;
	this->handleEvents();
	// close the server socket
}

Server::~Server()
{
	this->stopServer();
}
