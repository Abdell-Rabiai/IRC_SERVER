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
    char *args[] = {"python3", "bot.py", "1337", "&", NULL};
    execv(python3, args);
    // system("python3 SOURCES/bot.py 1337 &");
}

