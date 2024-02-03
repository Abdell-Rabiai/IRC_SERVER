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
        const int MAX_CLIENTS;
        int serverSocketfd;
        int serverPort;
        std::string password;
        std::string serverCreationTime;
        std::string serverName;
        std::string serverHostName;

        std::vector<int> clientSocketsfds;
        std::vector<Client> clients;
        std::map<int, Client> fdToClient;

        std::vector<Client> operators;
        std::map<int, Client> fdToOperator;

        std::vector<Channel> channels;
        std::map<std::string, Channel> nameToChannel;
        std::vector <pollfd> pollfds;

        std::map<int, std::string> fdToBuffer;
        std::map<std::string, std::string> NameToPassword; // channel name to password

    public:

        Server();
        Server(int port, std::string password);
        Server(int port, std::string password, Socket serverSocket);
        ~Server();

        void startServer();
        std::string getServerIp();
        void stopServer();
        void handleEvents(); // handles events such as new connections, new messages, client disconnected
        static std::string getCurrentTime();
        void printAllClients(std::string data);
        void sendConfimationMessage(Client &client);
        int acceptNewConnection(); // accepts a new connection and adds it to the clients list
        // and it returns the socket file descriptor of the new client create by accept

        Client getClientByNickname(std::string nickname);
        void sendPrivateMessageToClient(Client &client, std::string recipientNickname, std::string message);
        void sendPrivateMessageToChannel(Client &client, std::string channelName, std::string message);

        bool acceptNewMessage(int socketfd); // accepts a new message and processes it
        bool processClientData(Client &client, std::string data);
        void broadcastMessageOnChannel(Channel channel, std::string message);
        void broadcastMessageOnChannel(Channel channel, std::string message, Client sender);
        void broadcastMessageOnServer(std::string &message, int senderSocketfd);
        void removeClient(Client client);
        void removeDisconnectedClient(int &socketfd);
        void addClient(Client client, int socketfd);

        void removeChannel(std::string channelName);

        void sendMessageToClient(std::string message , Client client);
        void sendMessageToChannel(std::string message, Channel channel);
        void createChannel(std::string name, std::string password, Client &creator);
        
        bool handleNickCommand(Client &client);
        bool handleUserCommand(Client &client);
        
        void handleJoinCommand(Client &client);
        void JoinResponse(Client &client, std::string channelName);
        bool isChannelExist(std::string channelName);
        bool isClientInChannel(Client &client, std::string channelName);
        bool isChannelhasTopic(std::string channelName);
        bool isChannelHasPassword(std::string channelName);
        bool isChannelPasswordCorrect(std::string channelName, std::string password);
        Channel getChannelByName(std::string channelName);
        void handlePrivateMessageCommand(Client &client);

        bool handleRecievedData(Client &client, std::string data);
        void join_channel(std::string channelName, std::string key, Client &creator);

        // getters
        int getServerSocketfd();
        int getServerPort();
        std::string getPassword();
        std::string getServerCreationTime();
        std::string getServerName();
        std::string getServerHostName();

        std::vector<int> getClientSocketsfds();
        std::vector<Client> getClients();
        std::vector<Client> getOperators();
        std::vector<Channel> getChannels();
        std::map<int, Client> getFdToClient();
        std::map<std::string, Channel> getNameToChannel();
        // setters
        void setServerSocketfd(int serverSocketfd);
        void setServerPort(int serverPort);
        void setPassword(std::string password);
        void setServerCreationTime(std::string serverCreationTime);
        void setServerName(std::string serverName);
        void setServerHostName(std::string serverHostName);

        void setClientSocketsfds(std::vector<int> clientSocketsfds);
        void setClients(std::vector<Client> clients);
        void setOperators(std::vector<Client> operators);
        void setChannels(std::vector<Channel> channels);
        void setFdToClient(std::map<int, Client> fdToClient);
        void setNameToChannel(std::map<std::string, Channel> nameToChannel);


        //

        // void viewTopic(Client &client, std::string channelName);
        void viewTopic(Client &client, std::string channelName, Channel &channel);
        void handleTopicCommand(Client &client);
        void handleInviteCommand(Client &client);
        void inviteToChannel(Client &inviter, Channel &channel, Client &invitedClient);
        void handleKickCommand(Client &client);
        void handleModeCommand(Client &client);
        void handleNoticeCommand(Client &client);
        void handlePartCommand(Client &client);
        Channel &getChannelRefByName(std::string channelName);
        bool isOperatorInChannel(Client client, Channel channel);
        void do_modes(Client &client, Channel &channel, std::vector<std::string> modes);
        void launchBot(Client &client);
        int check_param(size_t i, std::vector<std::string> modes, Client &client);

};

std::string printHostInfos(const struct sockaddr_in &address);


# endif // _SERVER_HPP_