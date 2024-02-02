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
    msg = "341 " + client.getNickname() + " part :parted by " + client.getNickname();
    std::string reason = client.getTrailing();
    if (!reason.empty())
        msg += " (" + reason + ")\r\n";
    else
        msg += " (No reason given)\r\n";
    channel.removeUser(client);
    this->broadcastMessageOnChannel(channel, msg);
}