# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"

Server::Server() {
	this->serverPort = 6667;
	this->password = "pass";
	this->serverSocketfd = serverSocket.getFd();
}

Server::Server(int port, std::string password) {
	this->serverPort = port;
	this->password = password;
	this->serverSocketfd = serverSocket.getFd();
}

Server::Server(int port, std::string password, Socket serverSocket) {
	this->serverPort = port;
	this->password = password;
	this->serverSocket = serverSocket;
	this->serverSocketfd = serverSocket.getFd();
}

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

void Server::stopServer() {
	// close the server socket
	close(this->serverSocket.getFd());
	// close all client sockets
	for (Client client : this->clients) {
		close(client.getSocketfd());
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
	this->addClient(client, clientSocketfd);
	// send a welcome message to the client
	std::string clientSocketfdStr = std::to_string(clientSocketfd);
	std::string welcome_msg = "Welcome to the Server number " + clientSocketfdStr + " \n";
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
						+ std::string(buffer, 0, buffer.length()) + "\n";
	std::string thankYouMsg = "Sent\n";
	send(senderSocketfd, thankYouMsg.c_str(), thankYouMsg.size() + 1, 0);
	for (int i = 1; i < this->pollfds.size(); i++) {
		if (this->pollfds[i].fd != senderSocketfd && this->pollfds[i].fd >= 0) {
			sendMessageToClient(msg, this->fdToClient[this->pollfds[i].fd]);
		}
	}
}

void Server::broadcastMessageOnChannel(Channel channel, std::string message) {
		for (int i = 0; i < channel.getUsers().size(); i++) {
		this->sendMessageToClient(message, channel.getUsers()[i]);
	}
}


void Server::handleNickCommand(Client &client, std::string nickname) {
		if (nickname != "") {
			client.setNickname(nickname);
			std::string msg = "your nickname is set as: " + client.getNickname() + "\n";
			this->sendMessageToClient(msg, client);
		}
}

void Server::handleUserCommand(Client &client, std::string username, std::string realname) {
		if (username != "") {
			client.setUsername(username);
			std::string msg = "your username is set as: " + client.getUsername() + "\n";
			this->sendMessageToClient(msg, client);
		}
		if (realname != "") {
			client.setRealname(realname);
			std::string msg = "your realname is set as: " + client.getRealname() + "\n";
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

void Server::createChannel(std::string name, std::string password, Client &creator) {
	Channel channel(name);
	if (password != "")
		channel.setKey(password);
	channel.addUser(creator);
	channel.addOperator(creator);
	creator.setIsOperator(true);
	this->channels.push_back(channel);
	this->nameToChannel.insert(std::pair<std::string, Channel>(name, channel));
	std::string msg = "You have created the channel " + name  + " as an Operator\n";
	this->sendMessageToClient(msg, creator);
}

void Server::logic(std::string channelName, std::string key, Client &creator) {
	if (this->nameToChannel.find(channelName) == this->nameToChannel.end()) {
		if (!key.empty())
			this->createChannel(channelName, key, creator);
		else
			this->createChannel(channelName, "", creator);
	}
	else {
		if (this->nameToChannel[channelName].getKey() == key || this->nameToChannel[channelName].getKey() == "") {
			this->nameToChannel[channelName].addUser(creator);
			std::string msg = "You have joined the channel " + channelName + " as a Regular User\n";
			this->sendMessageToClient(msg, creator);
		}
		else {
			std::string msg = "Please enter the correct password to join the channel " + channelName + "\n";
			this->sendMessageToClient(msg, creator);
		}
	}
}

std::vector<std::string> split(std::string str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

void Server::handleJoinCommand(Client &client, std::string data) {
	std::istringstream str(data);
	std::string channelNames;
	std::string pwds;
	std::string token;
	std::vector<std::string> channels;
    std::vector<std::string> passwords;
	str >> token;
	if (token == "JOIN") {
		str >> channelNames;
		str >> pwds;
		channels = split(channelNames, ',');
		passwords = split(pwds, ',');
		std::cout << "channels.size() {" << channels.size() << "}" << std::endl;
		for (int i = 0; i < channels.size(); i++) {
			if (i < passwords.size()) {
				this->logic(channels[i], passwords[i], client);
			}
			else {
				this->logic(channels[i], "", client);
			}
		}
	}
}


void Server::handlePrivateMessageCommand(Client &client, std::string data) {
	// first of all parse the data
	std::string message;
	std::vector<std::string> receivers;
	std::istringstream str(data);
	std::string token;
	str >> token; // skip the PRIVMSG token
	if (token != "PRIVMSG")
		return;
	if (str >> token && token[0] != ':') {
		// extract the nickname of the receivers
		receivers = split(token, ',');
	}
	// extract the message
	std::getline(str, message);
	// remove the first space from the message
	if (!message.empty() && message[0] == ' ')
		message.erase(0, 1);
	std::cout << "message {" << message << "}" << std::endl;
	// print the receivers
	for (int i = 0; i < receivers.size(); i++) {
		std::cout << "receiver [" << receivers[i] << "]" << std::endl;
	}

	// check if the receivers are channels or users
	for (int i = 0; i < receivers.size(); i++) {
		if (receivers[i][0] != '#') {
			std::cout << "the receiver is a user [" << receivers[i] << "]" << std::endl;
			// the receiver is a user
			// check if the receiver is connected to the server
			if (this->fdToClient.find(client.getSocketfd()) != this->fdToClient.end()) {
				// the receiver is connected to the server
				// send the message to the receiver
				std::string msg = "<" + client.getNickname() + "> - [" + client.getUsername() + "]: " + message + "\n";
				int fd;
				for (int j = 0; j < this->clients.size(); j++) {
					if (this->clients[j].getNickname() == receivers[i]) {
						fd = this->clients[j].getSocketfd();
						std::cout << "sending the message to the receiver with fd [" << fd << "]" << std::endl;
						this->sendMessageToClient(msg, this->fdToClient[fd]);
						break;
					}
				}
			}
			else {
				// the receiver is not connected to the server
				// send an error message to the sender
				std::string msg = "Error: " + receivers[i] + " is not connected to the server\n";
				this->sendMessageToClient(msg, client);
			}
		}
		else if (receivers[i][0] == '#') {
			std::cout << "the receiver is a the channel [" << receivers[i] << "]" << std::endl;
			// the receiver is a channel
			// check if the channel exists
			if (this->nameToChannel.find(receivers[i]) != this->nameToChannel.end()) {
				// the channel exists
				// check if the sender is a member of the channel
				if (this->nameToChannel[receivers[i]].isUserInChannel(client)) {
					// the sender is a member of the channel
					// send the message to all the members of the channel
					std::string msg = "<" + client.getNickname() + "> - [" + client.getUsername() + "]: " + message + "\n";
					this->broadcastMessageOnChannel(this->nameToChannel[receivers[i]], msg);
				}
				else {
					// the sender is not a member of the channel
					// send an error message to the sender
					std::string msg = "Error: you are not a member of the channel " + receivers[i] + "\n";
					this->sendMessageToClient(msg, client);
				}
			}
			else {
				// the channel does not exist
				// send an error message to the sender
				std::string msg = "Error: the channel " + receivers[i] + " does not exist\n";
				this->sendMessageToClient(msg, client);
			}
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
	for (int i = 0; i < this->clients.size(); i++) {
		std::cout << "Client number " << i << "------------------------------------------"  << std::endl;
		this->clients[i].printClientInfo();
	}
}

bool Server::processClientData(Client &client, std::string data) {

	// 1.AUTHENTICATION
	std::string password, nickname, username, realname;
	parseData(password, nickname, username, realname, data);
	std::cout << "password {" << password << "}" << std::endl;
	std::cout << "this->password {" << this->password << "}" << std::endl;
	if (!client.getIsAuthenticated()) {
		if (!client.authenticate(this->password, password, data)){
			std::cout << "Error in authentication" << std::endl;
			return false;
		}
	}
	// 2. NICKNAME
	this->handleNickCommand(client, nickname);
	// 3. USERNAME
	this->handleUserCommand(client, username, realname);
	// 4. JOIN
	this->handleJoinCommand(client, data);
	// 5. PRIVMSG
	this->handlePrivateMessageCommand(client, data);
	// 6. PRINT
	this->printAllClients(data);
	return true;
}

bool Server::handleRecievedData(Client &client, std::string data) {
	// Append racieved data to the buffer of the client
	this->fdToBuffer[client.getSocketfd()] += data;
	// check if the buffer contains a complete message
	std::string buffer = this->fdToBuffer[client.getSocketfd()];
	int pos = buffer.find("\n");
	if (pos != std::string::npos) {
		// process the message
		if (!this->processClientData(client, buffer)) {
			std::cout << "Error in processingClientdata" << std::endl;
			this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next commands
			return false;
		}
		this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next command.
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
		std::cout << "Error in handling Recieved data" << std::endl;
		return false;
	}
	// std::cout << "{" << data << "}" << std::endl;
	// this->fdToClient[socketfd].printClientInfo();
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
				// affect the changes also to the vector of clients // TODO: make a function to do this
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
