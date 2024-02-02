# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"

// KICK #Finnish Matthew :Speaking English

void Server::handleKickCommand(Client &client) {
    std::string msg = "";
    std::string channelName = client.getParameters().size() >= 1 ? client.getParameters()[0] : "";
    std::string nickname = client.getParameters().size() >= 2 ? client.getParameters()[1] : "";

    if (client.getParameters().size() < 2) {
        msg = "461 " + client.getNickname() + " KICK :Not enough parameters" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
    }
    Channel &channel = this->getChannelRefByName(channelName);
    Client kickedClient = this->getClientByNickname(nickname);
    if (!this->isOperatorInChannel(client, channel) || !channel.isUserInChannel(client)) {
        msg = "482 " + client.getNickname() + " " + channelName + " :You're not channel operator" + "\r\n" ;
        this->sendMessageToClient(msg, client);
        return;
    }
    if (!channel.isUserInChannel(kickedClient)) {
        msg = "441 " + client.getNickname() + " " + nickname + " " + channelName + " :They aren't on that channel" + "\r\n" ;
        this->sendMessageToClient(msg, client);
        return;
    }
    msg = "341 " + client.getNickname() + " KICK :Kicked by " + client.getNickname();
    std::string reason = client.getTrailing();
    if (!reason.empty())
        msg += " (" + reason + ")\r\n";
    else
        msg += " (No reason given)\r\n";
    channel.removeUser(kickedClient);
    this->sendMessageToClient(msg, kickedClient);
}