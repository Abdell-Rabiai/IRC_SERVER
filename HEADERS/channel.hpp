# pragma once
# include "all_headers.hpp"

# include "client.hpp"
# include "message.hpp"

class Client;

class Channel
{
    private:
        std::string name;
        std::string topic;
        bool isInviteOnly;
        bool isRestrictedTopic;
        int limit;
        std::string key;
        bool isProtected;
        bool isLimited;
        std::vector<Client> users;
        std::map<int, Client> usersMap;
        std::vector<Client> operators;
        std::map<int, Client> operatorsMap; // key: socketfd, value Opeartor
        std::map<int, Client> ejectedClients; // key: socketfd, value Client

    public:
        Channel(std::string name);
        ~Channel();

        void addUser(Client client);
        void removeUser(Client client);
        void addOperator(Client client);
        void removeOperator(Client client);
        void broadcastMessage(Message message);
        void setInviteOnly();
        void removeInviteOnly();
        void setRestrictedTopic();
        void removeRestrictedTopic();
        void removeKey();
        void removeLimit();
        bool isUserInChannel(Client client);
        bool isOperatorInChannel(Client client);


        // getters
        const std::string & getName();
        const std::string &getTopic();
        bool getIsInviteOnly();
        bool getIsRestrictedTopic();
        int getLimit();
        std::string getKey();
        bool getIsProtected();
        bool getIsLimited();
        std::vector<Client> getUsers();
        std::map<int, Client> getUsersMap();
        std::vector<Client> getOperators();
        std::map<int, Client> getOperatorsMap();
        std::map<int, Client> getEjectedClients();
        // setters
        void setName(std::string name);
        void setTopic(std::string topic);
        void setIsInviteOnly(bool isInviteOnly);
        void setIsRestrictedTopic(bool isRestrictedTopic);
        void setLimit(int limit);
        void setKey(std::string key);
        void setIsProtected(bool isProtected);
        void setIsLimited(bool isLimited);
        void setUsers(std::vector<Client> users);
        void setUsersMap(std::map<int, Client> usersMap);
        void setOperators(std::vector<Client> operators);
        void setOperatorsMap(std::map<int, Client> operatorsMap);
};
