# ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include "all_headers.hpp"

# include "client.hpp"
# include "channel.hpp"
# include "socket.hpp"


class Server
{
    private:
        Socket serverSocket;
        const int MAX_CLIENTS = SOMAXCONN;
        int serverSocketfd;
        int serverPort;
        std::string password;

        std::vector<int> clientSocketsfds;
        std::vector<Client> clients;
        std::map<int, Client> fdToClient;

        std::vector<Client> operators;
        std::map<int, Client> fdToOperator;

        std::vector<Channel> channels;
        std::map<std::string, Channel> nameToChannel;
        std::vector <pollfd> pollfds;

    public:

        Server();
        Server(int port, std::string password);
        Server(int port, std::string password, Socket serverSocket);
        ~Server();

        void startServer();
        void stopServer();
        void handleEvents(); // handles events such as new connections, new messages, client disconnected

        int acceptNewConnection(); // accepts a new connection and adds it to the clients list
        // and it returns the socket file descriptor of the new client create by accept
        bool acceptNewMessage(int socketfd); // accepts a new message and processes it
        void processClientData(Client client);
        void broadcastMessageOnChannel(Channel channel, std::string message);
        void broadcastMessageOnServer(std::string message);
        void addClient(Client client, int socketfd);
        void removeClient(Client client);
        void addOperator(Client client);
        void removeOperator(Client client);

        // getters
        int getServerSocketfd();
        int getServerPort();
        std::string getPassword();
        std::vector<Client> getClients();
        std::vector<Client> getOperators();
        std::vector<Channel> getChannels();
        std::map<int, Client> getFdToClient();
        std::map<std::string, Channel> getNameToChannel();
        // setters
        void setServerSocketfd(int serverSocketfd);
        void setServerPort(int serverPort);
        void setPassword(std::string password);
        void setClients(std::vector<Client> clients);
        void setOperators(std::vector<Client> operators);
        void setChannels(std::vector<Channel> channels);
        void setFdToClient(std::map<int, Client> fdToClient);
        void setNameToChannel(std::map<std::string, Channel> nameToChannel);

};

std::string printHostInfos(const struct sockaddr_in &address);

# endif // _SERVER_HPP_