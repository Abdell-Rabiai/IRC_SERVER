# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"

void Server::handleNoticeCommand(Client &client) {
    std::string msg = "";
    std::string target = client.getParameters().size() >= 1 ? client.getParameters()[0] : "";
    std::string message = client.getParameters().size() >= 2 ? client.getParameters()[1] : "";
    if (target[0] == '#') {
        Channel &channel = this->getChannelRefByName(target);
        // msg = client.getNickname() + " " + ":" + message + "\r\n";
        // msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + this->serverHostName + " NOTICE " + recipientNickname + " :" + message + "\n";
        msg  = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName() + " PRIVMSG " + target + " :" + message + "\n";
        if (!channel.getName().empty() && channel.isUserInChannel(client))
            this->broadcastMessageOnChannel(channel, msg);
    }
    else {
        Client targetClient = this->getClientByNickname(target);
        msg = client.getNickname() + " " + ":" + message + "\r\n";
        if (!targetClient.getNickname().empty())
            this->sendMessageToClient(msg, targetClient);
    }
}