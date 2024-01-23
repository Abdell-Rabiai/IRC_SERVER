# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/client.hpp"


Operator::Operator(int socketfd) {
    this->socketfd = socketfd;
    this->nickname = "";
    this->username = "";
    this->isOperator = true;
    this->isAuthenticated = false;
}

Operator::~Operator() {

}

void Operator::kickClient(Client client) { // KICK
    // TODO
}

void Operator::inviteClient(Channel channel, Client client) { // INVITE
    // TODO
}

void Operator::setChannelTopic(Channel channel, std::string topic) { // TOPIC
    // TODO
}

void Operator::viewChannelTopic(Channel channel) { // TOPIC
    // TODO
}

void Operator::setChannelMode(Channel channel, std::string mode) { // MODE
    // TODO
}

void Operator::setInviteOnly(Channel channel) { // MODE
    // TODO
}

void Operator::removeInviteOnly(Channel channel) { // MODE
    // TODO
}

void Operator::setRestrictedTopic(Channel channel) { // MODE
    // TODO
}

void Operator::removeRestrictedTopic(Channel channel) { // MODE
    // TODO
}   

void Operator::setChannelKey(Channel channel, std::string key) { // MODE
    // TODO
}

void Operator::removeChannelKey(Channel channel) {  // MODE
    // TODO
}

void Operator::giveOperatorPrivileges(Channel channel, Client client) { // MODE
    // TODO
}

void Operator::removeOperatorPrivileges(Channel channel, Client client) { // MODE
    // TODO
}

void Operator::setChannelLimit(Channel channel, int limit) { // MODE
    // TODO
}

void Operator::removeChannelLimit(Channel channel) { // MODE
    // TODO
}

