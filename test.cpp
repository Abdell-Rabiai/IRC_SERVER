#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string>  splitt(std::string str, std::string delimiter) {
	std::vector<std::string> result;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		result.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	result.push_back(str);
	return result;
}

void parseIrcMessage(std::string ircMessage) {
	std::string command;
	std::vector<std::string> parameters;
	std::string trailing;


    // clear existing data
    command = "";
    parameters.clear();
    trailing = "";
    // remove if there is a \n at the end
    if (ircMessage[ircMessage.length() - 1] == '\n') {
        ircMessage = ircMessage.substr(0, ircMessage.length() - 1);
    }
	int i = ircMessage.find_first_of(' ');
	command = ircMessage.substr(0, i);
	ircMessage = ircMessage.substr(i + 1, ircMessage.length() - i - 1);

	i = ircMessage.find_first_of(':');
	if (i != -1) {
		trailing = ircMessage.substr(i + 1, ircMessage.length() - i - 1);
		ircMessage = ircMessage.substr(0, i - 1);
	}

    // parse parameters and push them to the vector/ don't push the spaces 
	while (ircMessage.length() > 0) {
        i = ircMessage.find_first_of(' ');
        if (i == -1) {
            if (ircMessage.find_first_of('\n') != -1) {
                ircMessage = ircMessage.substr(0, ircMessage.find_first_of('\n'));
            }
            parameters.push_back(ircMessage);
            ircMessage = "";
        }
        else {
            std::string str = ircMessage.substr(0, i);
            if (str.length() > 0){
                parameters.push_back(str);
            }
            ircMessage = ircMessage.substr(i + 1, ircMessage.length() - i - 1);
        }
	}
    // push also the trailing part
    parameters.push_back(trailing);
    std::cout << "Command: " << command << std::endl;
    std::cout << "Parameters: ";
    for (auto parameter : parameters) {
        std::cout <<"{"<< parameter << "} ";
    }
    std::cout << std::endl;
    std::cout << "Trailing: " << trailing << std::endl;
}
int main() {
system("python3 SOURCES/bot.py 1337");
    // std::string ircMessage = "PASS 1337\nNICK tabi3a\nUSER arabiai * 0 :abdelouahed rabiai\n";
    // parseIrcMessage(ircMessage);
    // ircMessage = "PASS 1337\nNICK black\nUSER ahmaymou * 0 :abderhman hmaymou\n";
    // parseIrcMessage(ircMessage);
    // ircMessage = "PASS 1337\nNICK moha\nUSER momihamm * 0 :mohammed momimann\n";
    // parseIrcMessage(ircMessage);
    return 0;
}

// PASS 1337
// NICK tabi3a
// USER arabiai * 0 :abdelouahed rabiai

// PASS 1337
// NICK black
// USER ahmaymou * 0 :abderhman hmaymou

// PASS 1337
// NICK moha
// USER momihamm * 0 :mohammed momimann