# pragma once
# include "ircUser.hpp"
# include "channel.hpp"
# include "client.hpp"


class Client;

class Operator : public IrcUser
{
    private : 
        // none yet, we'll add them later if needed
    
    public :
        Operator(int socketfd);
        ~Operator();

        // methods specific to operator, we'll add them later
        void kickClient(Client client); // KICK
        void inviteClient(Channel channel, Client client); // INVITE
        void setChannelTopic(Channel channel, std::string topic); // TOPIC
        void viewChannelTopic(Channel channel); // TOPIC
        void setChannelMode(Channel channel, std::string mode); // MODE
        void setInviteOnly(Channel channel); // MODE
        void removeInviteOnly(Channel channel); // MODE
        void setRestrictedTopic(Channel channel); // MODE
        void removeRestrictedTopic(Channel channel); // MODE
        void setChannelKey(Channel channel, std::string key); // MODE
        void removeChannelKey(Channel channel);  // MODE
        void giveOperatorPrivileges(Channel channel, Client client); // MODE
        void removeOperatorPrivileges(Channel channel, Client client); // MODE
        void setChannelLimit(Channel channel, int limit); // MODE
        void removeChannelLimit(Channel channel); // MODE
};