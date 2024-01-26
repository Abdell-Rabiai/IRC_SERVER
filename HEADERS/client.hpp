# pragma once
# include "all_headers.hpp"
# include "channel.hpp"

class Client
{
	protected :
		int socketfd;
		std::string nickname;
		std::string hostname;
		std::string username;
		std::string realname;
		std::string port;
		bool isOperator;
		bool isAuthenticated;
		std::vector<std::string> buffer;

	public :
		Client();
		Client(int socketfd);
		Client(int socketfd, std::string nickname, std::string username);
		Client(int socketfd, std::string nickname, std::string username, std::string realname);
		~Client();

		// methods
		void sendMessageToClient(std::string message , Client client);
		void sendMessageToChannel(std::string message, Channel channel);
		bool authenticate(std::string _password, std::string password, std::string data);
		void becomeOperator();
		void processReceivedData(std::string message);
		void printClientInfo();
		// JOIN

		// methods specific to operator, we'll add them later
        void kickClient(Client client, Channel channel); // KICK
        void inviteClient(Client client, Channel channel); // INVITE

        void setChannelTopic(Channel channel, std::string topic); // TOPIC
        void viewChannelTopic(Channel channel); // TOPIC
        void setChannelMode(Channel channel, std::string mode); // MODE
        void setInviteOnly(Channel channel); // MODE
        void removeInviteOnly(Channel channel); // MODE
        void setRestrictedTopic(Channel channel); // MODE
        void removeRestrictedTopic(Channel channel); // MODE
        void setChannelKey(Channel channel, std::string key); // MODE
        void removeChannelKey(Channel channel);  // MODE
        void giveOperatorPrivileges(Channel channel, Client client); // MODE
        void removeOperatorPrivileges(Channel channel, Client client); // MODE
        void setChannelLimit(Channel channel, int limit); // MODE
        void removeChannelLimit(Channel channel); // MODE

		// getters
		int getSocketfd();
		std::string getNickname();
		std::string getUsername();
		std::string getRealname();
		bool getIsOperator();
		bool getIsAuthenticated();
		// std::vector<Channel> getCurrentChannels();
		// std::map<std::string, Channel> getNameToChannel();
		std::string getHostName();
		std::string getPort();

		// setters
		void setSocketfd(int socketfd);
		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setRealname(std::string realname);
		void setIsOperator(bool isOperator);
		void setIsAuthenticated(bool isAuthenticated);
		// void setCurrentChannels(std::vector<Channel> currentChannels);
		// void setNameToChannel(std::map<std::string, Channel> nameToChannel);
		void setHostName(std::string hostname);
		void setPort(std::string port);
};
