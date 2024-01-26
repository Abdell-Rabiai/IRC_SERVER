# include "HEADERS/all_headers.hpp"
# include "HEADERS/server.hpp"
# include "HEADERS/client.hpp"
# include "HEADERS/channel.hpp"
# include "HEADERS/socket.hpp"


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./server <port> <password>" << std::endl;
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);
    std::string password = argv[2];

    Server server(port, password);
    server.startServer();
    return 0;
}
