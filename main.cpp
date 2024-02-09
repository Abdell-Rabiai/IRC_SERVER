# include "HEADERS/all_headers.hpp"
# include "HEADERS/server.hpp"
# include "HEADERS/client.hpp"
# include "HEADERS/channel.hpp"
# include "HEADERS/socket.hpp"

bool checkPort(const char *port)
{
    for (size_t i = 0; i < strlen(port); i++) {
        if (isdigit(port[i]) == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    try {
        signal(SIGPIPE, SIG_IGN);
        if (argc != 3) {
            std::cerr << "Usage: ./server <port> <password>" << std::endl;
            return 1;
        }
        std::string password = argv[2];
        if (password.empty()) {
            std::cerr << "Password Empty" << std::endl;
            return 1;
        }
        if (checkPort(argv[1]) == false) {
            std::cerr << "Port Not Valid" << std::endl;
            return 1;
        }
        int port = atoi(argv[1]);
        if (port < 1024 || port > 65535) {
            std::cerr << "Port Not Valid" << std::endl;
            return 1;
        }
        Server server(port, password);
        server.startServer();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
