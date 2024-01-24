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

void Server::broadcastMessageOnServer(std::string message) {
	// send the message to all clients including the server except the sender client 
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
		std::cout << "Client disconnected " << std::endl;
		close(socketfd);
		return false;
	}
	if (buffer[0] == '\n') {
		return false;
	}
	// display message in this format <hostname>: [port]: "message"
	std::string msg = "<"+ this->fdToClient[socketfd].getHostName() + "> [" 
						+ this->fdToClient[socketfd].getPort() + "]: "
						+ std::string(buffer, 0, bytesReceived - 1) + "\n";

	// this->broadcastMessageOnServer(msg);
	std::string thankYouMsg = "Sent\n";
	send(socketfd, thankYouMsg.c_str(), thankYouMsg.size() + 1, 0);
	for (int i = 1; i < this->pollfds.size(); i++) {
		if (this->pollfds[i].fd != socketfd) {
			send(this->pollfds[i].fd, msg.c_str(), msg.length(), 0);
		}
	}
	return true;
}

void Server::handleEvents() {
	// create a poll set
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

/*
void Server::acceptNewMessage(int socketfd) {
	char buffer[4096];
	while(true) {
		memset(buffer, 0, 4096);
		int bytesReceived = recv(socketfd, buffer, 4096, 0);
		if (bytesReceived == -1) {
			std::cerr << "Error in recv(). Quitting" << std::endl;
			exit(EXIT_FAILURE);
		}
		if (bytesReceived == 0) {
			std::cout << "Client disconnected " << std::endl;
			close(socketfd);
			return;
		}
		if (buffer[0] == '\n') {
			continue;
		}
		// display message
		std::string message = std::string(buffer, 0, bytesReceived - 1);
		int i = message.find_first_of(' ');
		std::string command = message.substr(0, i);
		std::string password = message.substr(i + 1, message.length() - i - 1);
		std::cout << "pasword: {" << password <<"}"<< std::endl;
		std::string msg = "You need to authenticate first\n";
		if (command != "PASS" || password != this->password) {
			send(socketfd, msg.c_str(), msg.length(), 0);
			continue;
		}
		msg = "You are authenticated successfully \nNow set a nickname using the command NICK <nickname>\n";
		send(socketfd, msg.c_str(), msg.length(), 0);

		std::cout << "Received: " << message << " from " << socketfd << std::endl;
		int bytesnickname = recv(socketfd, buffer, 4096, 0);
		if (bytesnickname == -1) {
			std::cerr << "Error in recv(). Quitting" << std::endl;
			exit(EXIT_FAILURE);
		}
		if (bytesnickname == 0) {
			std::cout << "Client disconnected " << std::endl;
			close(socketfd);
			return;
		}
		if (buffer[0] == '\n') {
			continue;
		}

		std::string n = "your nickname is: " + std::string(buffer, 0, bytesnickname - 1) + "\n";
		send(socketfd, n.c_str(), n.length(), 0);
		// echo a thank you message back to the client
		std::string thankYouMsg = "Thank you !! Your message has been received\n";
		send(socketfd, thankYouMsg.c_str(), thankYouMsg.size() + 1, 0);
	}
}
*/