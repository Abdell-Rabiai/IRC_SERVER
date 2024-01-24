#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>

class Channel {
public:
    std::string name;
    Channel(const std::string& n) : name(n) {}
};

class IRCUser {
public:
    int socketfd;
    std::string username;
    std::string nickname;
    bool isOperator;
    std::vector<Channel> currentChannels;
    std::map<std::string, Channel> channelMap;

    // Common functions
    virtual void sendMessage(const std::string& message) = 0;
    virtual std::string receiveMessage() = 0;
    virtual void setNickname(const std::string& nick) = 0;
    virtual void setUsername(const std::string& user) = 0;
    virtual void joinChannel(const std::string& channel) = 0;
    virtual void sendChannelMessage(const std::string& message) = 0;
    virtual void sendPrivateMessage(const std::string& target, const std::string& message) = 0;

    // Destructor
    virtual ~IRCUser() {}
};

class Client : public IRCUser {
public:
    // Implement the virtual functions
    void sendMessage(const std::string& message) override {
        // Implementation for clients
    }

    std::string receiveMessage() override {
        // Implementation for clients
        return "Received message (client)";
    }

    void setNickname(const std::string& nick) override {
        // Implementation for clients
    }

    void setUsername(const std::string& user) override {
        // Implementation for clients
    }

    void joinChannel(const std::string& channel) override {
        // Implementation for clients
    }

    void sendChannelMessage(const std::string& message) override {
        // Implementation for clients
    }

    void sendPrivateMessage(const std::string& target, const std::string& message) override {
        // Implementation for clients
    }
};

class IRCServer {
private:
    int port;
    std::string password;
    int serverSocket;
    std::vector<Client> clients;

public:
    IRCServer(int p, const std::string& pwd) : port(p), password(pwd), serverSocket(-1) {}

    ~IRCServer() {
        close(serverSocket);
        for (const auto& client : clients) {
            close(client.socketfd);
        }
    }

    // Start the server
    void start() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            std::cerr << "Error creating server socket" << std::endl;
            return;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << "Error binding server socket" << std::endl;
            return;
        }

        if (listen(serverSocket, SOMAXCONN) == -1) {
            std::cerr << "Error listening for connections" << std::endl;
            return;
        }

        std::cout << "Server started. Listening on port " << port << std::endl;

        std::vector<pollfd> fds;
        fds.push_back({serverSocket, POLLIN, 0});  // Add server socket to the poll set

        while (true) {
            int numReady = poll(fds.data(), fds.size(), -1);  // Block indefinitely until an event occurs

            if (numReady == -1) {
                std::cerr << "Error in poll" << std::endl;
                return;
            }

            if (fds[0].revents & POLLIN) {
                // New connection
                sockaddr_in clientAddr{};
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
                if (clientSocket == -1) {
                    std::cerr << "Error accepting connection" << std::endl;
                } else {
                    std::cout << "New connection accepted" << std::endl;
                    clients.push_back({clientSocket, "", "", false, {}, {}});
                    fds.push_back({clientSocket, POLLIN, 0});  // Add client socket to the poll set
                }
            }

            // Handle data from clients
            for (size_t i = 1; i < fds.size(); ++i) {
                if (fds[i].revents & POLLIN) {
                    // Data is ready to be read
                    // Handle read operation for the client
                    // For simplicity, you might want to implement a function to handle client input
                    handleClientInput(clients[i - 1]);
                }
            }
        }
    }

    // Handle input from a client
    void handleClientInput(Client& client) {
        // Implementation to handle client input
        // You can use client.receiveMessage() to get messages from the client
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    std::string password = argv[2];

    IRCServer ircServer(port, password);
    ircServer.start();

    return 0;
}
