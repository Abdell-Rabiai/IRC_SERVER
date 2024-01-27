# include "HEADERS/all_headers.hpp"
# include "HEADERS/server.hpp"
# include "HEADERS/client.hpp"
# include "HEADERS/channel.hpp"
# include "HEADERS/socket.hpp"


int main(int argc, char **argv)
{
    try {

        if (argc != 3)
        {
            std::cerr << "Usage: ./server <port> <password>" << std::endl;
            return 1;
        }
        int port = atoi(argv[1]);
        std::string password = argv[2];
        std::cout << password << std::endl;

        Server server(port, password);
        server.startServer();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
