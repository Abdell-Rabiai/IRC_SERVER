# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"

// INVITE Wiz #foo_bar    ; Invite Wiz to #foo_bar

void Server::inviteToChannel(Client &inviter, Channel &channel, Client &invitedClient) {
    
        std::string msg = "";
        std::string invitedNickname = invitedClient.getNickname();
        // msg = "341 " + inviter.getNickname() + " " + invitedNickname + " " + channel.getName() + " :Inviting you to channel" + "\r\n";
        // this->sendMessageToClient(msg, invitedClient);
    
        if (!this->isChannelExist(channel.getName()) || invitedNickname.empty()) {
            // std::cout << "{{{{" << this->isChannelExist(channel.getName()) << "----" << channel.getName() << "---" << invitedNickname << "}}}}" << std::endl;
            msg = "401 " + inviter.getNickname() + " " + invitedNickname + " :No such nick/channel" + "\r\n";
            this->sendMessageToClient(msg, inviter);
            return;
        }
        if (channel.getIsInviteOnly() && !this->isOperatorInChannel(inviter, channel)) {
            msg = "482 " + inviter.getNickname() + " " + channel.getName() + " :You're not channel operator" + "\r\n";
            this->sendMessageToClient(msg, inviter);
            return;
        }
        if (channel.getIsLimited() && channel.getUsers().size() >= (size_t)channel.getLimit() && !this->isOperatorInChannel(inviter, channel)) {
            msg = "405 " + inviter.getNickname() + " " + channel.getName() + " :You have joined too many channels" + "\r\n";
            this->sendMessageToClient(msg, inviter);
            return;
        }
        // if (this->isProtected && this->key != invitedClient.getPassword()) {
        //     msg = "475 " + inviter.getNickname() + " " + channel.getName() + " :Cannot join channel (+k)" + "\r\n";
        //     this->sendMessageToClient(msg, inviter);
        //     return;
        // }
        if (channel.getIsLimited() && this->isOperatorInChannel(inviter, channel)) {
            msg = "473 " + inviter.getNickname() + " " + channel.getName() + " :" + "Cannot join channel (+l)" + "\r\n";
            this->sendMessageToClient(msg, invitedClient);
            return;
        }
        if (channel.getIsInviteOnly() && this->isOperatorInChannel(inviter, channel)) {
            msg = "341 " + inviter.getNickname() + " " + invitedNickname + " " + channel.getName() + " :Inviting you to channel" + "\r\n";
            this->sendMessageToClient(msg, invitedClient);
            return;
        }
        if (channel.isUserInChannel(invitedClient)) {
            msg = "443 " + inviter.getNickname() + " " + invitedNickname + " " + channel.getName() + " :is already on channel" + "\r\n";
            this->sendMessageToClient(msg, inviter);
            return;
        }
        msg = "341 " + inviter.getNickname() + " " + invitedNickname + " " + channel.getName() + " :Inviting you to channel" + "\r\n";
        channel.addUser(invitedClient);
        this->sendMessageToClient(msg, invitedClient);
}

void Server::handleInviteCommand(Client &client) {

    std::string msg = "";
    std::string Username = client.getParameters().size() >= 1 ? client.getParameters()[0] : "";
    Client InvitedClient = this->getClientByNickname(Username);
    std::string channelName = client.getParameters().size() > 1 ? client.getParameters()[1] : "";
    Channel &channel = this->getChannelRefByName(channelName);

    if (client.getParameters().size() == 2) {
        this->inviteToChannel(client, channel, InvitedClient);
        return ;
    }
    else if (client.getParameters().size() < 2)
    {
        msg = "461 " + client.getNickname() + " INVITE :Not enough parameters" + "\r\n";
        this->sendMessageToClient(msg, client);
        return ;
    }
}