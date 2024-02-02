# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"

/*
    Command: MODE
  Parameters: <target> [<modestring> [<mode arguments>...]]

  Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
               [<ban mask>]


∗ MODE - Change the channel’s mode:
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privileges
· l: Set/remove the user limit to channel

*/

void Server::do_modes(Client &client, Channel &channel, std::vector<std::string> modes) {
  std::string msg = "";
  for (size_t i = 0; i < modes.size(); i++) {

    if (!this->isOperatorInChannel(client, channel) || !channel.isUserInChannel(client)) {
      msg = "482 " + client.getNickname() + " " + channel.getName() + " :You're not channel operator" + "\r\n";
      this->sendMessageToClient(msg, client);
      return;
    }
    if (modes[i] == "+i") {
      channel.setInviteOnly();
      msg = "324 " + client.getNickname() + " " + channel.getName() + " +i\r\n";
    }
    else if (modes[i] == "-i") {
      channel.removeInviteOnly();
      msg = "324 " + client.getNickname() + " " + channel.getName() + " -i\r\n";
    }
    else if (modes[i] == "+t") {
      channel.setRestrictedTopic();
      msg = "324 " + client.getNickname() + " " + channel.getName() + " +t\r\n";
    }
    else if (modes[i] == "-t") {
      channel.removeRestrictedTopic();
      msg = "324 " + client.getNickname() + " " + channel.getName() + " -t\r\n";
    }
    else if (modes[i] == "+k") {
      channel.setKey(modes[i + 1]);
      msg = "324 " + client.getNickname() + " " + channel.getName() + " +k " + modes[i + 1] + "\r\n";
      i++;
    }
    else if (modes[i] == "-k") {
      channel.removeKey();
      msg = "324 " + client.getNickname() + " " + channel.getName() + " -k\r\n";
    }
    else if (modes[i] == "+o") {
      Client operatorClient = this->getClientByNickname(modes[i + 1]);;
      channel.addOperator(operatorClient);
      i++;
      msg = "324 " + client.getNickname() + " " + channel.getName() + " +o " + operatorClient.getNickname() + "\r\n";
    }
    else if (modes[i] == "-o") {
      Client operatorClient = this->getClientByNickname(modes[i + 1]);
      if (operatorClient.getNickname() == channel.getCreatorNickname()){
        msg = "324 " + client.getNickname() + " " + channel.getName() + " :You Can't remove privs of the Creator!" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
      }
      if (!channel.isUserInChannel(operatorClient)) {
        msg = "441 " + client.getNickname() + " " + operatorClient.getNickname() + " " + channel.getName() + " :They aren't on that channel" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
      }
      channel.removeOperator(operatorClient);
      i++;
      msg = "324 " + client.getNickname() + " " + channel.getName() + " -o " + operatorClient.getNickname() + "\r\n";
    }
    else if (modes[i] == "+l") {
      channel.setLimit(std::stoi(modes[i + 1]));
      msg = "324 " + client.getNickname() + " " + channel.getName() + " +l " + modes[i + 1] + "\r\n";
      i++;
    }
    else if (modes[i] == "-l") {
      channel.removeLimit();
      msg = "324 " + client.getNickname() + " " + channel.getName() + " -l\r\n";
    }
    this->sendMessageToClient(msg, client);
  }
}

void Server::handleModeCommand(Client &client) {
    std::string msg = "";
    std::string channelName = client.getParameters().size() >= 1 ? client.getParameters()[0] : "";
    std::vector<std::string> modes;
    if (client.getParameters().size() < 2) {
        msg = "461 " + client.getNickname() + " MODE :Not enough parameters" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
    }
    if (!this->isChannelExist(channelName)) {
        msg = "403 " + client.getNickname() + " " + channelName + " :No such channel" + "\r\n";
        this->sendMessageToClient(msg, client);
        return;
    }
    // if (!this->isUserInChannel(client, channelName)) {
    //     msg = "442 " + client.getNickname() + " " + channelName + " :You're not on that channel" + "\r\n";
    //     this->sendMessageToClient(msg, client);
    //     return;
    // }
    for (size_t i = 1; i < client.getParameters().size(); i++)
        modes.push_back(client.getParameters()[i]);
    Channel &channel = this->getChannelRefByName(channelName);
    do_modes(client, channel, modes);
}