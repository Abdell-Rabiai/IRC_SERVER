# pragma once
# include "all_headers.hpp"
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
        std::vector<int> usersfds;
        std::map<int, Client> fdToUser; // key: socketfd, value Client
        std::map<int, Client> ejectedClients; // key: socketfd, value Client
        std::vector<int> ejectedfds;
        std::vector<Client> operators;
        std::map<int, Client> fdToOperator; // key: socketfd, value Client

        std::string creatorNickname; // the nickname of the client who created the channel
        std::vector<std::string> OperatorNicknames;
        std::string creationTime;
        std::string topicTime;
        std::string topicSetterNickname; // the nickname of the client who set the topic

    public:
        Channel(std::string name);
        Channel(std::string name, std::string password);
        Channel(std::string name, std::string password, std::string topic);
        Channel();
        ~Channel();

        void addUser(Client client);
        bool isUserInChannel(Client client);
        void removeUser(Client client);
        void addOperator(Client client);
        void removeOperator(Client client);
        void setInviteOnly();
        void removeInviteOnly();
        void setRestrictedTopic();
        void removeRestrictedTopic();
        void removeKey();
        void removeLimit();
        bool isOperatorInChannel(Client client);


        // getters
        std::string getName();
        std::string getTopic();

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
        std::vector<int> getEjectedfds();
        std::string getTopicSetterNickname();
        std::string getCreationTime();
        std::string getTopicTime();
        std::string getCreatorNickname();



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
        void setEjectedClients(std::map<int, Client> ejectedClients);
        void setEjectedfds(std::vector<int> ejectedfds);
        void setCreatorNickname(std::string creatorNickname); // dont forget to set creator in the command JOIN
        void setCreationTime(std::string creationTime); // dont forget to set creation time in the command JOIN
        void setTopicTime(std::string topicTime); // dont forget to set topic time in the command TOPIC
        void setTopicSetterNickname(std::string topicSetterNickname); // dont forget to set topic setter in the command TOPIC
};
