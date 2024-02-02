# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"

/*
!date
!quiz 
*/





void Server::launchBot(Client &client) {
    (void)client;
    int  ServerPort = this->getServerPort();
    std::stringstream ss;
    ss << ServerPort;
    char *port = strdup(ss.str().c_str());
    char *python3 = strdup("python3");
    char *path = strdup("/Users/arabiai/Desktop/IRC_SERVER/SOURCES/bot.py");

    char *args[] = {python3, path, port, NULL};
    pid_t child = fork();
    if (child == 0) {
        execvp(python3, args);
    }
    else {
        std::cout << "Bot launched\n" << std::endl;
    }
    free(python3);
    free(path);
}