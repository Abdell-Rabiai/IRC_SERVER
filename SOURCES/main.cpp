# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/message.hpp"
# include "../HEADERS/channel.hpp"
# include "../HEADERS/operator.hpp"


int main()
{
    int port = 6667;
    std::string password = "pass";

    Server server(port, password);
    if (!server.startServer()) {
        std::cout << "Error starting server" << std::endl;
        return 1;
    }
    return 0;
}
