# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/message.hpp"
# include "../HEADERS/channel.hpp"
# include "../HEADERS/operator.hpp"


int main()
{
    int port = 1337;
    std::string password = "1337";

    Server server(port, password);
    server.startServer();
    return 0;
}
