# include "../HEADERS/channel.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/server.hpp"


Channel::Channel(std::string name)
{
    this->name = name;
    this->topic = "";
    this->isInviteOnly = false;
    this->isRestrictedTopic = false;
    this->limit = -1;
    this->key = "";
    this->isProtected = false;
    this->isLimited = false;
    this->users = std::vector<Client>();
    this->fdToUser = std::map<int, Client>();
    this->ejectedClients = std::map<int, Client>();
    this->ejectedfds = std::vector<int>();
    this->operators = std::vector<Client>();
    this->fdToOperator = std::map<int, Client>();
    this->creatorNickname = "";
    this->creationTime = Server::getCurrentTime();
    this->topicTime = Server::getCurrentTime();
    this->topicSetterNickname = "";
}

Channel::Channel(std::string name, std::string password)
{
    this->name = name;
    this->topic = "";
    this->isInviteOnly = false;
    this->isRestrictedTopic = false;
    this->limit = -1;
    this->key = password;
    this->isProtected = false;
    this->isLimited = false;
    this->users = std::vector<Client>();
    this->fdToUser = std::map<int, Client>();
    this->ejectedClients = std::map<int, Client>();
    this->ejectedfds = std::vector<int>();
    this->operators = std::vector<Client>();
    this->fdToOperator = std::map<int, Client>();
    this->creatorNickname = "";
    this->creationTime = Server::getCurrentTime();
    this->topicTime = Server::getCurrentTime();
    this->topicSetterNickname = "";
}

Channel::Channel(std::string name, std::string password, std::string topic)
{
    this->name = name;
    this->topic = topic;
    this->isInviteOnly = false;
    this->isRestrictedTopic = false;
    this->limit = -1;
    this->key = password;
    this->isProtected = false;
    this->isLimited = false;
    this->users = std::vector<Client>();
    this->fdToUser = std::map<int, Client>();
    this->ejectedClients = std::map<int, Client>();
    this->ejectedfds = std::vector<int>();
    this->operators = std::vector<Client>();
    this->fdToOperator = std::map<int, Client>();
    this->creatorNickname = "";
    this->creationTime = Server::getCurrentTime();
    this->topicTime = Server::getCurrentTime();
    this->topicSetterNickname = "";
}

Channel::Channel()
{
    this->name = "";
    this->topic = "";
    this->isInviteOnly = false;
    this->isRestrictedTopic = false;
    this->limit = -1;
    this->key = "";
    this->isProtected = false;
    this->isLimited = false;
    this->users = std::vector<Client>();
    this->fdToUser = std::map<int, Client>();
    this->ejectedClients = std::map<int, Client>();
    this->ejectedfds = std::vector<int>();
    this->operators = std::vector<Client>();
    this->fdToOperator = std::map<int, Client>();
    this->creatorNickname = "";
    this->creationTime = Server::getCurrentTime();
    this->topicTime = Server::getCurrentTime();
    this->topicSetterNickname = "";
}

Channel::~Channel()
{
}

//methods

bool Channel::isUserInChannel(Client client)
{
    if (fdToUser.find(client.getSocketfd()) != fdToUser.end())
        return true;
    else
        return false;
}

void Channel::addUser(Client client)
{
    this->users.push_back(client);
    this->fdToUser.insert(std::pair<int, Client>(client.getSocketfd(), client));
}

void Channel::addOperator(Client client)
{
    this->operators.push_back(client);
    this->fdToOperator.insert(std::pair<int, Client>(client.getSocketfd(), client));
}

void Channel::removeUser(Client client)
{
    // this is an iterator to the element in the vector that has the same socketfd as the client
    std::vector<Client>::iterator it = std::find(this->users.begin(), this->users.end(), client);
    if (it != this->users.end()) {
        this->users.erase(it);
        this->ejectedfds.push_back(client.getSocketfd());
        this->ejectedClients.insert(std::pair<int, Client>(client.getSocketfd(), client));
    }
    // this is an iterator to the element in the map that has the same socketfd as the client
    std::map<int, Client>::iterator it2 = this->fdToUser.find(client.getSocketfd());
    if (it2 != this->fdToUser.end()) {
        this->fdToUser.erase(it2);
        this->ejectedfds.push_back(client.getSocketfd());
        this->ejectedClients.insert(std::pair<int, Client>(client.getSocketfd(), client));
    }
}

void Channel::removeOperator(Client client)
{
    // this is an iterator to the element in the vector that has the same socketfd as the client
    std::vector<Client>::iterator it = std::find(this->operators.begin(), this->operators.end(), client);
    if (it != this->operators.end()) {
        this->operators.erase(it);
    }
    // this is an iterator to the element in the map that has the same socketfd as the client
    std::map<int, Client>::iterator it2 = this->fdToOperator.find(client.getSocketfd());
    if (it2 != this->fdToOperator.end()) {
        this->fdToOperator.erase(it2);
    }
}

// getters  

std::string Channel::getName()
{
    return this->name;
}

std::string Channel::getTopic()
{
    return this->topic;
}

bool Channel::getIsInviteOnly()
{
    return this->isInviteOnly;
}

bool Channel::getIsRestrictedTopic()
{
    return this->isRestrictedTopic;
}

int Channel::getLimit()
{
    return this->limit;
}

std::string Channel::getKey()
{
    return this->key;
}

bool Channel::getIsProtected()
{
    return this->isProtected;
}

bool Channel::getIsLimited()
{
    return this->isLimited;
}

std::vector<Client> Channel::getUsers()
{
    return this->users;
}

std::map<int, Client> Channel::getUsersMap()
{
    return this->fdToUser;
}

std::vector<Client> Channel::getOperators()
{
    return this->operators;
}

std::map<int, Client> Channel::getOperatorsMap()
{
    return this->fdToOperator;
}

std::map<int, Client> Channel::getEjectedClients()
{
    return this->ejectedClients;
}

std::vector<int> Channel::getEjectedfds()
{
    return this->ejectedfds;
}

std::string Channel::getCreatorNickname()
{
    return this->creatorNickname;
}

std::string Channel::getCreationTime()
{
    return this->creationTime;
}

std::string Channel::getTopicTime()
{
    return this->topicTime;
}

std::string Channel::getTopicSetterNickname()
{
    return this->topicSetterNickname;
}

// setters
void Channel::setName(std::string name)
{
    this->name = name;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
    this->topicTime = Server::getCurrentTime();
}

void Channel::setIsInviteOnly(bool isInviteOnly)
{
    this->isInviteOnly = isInviteOnly;
}

void Channel::setIsRestrictedTopic(bool isRestrictedTopic)
{
    this->isRestrictedTopic = isRestrictedTopic;
}

void Channel::setLimit(int limit)
{
    this->limit = limit;
}

void Channel::setKey(std::string key)
{
    this->key = key;
}

void Channel::setIsProtected(bool isProtected)
{
    this->isProtected = isProtected;
}

void Channel::setIsLimited(bool isLimited)
{
    this->isLimited = isLimited;
}

void Channel::setUsers(std::vector<Client> users)
{
    this->users = users;
}

void Channel::setUsersMap(std::map<int, Client> usersMap)
{
    this->fdToUser = usersMap;
}

void Channel::setOperators(std::vector<Client> operators)
{
    this->operators = operators;
}

void Channel::setOperatorsMap(std::map<int, Client> operatorsMap)
{
    this->fdToOperator = operatorsMap;
}

void Channel::setEjectedClients(std::map<int, Client> ejectedClients)
{
    this->ejectedClients = ejectedClients;
}

void Channel::setEjectedfds(std::vector<int> ejectedfds)
{
    this->ejectedfds = ejectedfds;
}

void Channel::setCreatorNickname(std::string creatorNickname)
{
    this->creatorNickname = creatorNickname;
}

void Channel::setCreationTime(std::string creationTime)
{
    this->creationTime = creationTime;
}

void Channel::setTopicTime(std::string topicTime)
{
    this->topicTime = topicTime;
}

void Channel::setTopicSetterNickname(std::string topicSetterNickname)
{
    this->topicSetterNickname = topicSetterNickname;
}