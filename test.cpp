#include <iostream>
#include <sstream>
#include <vector>


void handleJoinCommand(const std::string& data) {
    std::istringstream str(data);
    std::string token;

    // Assuming the data format is {JOIN #channel1,#channel2,#channel3 password1,password2,password3\n}
    while (str >> token) {
        // Check if the token is JOIN
        if (token == "JOIN") {
            std::string channelPasswordString;
            std::getline(str, channelPasswordString);
            // std::cout << "string {" << channelPasswordString << "}" << std::endl;
            
            // Separate channels and passwords by ','
            std::istringstream channelPasswordStream(channelPasswordString);
            std::string channelPasswordPair; 

            // Vector to store the pairs of channel names and passwords
            std::vector<std::pair<std::string, std::string>> channelPasswordPairs;

            // Extract channel-password pairs
            while (std::getline(channelPasswordStream, channelPasswordPair, ',')) {
                std::cout << "the pairstring: {" << channelPasswordPair << "}" << std::endl;
                std::istringstream channelPasswordPairStream(channelPasswordPair);
                std::string channelName, password;
                if (std::getline(channelPasswordPairStream, channelName, ' ')) {
                    channelName.erase(0, channelName.find_first_not_of(" \n\r\t"));
                    channelName.erase(channelName.find_last_not_of(" \n\r\t") + 1);
                }
                els eif (std::getline(channelPasswordPairStream, password)) {
                    password.erase(0, password.find_first_not_of(" \n\r\t"));
                    password.erase(password.find_last_not_of(" \n\r\t") + 1);
                }
                channelPasswordPairs.emplace_back(channelName, password);
            }
            
            // Process channel names and passwords
            for (const auto& pair : channelPasswordPairs) {
                const std::string& channelName = pair.first;
                const std::string& password = pair.second;

                // Handle each channel name and password
                std::cout << "Channel: {" << channelName << "}, Password: {" << password << "}" << std::endl;
                // Your logic here
            }
        }
    }
}

int main() {
    // Test with different JOIN commands
    std::string data1 = "JOIN #music,#film, #data\n"; // Empty passwords for all channels
    std::string data2 = "JOIN #music,#film musicKey\n"; // Password only for #music
    std::string data3 = "JOIN #music,#film,#data musicKey,filmKey,dataKey\n"; // Different passwords for each channel

    std::cout << "Handling JOIN command 1:" << std::endl;
    handleJoinCommand(data1);
    std::cout << std::endl;

    std::cout << "Handling JOIN command 2:" << std::endl;
    handleJoinCommand(data2);
    std::cout << std::endl;

    std::cout << "Handling JOIN command 3:" << std::endl;
    handleJoinCommand(data3);
    std::cout << std::endl;

    return 0;
}

