# include "../HEADERS/all_headers.hpp"
# include "../HEADERS/server.hpp"
# include "../HEADERS/client.hpp"
# include "../HEADERS/channel.hpp"

/*
!date
!quiz 
*/

void Server::launchBot(Client &client) {
    std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName() + " PRIVMSG " + client.getNickname() + " :" + "Bot is ready :)" + "\n";
    this->sendMessageToClient(msg, client);
    msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName() + " PRIVMSG " + client.getNickname() + " :" + "commands: " + "\n";
    this->sendMessageToClient(msg, client);
    msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName() + " PRIVMSG " + client.getNickname() + " :" + "!date: shows todays date" + "\n";
    this->sendMessageToClient(msg, client);
    msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName() + " PRIVMSG " + client.getNickname() + " :" + "!quiz: launches a quiz game" + "\n";
    this->sendMessageToClient(msg, client);
    msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName() + " PRIVMSG " + client.getNickname() + " :" + "!weather: shows the weather" + "\n";
    this->sendMessageToClient(msg, client);
    msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName() + " PRIVMSG " + client.getNickname() + " :" + "!quit: quits the bot" + "\n";
    this->sendMessageToClient(msg, client);
    char *python3 = "python3";
    char *args[] = {"python3", "/Users/ahmaymou/Desktop/IRC_SERVER/SOURCES/bot.py", "1337", NULL};
    pid_t child = fork();
    if (child == 0) {
        execvp(python3, args);
    }
    else {
        std::cout << "Bot launched\n" << std::endl;
    }
}