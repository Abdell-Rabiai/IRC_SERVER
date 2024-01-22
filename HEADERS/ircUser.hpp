# pragma once
# include <string>
# include <vector>
# include <map>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <string.h>
# include "channel.hpp"


class Channel;

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