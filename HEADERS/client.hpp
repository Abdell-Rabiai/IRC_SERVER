# pragma once
# include "all_headers.hpp"
class Channel;


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
		std::string buffer;

		std::string command;
		std::vector<std::string> parameters;
		std::string trailing;

		bool isUserRegistered;

	public :
		Client();
		Client(int socketfd);
		Client(int socketfd, std::string nickname, std::string username);
		Client(int socketfd, std::string nickname, std::string username, std::string realname);
		~Client();
		bool operator==(const Client &client) const;

		// methods
		void parseIrcMessage(std::string buffer);
		bool authenticate(std::string SeverPassword);

		// nickname related methods

		void becomeOperator();
		void processReceivedData(std::string message);
		void printClientInfo();
		bool confirmRegistration();
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
        void setChannelLimit(Channel channel, size_t limit); // MODE
        void removeChannelLimit(Channel channel); // MODE

		// getters
		int getSocketfd();
		std::string getNickname();
		std::string getUsername();
		std::string getRealname();
		bool getIsOperator();
		bool getIsAuthenticated();
		// bool isNicknameSet();
		// bool isUsernameSet();
		std::string getBuffer();
		std::string getCommand();
		std::vector<std::string> getParameters();
		std::string getTrailing();
		std::string getHostName();
		std::string getPort();
		bool getIsUserRegistered();

		// setters
		void setSocketfd(int socketfd);
		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setRealname(std::string realname);
		void setIsOperator(bool isOperator);
		void setIsAuthenticated(bool isAuthenticated);
		void setBuffer(std::string buffer);
		void setCommand(std::string command);
		void setParameters(std::vector<std::string> parameters);
		void setTrailing(std::string trailing);
		void setHostName(std::string hostname);
		void setPort(std::string port);
		void setIsUserRegistered(bool isUserRegistered);
};
