# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/client.hpp"

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
}

Channel::Channel(std::string name, std::string topic)
{
    this->name = name;
    this->topic = topic;
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
}

Channel::~Channel()
{
}

void Channel::addUser(Client client)
{
    this->users.push_back(client);
    this->fdToUser.insert(std::pair<int, Client>(client.getSocketfd(), client));
}


