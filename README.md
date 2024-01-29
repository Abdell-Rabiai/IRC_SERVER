# IRC_SERVER


Internet Relay Chat or IRC is a text-based communication protocol on the Internet. It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels. IRC clients connect to IRC servers in order to join channels. IRC servers are connected together to form a network.




void Server::handleJoinCommand(Client &client, std::string data) {
	std::istringstream str(data);
	std::string channelNames;
	std::string pwds;
	std::string token;
	std::vector<std::string> channels;
    std::vector<std::string> passwords;
	str >> token;
	if (token == "JOIN") {
		str >> channelNames;
		str >> pwds;
		channels = split(channelNames, ',');
		passwords = split(pwds, ',');
		std::cout << "channels.size() {" << channels.size() << "}" << std::endl;
		for (int i = 0; i < channels.size(); i++) {
			if (i < passwords.size()) {
				this->logic(channels[i], passwords[i], client);
			}
			else {
				this->logic(channels[i], "", client);
			}
		}
	}
}

USER * 0 : realname / to dubbug
 