# pragma once
# include "all_headers.hpp"
# include "channel.hpp"

class IrcUser
{
	protected :
		int socketfd;
		std::string nickname;
		std::string username;
		bool isOperator;
		bool isAuthenticated;
		std::vector<Channel> currentChannels;
		std::map<std::string, Channel> nameToChannel;

	public :
		IrcUser();
		IrcUser(int socketfd);
		~IrcUser();

		// methods
		void sendMessageToClient(std::string message);
		void sendMessageToChannel(std::string message);
		void joinChannel(Channel channel);
		void authenticate();
		void becomeOperator();
		void processReceivedData(std::string message);

		// getters
		int getSocketfd();
		std::string getNickname();
		std::string getUsername();
		bool getIsOperator();
		bool getIsAuthenticated();
		std::vector<Channel> getCurrentChannels();
		std::map<std::string, Channel> getNameToChannel();

		// setters
		void setSocketfd(int socketfd);
		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setIsOperator(bool isOperator);
		void setIsAuthenticated(bool isAuthenticated);
		void setCurrentChannels(std::vector<Channel> currentChannels);
		void setNameToChannel(std::map<std::string, Channel> nameToChannel);
};

class Client : public IrcUser
{
    private :
            // none yet, we'll add them later if needed

    public :
        Client(int socketfd);
        ~Client();

        // methods specific to client, we'll add them later
};

class Operator : public IrcUser
{
    private : 
        // none yet, we'll add them later if needed
    
    public :
        Operator(int socketfd);
        ~Operator();

        // methods specific to operator, we'll add them later
        void kickClient(Client client); // KICK
        void inviteClient(Channel channel, Client client); // INVITE
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
};
