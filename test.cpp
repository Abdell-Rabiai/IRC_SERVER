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

void parseIrcMessage(std::string buffer) {
	std::string command;
	std::vector<std::string> parameters;
	std::string trailing;

	int i = buffer.find_first_of(' ');
	if (i != -1) {
		command = buffer.substr(0, i);
		buffer = buffer.substr(i + 1, buffer.length() - i - 1);
	}
	else {
		command = buffer;
		buffer = "";
	}

	i = buffer.find_first_of(':');
	if (i != -1) {
		trailing = buffer.substr(i + 1, buffer.length() - i - 1);
		buffer = buffer.substr(0, i - 1);
	}

	while (buffer.length() > 0) {
		int j = buffer.find_first_of(':');
		if (j != -1)
			break ;
		i = buffer.find_first_of(' ');
		if (i == -1) {
			parameters.push_back(buffer);
			buffer = "";
		}
		else {
			std::string str = buffer.substr(0, i);
			if (str.length() > 0){
				parameters.push_back(str);
			}
			buffer = buffer.substr(i + 1, buffer.length() - i - 1);
		}
	}
	if (trailing.length() > 0) {
		parameters.push_back(trailing);
	}

    std::cout << "command: {" << command << "}" << std::endl;
    std::cout << "parameters: ";
    for (int i = 0; i < parameters.size(); i++) {
        std::cout << "[" << parameters[i] << "] ";
    }
    std::cout << std::endl;
    std::cout << "trailing: (" << trailing << ")"  << std::endl;
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


// // PASS 1337
// // NICK black
// // USER ahmaymou * 0 :abderhman hmaymou

// // PASS 1337
// // NICK moha
// // USER momihamm * 0 :mohammed momimann

// start = time.time()

// <<<<<<< HEAD
//         # if (time.time() - start > 10):
//         #     msg = "privmsg " + recipient + " :Time's up! \n"
//         #     io.sendall(msg.encode('utf-8'))
//         #     i += 1
//         #     print("Time's up!")
// =======
// join #canal1,#canal2,#canal3
// >>>>>>> 8e94704625d81ec8b37bce21d1ed86fddbb7ec7d
