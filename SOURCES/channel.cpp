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
    this->operators = std::vector<Client>();
    this->fdToOperator = std::map<int, Client>();
    this->ejectedClients = std::map<int, Client>();
    this->ejectedfds = std::vector<int>();
}

Channel::~Channel()
{
}


