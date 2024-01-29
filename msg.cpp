PASS 1337
NICK tabi3a
USER arabiai * 0    :abdelouahed

PASS 1337
NICK moha
USER momihamm * 0    :mohammed

PASS 1337
NICK black
USER ahmaymou * 0    :abderhamn hamama


void Channel::notifyUsers(int fd){
    std::string notifyusers = ":" + this->_joinedUsers.find(fd)->second.getNICKNAME() \
    + "!" + this->_joinedUsers.find(fd)->second.getUSERNAME() + "@" +  this->_HostName + " JOIN " \
    + this->getChannelName() + "\r\n";
    for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
        writeMessageToClient_fd(i->first, notifyusers);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void Channel::welcomeUser(int fd){

        std::string nickName = this->_joinedUsers.find(fd)->second.getNICKNAME();
        std::string membersList;
        for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
            if (this->_admins.find(i->first) != this->_admins.end()){
                membersList += "@" + i->second.getNICKNAME() + " ";
            }else
                membersList += i->second.getNICKNAME() + " ";
        }
        RPL_NAMREPLY(fd, nickName,  this->getChannelName(), membersList);
        RPL_ENDOFNAMES(fd, nickName,  this->getChannelName());

}