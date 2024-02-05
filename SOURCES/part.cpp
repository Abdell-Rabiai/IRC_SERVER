# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"


void Server::handlePartCommand(Client &client) {
    std::string msg = "";
    std::string channelName = client.getParameters().size() >= 1 ? client.getParameters()[0] : "";
    if (client.getParameters().size() < 1) {
        msg = "461 " + client.getNickname() + " part :Not enough parameters" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
    }
    Channel &channel = this->getChannelRefByName(channelName);
    if (!channel.isUserInChannel(client) || !this->isChannelExist(channelName)) {
        msg = "441 " + client.getNickname() + " " + channelName + " :They aren't on that channel" + "\r\n" ;
        this->broadcastMessageOnChannel(channel, msg);
        return;
    }
    //check if this client who wants to part is the only operator in the channel
    if (client.getIsOperator() && channel.getOperators().size() == 1) {
        std::string msg3 = "482 " + client.getNickname() + " " + channelName + " :You're the only operator in the channel" \
        " You must set an operator before leaving the channel" + "\r\n";
        this->sendMessageToClient(msg3, client);
        return;
    }
    msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName() + " PART " + channelName + " : PARTED";
    std::string reason = client.getTrailing();
    if (!reason.empty())
        msg += " (" + reason + ")\r\n";
    else
        msg += " (No reason given)\r\n";
    channel.removeUser(client);
    this->broadcastMessageOnChannel(channel, msg);
    std::string msg2 = msg = "121 " + client.getNickname() + "!" + client.getUsername() + "@" + this->serverHostName + " You have left the channel " + channelName + "\r\n";
    this->sendMessageToClient(msg2, client);
    if (channel.getUsers().size() == 0) {
        this->removeChannel(channelName);
    }
}