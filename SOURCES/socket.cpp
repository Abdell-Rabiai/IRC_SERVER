# include "../HEADERS/socket.hpp"

Socket::Socket(int domain, int type, int protocol, int port, unsigned long ip) {
    this->_domain = domain;
    this->_type = type;
    this->_protocol = protocol;
    // define the address structure
    this->address.sin_family = domain;
    this->address.sin_addr.s_addr = ip;
    this->address.sin_port = htons(port);
    memset(this->address.sin_zero, '\0', sizeof this->address.sin_zero);
    // create the socket, establish the connection
    this->fd = socket(this->_domain, this->_type, this->_protocol);
}

Socket::Socket() {
    this->_domain = AF_INET;
    this->_type = SOCK_STREAM;
    this->_protocol = 0;
    // define the address structure
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = htonl(INADDR_ANY);
    this->address.sin_port = htons(0);
    memset(this->address.sin_zero, '\0', sizeof this->address.sin_zero);
    // create the socket, establish the connection
    this->fd = socket(this->_domain, this->_type, this->_protocol);
}

Socket::~Socket() {
    this->closeSocket();
}

// this is used to bind the server socket to the server address
// which is a combination of the server ip and the server port
void Socket::bindSocket() {
    if (bind(this->fd, (struct sockaddr *)&(this->address), sizeof(this->address)) < 0)
    {
        std::cerr << "Error binding socket to address" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// this is used to listen on the server socket for new connections
// all incoming connections will be queued until they are accepted
void Socket::listenSocket(int maxConnections) {
    if (listen(this->fd, maxConnections) < 0)
    {
        std::cerr << "Error listening on socket" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// this is used to accept a new connection on the server socket
// it returns the client socket file descriptor
int Socket::acceptSocket() {
    int clientSocketfd;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);

    // create a new client socket
    clientSocketfd = accept(this->fd, (struct sockaddr *)&clientAddress, &clientAddressSize);
    if (clientSocketfd < 0)
    {
        std::cerr << "Error accepting new connection" << std::endl;
        exit(EXIT_FAILURE);
    }
    return clientSocketfd;
}

// this is used to connect to a server socket from a client socket

void Socket::connectSocket() {
    if (connect(this->fd, (struct sockaddr *)&(this->address), sizeof(this->address)) < 0)
    {
        std::cerr << "Error connecting to server" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Socket::Socket(const Socket &socket) {
    this->_domain = socket._domain;
    this->_type = socket._type;
    this->_protocol = socket._protocol;
    this->fd = socket.fd;
    this->address = socket.address;
}

Socket &Socket::operator=(const Socket &socket) {
    this->_domain = socket._domain;
    this->_type = socket._type;
    this->_protocol = socket._protocol;
    this->fd = socket.fd;
    this->address = socket.address;
    return *this;
}

void Socket::closeSocket() {
    close(this->fd);
}

int Socket::getDomain() {
    return this->_domain;
}

int Socket::getType() {
    return this->_type;
}

int Socket::getProtocol() {
    return this->_protocol;
}

int Socket::getFd() {
    return this->fd;
}

const struct sockaddr_in &Socket::getAddress() {
    return this->address;
}

void Socket::setDomain(int domain) {
    this->_domain = domain;
}

void Socket::setType(int type) {
    this->_type = type;
}

void Socket::setProtocol(int protocol) {
    this->_protocol = protocol;
}

void Socket::setFd(int fd) {
    this->fd = fd;
}

void Socket::setAddress(struct sockaddr_in address) {
    this->address = address;
}


