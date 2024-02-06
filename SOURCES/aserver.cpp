# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"

Server::Server() : MAX_CLIENTS(SOMAXCONN) {
	this->serverName = ":IRC_SERVER";
	this->serverCreationTime = Server::getCurrentTime();
	this->serverSocketfd = serverSocket.getFd();
	this->serverHostName = getServerIp();
	this->serverPort = 6667;
	this->password = "pass";
}

bool Server::isOperatorInChannel(Client client, Channel channel) {
	for (size_t i = 0; i < channel.getOperators().size(); i++) {
		if (channel.getOperators()[i].getNickname() == client.getNickname())
			return true;
	}
	return false;
}

Server::Server(int port, std::string password) : MAX_CLIENTS(SOMAXCONN) {
	this->serverPort = port;
	this->password = password;
	this->serverName = ":IRC_SERVER";
	this->serverCreationTime = Server::getCurrentTime();
	this->serverSocketfd = serverSocket.getFd();
	this->serverHostName = getServerIp();
}

Server::Server(int port, std::string password, Socket serverSocket) : MAX_CLIENTS(SOMAXCONN) {
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

// methods of sever

std::string printHostInfos(const struct sockaddr_in &address, Client *client) {
	char hostname[NI_MAXHOST];      // The remote host name
	char service[NI_MAXSERV];   // The port the remote host is connected to
	socklen_t addr_len = sizeof(address);
	int result = getnameinfo((struct sockaddr *)&address, addr_len, hostname, NI_MAXHOST, service, NI_MAXSERV, 0);
	if (result) {
		std::cout << "Error in getnameinfo: " << gai_strerror(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Client with hostname: " << hostname << " connected on port number: " << service << std::endl;
	client->setHostName(hostname);
	client->setPort(service);
	return std::string(hostname);
}



void Server::removeDisconnectedClient(int &socketfd) {
	// Remove the disconnected client from pollfds
	std::cout << "Client with socketfd: [" << socketfd << "] has disconnected" << std::endl;
	for (size_t i = 0; i < this->pollfds.size(); i++) {
		if (this->pollfds[i].fd == socketfd) {
			this->pollfds.erase(this->pollfds.begin() + i);
			break ;
		}
	}
	// Remove the disconnected client from all the channels he is in
	for (size_t i = 0; i < this->channels.size(); i++) {
		if (this->channels[i].isUserInChannel(this->fdToClient[socketfd])) {
			this->channels[i].removeUser(this->fdToClient[socketfd]);
		}
	}
	// Remove the disconnected client from the server
	this->removeClient(this->fdToClient[socketfd]);
	// close the socket
	close(socketfd);
}

bool Server::handleNickCommand(Client &client) {
	std::string nickname = this->fdToClient[client.getSocketfd()].getParameters()[0];
	if (this->getClientByNickname(nickname).getSocketfd() != -1) {
		std::string msg = "433 " + nickname + " :Nickname is already in use\r\n";
		this->sendMessageToClient(msg, client);
		return false;
	}
	if (nickname != "") {
		client.setNickname(nickname);
		std::string msg = "your nickname is set as: " + client.getNickname() + "\r\n";
		this->sendMessageToClient(msg, client);
	}
	return true;
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

bool Server::isRegistered(Client &client) {
	if (client.getNickname() == "" || client.getUsername() == "" || !client.getIsAuthenticated()) {
		std::string response = "451 Please register first\r\n";
		this->sendMessageToClient(response, client);
		return false;
	}
	return true;
}

bool Server::processClientData(Client &client, std::string data) {

	this->fdToClient[client.getSocketfd()].parseIrcMessage(data);
	std::string command = this->fdToClient[client.getSocketfd()].getCommand();
	// check not enough parameters
	if (command != "" && this->fdToClient[client.getSocketfd()].getParameters().size() == 0
		&& command != "PART" && command != "part" &&  command != "!bot" && command != "!BOT" && command != "QUIT" && command != "quit")
	{
		std::string response = "461 " + client.getNickname() + " " + command + " :Not enough parameters\r\n";
		this->sendMessageToClient(response, client);
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
			std::string response = "462 Already registred, You may not reregister\r\n";
			this->sendMessageToClient(response, client);
		}
	}
	// 2. NICKNAME
	if (command == "NICK" || command == "nick")
		if (!this->handleNickCommand(client))
			return false;
	// 3. USERNAME
	if (command == "USER" || command == "user")
		this->handleUserCommand(client);
	
	// 4. JOIN
	if ((command == "JOIN" || command == "join") && this->isRegistered(client))
		this->handleJoinCommand(client);
	// 5. PRIVMSG
	if ((command == "PRIVMSG" || command == "privmsg") && this->isRegistered(client))
		this->handlePrivateMessageCommand(client);
	// 6. KICK
	if ((command == "KICK" || command == "kick") && this->isRegistered(client))
		this->handleKickCommand(client);
	// 7. INVITE
	if ((command == "INVITE" || command == "invite") && this->isRegistered(client))
		this->handleInviteCommand(client);
	// 8. TOPIC
	if ((command == "TOPIC" || command == "topic") && this->isRegistered(client))
		this->handleTopicCommand(client);
	// 8. MODE
	if ((command == "MODE" || command == "mode") && this->isRegistered(client))
		this->handleModeCommand(client);
	// 7. PART
	if ((command == "PART" || command == "part") && this->isRegistered(client))
		this->handlePartCommand(client);
	// 8. NOTICE
	if ((command == "NOTICE" || command == "notice") && this->isRegistered(client))
		this->handleNoticeCommand(client);
	// 9. BOT
	if ((command == "!bot" || command == "!BOT") && this->isRegistered(client))
		this->launchBot(client);
	// 10. QUIT : close the connection
	if ((command == "QUIT" || command == "quit") && this->isRegistered(client)) {
		close(client.getSocketfd());
		return false;
	}
	if (!client.getIsUserRegistered() && client.getIsAuthenticated() && client.getNickname() != "" && client.getUsername() != "")
		this->sendConfimationMessage(client);
	std::cout << "Activity detected: Client number " << client.getSocketfd() << "------------------------------------------"  << std::endl;
	std::cout << "command: [" << command << "]\t";
	std::cout << "parameters: ";
	for (size_t i = 0; i < this->fdToClient[client.getSocketfd()].getParameters().size(); i++) {
		std::cout << "[" << this->fdToClient[client.getSocketfd()].getParameters()[i] << "] ";
	}
	std::cout << "\nclient info: " << std::endl;
	std::cout << "nickname: [" << client.getNickname() << "]" << std::endl;
	std::cout << "username: [" << client.getUsername() << "]" << std::endl;
	std::cout << "realname: [" << client.getRealname() << "]" << std::endl;
	std::cout << "isUserRegistered: [" << (client.getIsUserRegistered() ? "YES" : "NO") << "]" << std::endl;
	std::cout << "isOperator: [" << (client.getIsOperator() ? "YES" : "NO") << "]" << std::endl;
	std::cout << "END of Activity: Client number " << client.getSocketfd() << "--------------------------------------------"  << std::endl;
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
		for (size_t i = 0; i < cmds.size() - 1; i++) {
			if (!this->processClientData(client, cmds[i])) {
				this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next commands
				return false;
			}
		}
		this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next commands
	}
	else if (pos2 != std::string::npos) {
		std::vector<std::string> cmds = split(buffer, "\n");
		for (size_t i = 0; i < cmds.size() - 1; i++) {
			if (!this->processClientData(client, cmds[i])) {
				this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next commands
				return false;
			}
		}
		this->fdToBuffer[client.getSocketfd()].clear(); // clear the buffer for next commands
	}
	return true;
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

bool Server::acceptNewMessage(int socketfd) {
	char buffer[4096];
	memset(buffer, 0, 4096);
	int bytesReceived = recv(socketfd, buffer, 4096, 0);
	if (bytesReceived == -1) {
		perror("Error in recv");
		throw std::runtime_error("Error in recv! Quitting...");
	}
	if (bytesReceived == 0) {
		this->removeDisconnectedClient(socketfd); /**HERE**/
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
	struct pollfd serverPollfd = {this->serverSocket.getFd(), POLLIN, 0};
	this->pollfds.push_back(serverPollfd);
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
			struct pollfd clientPollfd = {clientSocketfd, POLLIN, 0};
			this->pollfds.push_back(clientPollfd);
		}
		// check if any of the client sockets is ready to receive data
		for (size_t i = 1; i < this->pollfds.size(); i++) {
			if (this->pollfds[i].revents & POLLIN) {
				if (!this->acceptNewMessage(this->pollfds[i].fd)) {
					continue;
				}
				// update the client info in the pollfds vector
				for (size_t j = 0; j < this->clients.size(); j++) {
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
