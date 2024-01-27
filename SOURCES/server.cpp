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
			std::string msg = "your nickname is set as: " + nickname + "\n";
			this->sendMessageToClient(msg, client);
		}
}

void Server::handleUserCommand(Client &client, std::string username, std::string realname) {
		if (username != "") {
			client.setUsername(username);
			std::string msg = "your username is set as: " + username + "\n";
			this->sendMessageToClient(msg, client);
		}
		if (realname != "") {
			client.setRealname(realname);
			std::string msg = "your realname is set as: " + realname + "\n";
			this->sendMessageToClient(msg, client);
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

void Server::createChannel(std::string name, std::string password, Client creator) {
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

void Server::logic(std::string channelName, std::string key, Client creator) {
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

// brojola chof had Join command, hm9aatni void Server::handleJoinCommand(Client &client, std::string data)
// hanta nchr7 lik:
// 3ndk wahd l buffer smito data, w 3ndk wahd l client smito client
// data howa la commande dyalk exemple JOIN #channel1,#channel2,#channel3 password1,password2,password3 ==> in this case there are 3 channels and 3 passwords for each channel
// ha kifach katkhdm JOIN :
// JOIN {list of channels separated by ','} {list of passwords separated by ','}
// if the list of passwords wasn't provided, then channels will be created without passwords
// exemnple : JOIN #channel1,#channel2,#channel3
// if the there are fewer passwords than channels, then the channels that don't have passwords will be created without passwords
// example: JOIN #channel1,#channel2,#channel3 password1 ==> in this case channel1 will have password1 and channel2 and channel3 will be created without passwords
// exemple: JOIN #channel1,#channel2,#channel3 password1,password2 ==> in this case channel1 will have password1 and channel2 will have password2 and channel3 will be created without passwords
// exemple: JOIN #channel1,#channel2,#channel3 password1,password2,password3 ==> in this case channel1 will have password1 and channel2 will have password2 and channel3 will have password3\
// FHMTI, PARSI HADCHI LAH IHFDAK

// hadi li drt ltht khdama mzyan f test case fach kan3ti l kola channel password dyalha, mais f les autres cas la.
void Server::handleJoinCommand(Client &client, std::string data) {
	std::istringstream str(data);
	std::string token;
	while (str >> token) {
		// check if the token is JOIN
		if (token == "JOIN") {
			std::string channels, passwords, key;
			str >> channels;
			std::size_t pos = channels.find(",");
			while(pos !=  std::string::npos) {
				// extract first the channel name from the string of channels
				std::string channelName = channels.substr(0, pos);
				// remove the channel name from the string channels
				channels.erase(0, pos + 1);
				// extract the list of passwords from the string
				str >> passwords;
				pos = passwords.find(",");
				if (pos != std::string::npos){
					// extract the first password from the string of passwords
					key = passwords.substr(0, pos);
					// remove it password from the string of passwords
					passwords.erase(0, pos + 1);
				}
				else {
					// extract the last password from the string of passwords
					key = passwords;
				}
				std::cout << "Client " << client.getSocketfd() << " wants to join channel {" << channelName << "} with password {" << key << "}" << std::endl;
				this->NameToPassword.insert(std::pair<std::string, std::string>(channelName, key));
				// check if the channel not created yet
				// logic to add the Channel to the server or add the client to the channel
				// channel name is channelName and password is key and created by client or client is trying to join the channel
				this->logic(channelName, key, client);
			}
		}
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
	// this->handlePrivateMessageCommand(client, data);
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
	std::cout << "{" << data << "}" <<std::endl;
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
			// add the new client to the map fdToClient
		}
		// check if any of the client sockets is ready to receive data
		for (int i = 1; i < this->pollfds.size(); i++) {
			if (this->pollfds[i].revents & POLLIN) {
				if (!this->acceptNewMessage(this->pollfds[i].fd)) {
					continue;
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
