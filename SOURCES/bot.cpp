# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"

/*
!date
!quiz 
*/

void Server::launchBot(Client &client) {
    char *python3 = "python3";
    char *args[] = {"python3", "/Users/ahmaymou/Desktop/IRC_SERVER/SOURCES/bot.py", "1234", NULL};
    pid_t child = fork();
    if (child == 0) {
        execvp(python3, args);
    }
    else {
        std::cout << "Bot launched\n" << std::endl;
    }
}