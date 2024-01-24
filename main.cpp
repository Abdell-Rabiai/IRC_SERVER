# include "HEADERS/all_headers.hpp"
# include "HEADERS/server.hpp"
# include "HEADERS/client.hpp"
# include "HEADERS/channel.hpp"
# include "HEADERS/socket.hpp"


int main()
{
    int port = 1337;
    std::string password = "1337";

    Server server(port, password);
    server.startServer();
    return 0;
}
