# ifndef __MESSAGE_H__
# define __MESSAGE_H__

# include "all_headers.hpp"

# include "client.hpp"
# include "operator.hpp"
# include "server.hpp"
# include "channel.hpp"
class Client;

enum MessageType {
    JOIN, // 0
    LEAVE, // 1
    PRIVATE_MESSAGE, // 2
    OPERATOR_COMMAND // 3
};

class Message
{
    private:
        Client *sender;
        std::string content;
        MessageType type;

    public:
        Message(Client sender, std::string content);
        ~Message();

        // getters
        Client getSender();
        std::string getContent();
        MessageType getType();
        // setters
        void setSender(Client sender);
        void setContent(std::string content);
        void setType(MessageType type);
};

# endif // __MESSAGE_H__