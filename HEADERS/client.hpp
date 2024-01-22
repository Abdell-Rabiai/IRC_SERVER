# ifndef _CLIENT_HPP_
# define _CLIENT_HPP_
# include "all_headers.hpp"

# include "channel.hpp"
# include "message.hpp"
# include "operator.hpp"
# include "server.hpp"
# include "ircUser.hpp"

class Client : public IrcUser
{
    private :
            // none yet, we'll add them later if needed

    public :
        Client(int socketfd);
        ~Client();

        // methods specific to client, we'll add them later
};

# endif // _CLIENT_HPP_