# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"

/*
TOPIC #test :New topic          ; Setting the topic on "#test" to "New topic".

TOPIC #test :                   ; Clearing the topic on "#test"

TOPIC #test                     ; Checking the topic for "#test"
*/
// topic #taraaa test

void Server::viewTopic(Client &client, std::string channelName, Channel &channel)
{
    std::string msg;
    if (channel.getIsRestrictedTopic() && !this->isOperatorInChannel(client, channel)) {
        msg = "484 " + client.getNickname() + " " + channelName + " :Cannot change topic for channel" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
    }
    if (channel.getName().empty()) {
        msg = "403 "  + client.getNickname() + " " + channelName + " :No such channel" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
    }
    if (channel.getTopic().empty()) {
        msg = "332 "  + client.getNickname() + " " + channelName + " :No topic is set" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
    }
    else {
        msg = "332 " + client.getNickname() + " " + channelName + " :" + channel.getTopic() + "\r\n";
        this->sendMessageToClient(msg, client);
    }
}

void Server::handleTopicCommand(Client &client) {

    std::string msg = "";
    std::string Topic = "";
    std::string channelName = client.getParameters().size() >= 1 ? client.getParameters()[0] : "";
    Channel &channel = this->getChannelRefByName(channelName);

    if (!channel.isUserInChannel(client)) {
        msg = "442 " + client.getNickname() + " " + channelName + " :You're not on that channel" + "\r\n";
        this->sendMessageToClient(msg, client);
        return ;
    }
    if (client.getParameters().size() == 1) {
        viewTopic(client, channelName, channel);
        return;
    }
    else if (client.getParameters().size() == 0)
    {
        msg = "461 " + client.getNickname() + " TOPIC :Not enough parameters" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
    }
    else {
        if (client.getTrailing().empty() && client.getParameters().size() == 2) {
            Topic = client.getParameters()[1];
        }
        else if (!client.getTrailing().empty() && client.getParameters().size() == 2) {
            Topic = client.getTrailing();
        }
        channel.setTopic(Topic);
        msg = "332 " + client.getNickname() + " " + channelName + " :" + channel.getTopic() + "\r\n";
        this->sendMessageToClient(msg, client);
    }
}