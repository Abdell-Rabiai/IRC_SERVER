# include "../HEADERS/client.hpp"

Client::Client() {
	this->socketfd = -1;
	this->nickname = "";
	this->username = "";
	this->isOperator = false;
	this->isAuthenticated = false;
}

Client::Client(int socketfd) {
    this->socketfd = socketfd;
    this->nickname = "";
    this->username = "";
    this->isOperator = false;
    this->isAuthenticated = false;
}

Client::Client(int socketfd, std::string nickname, std::string username) {
    this->socketfd = socketfd;
    this->nickname = nickname;
    this->username = username;
    this->isOperator = false;
    this->isAuthenticated = false;
}

Client::~Client() {

}

// methods

void Client::sendMessageToClient(std::string message , Client client) {
    // send message to client
}

void Client::sendMessageToChannel(std::string message, Channel channel) {
    // send message to channel
}

void Client::joinChannel(Channel channel) {
    // join channel
}

void Client::authenticate() {
    // authenticate
}

void Client::becomeOperator() {
    // become operator
}

void Client::processReceivedData(std::string message) {
    // process received data
}

// methods specific to operator, we'll add them later
void Client::kickClient(Client client, Channel) { // KICK
    // kick client
}

void Client::inviteClient(Client client, Channel channel) { // INVITE
    // invite client
}

void Client::setChannelTopic(Channel channel, std::string topic) { // TOPIC
    // set channel topic
}

void Client::viewChannelTopic(Channel channel) { // TOPIC
    // view channel topic
}

void Client::setChannelMode(Channel channel, std::string mode) { // MODE
    // set channel mode
}

void Client::setInviteOnly(Channel channel) { // MODE
    // set invite only
}

void Client::removeInviteOnly(Channel channel) { // MODE
    // remove invite only
}

void Client::setRestrictedTopic(Channel channel) { // MODE
    // set restricted topic
}

void Client::removeRestrictedTopic(Channel channel) { // MODE
    // remove restricted topic
}

void Client::setChannelKey(Channel channel, std::string key) { // MODE
    // set channel key
}

void Client::removeChannelKey(Channel channel) {  // MODE
    // remove channel key
}

void Client::giveOperatorPrivileges(Channel channel, Client client) { // MODE
    // give operator privileges
}

void Client::removeOperatorPrivileges(Channel channel, Client client) { // MODE
    // remove operator privileges
}

void Client::setChannelLimit(Channel channel, int limit) { // MODE
    // set channel limit
}

void Client::removeChannelLimit(Channel channel) { // MODE
    // remove channel limit
}

// getters 
int Client::getSocketfd() {
	return this->socketfd;
}

std::string Client::getNickname() {
	return this->nickname;
}

std::string Client::getUsername() {
	return this->username;
}

bool Client::getIsOperator() {
	return this->isOperator;
}

bool Client::getIsAuthenticated() {
	return this->isAuthenticated;
}

std::vector<Channel> Client::getCurrentChannels() {
	return this->currentChannels;
}

std::map<std::string, Channel> Client::getNameToChannel() {
	return this->nameToChannel;
}

std::string Client::getHostName() {
    return this->hostname;
}

std::string Client::getPort() {
    return this->port;
}

// setters

void Client::setSocketfd(int socketfd) {
	this->socketfd = socketfd;
}

void Client::setNickname(std::string nickname) {
	this->nickname = nickname;
}

void Client::setUsername(std::string username) {
	this->username = username;
}

void Client::setIsOperator(bool isOperator) {
	this->isOperator = isOperator;
}

void Client::setIsAuthenticated(bool isAuthenticated) {
	this->isAuthenticated = isAuthenticated;
}

void Client::setCurrentChannels(std::vector<Channel> currentChannels) {
	this->currentChannels = currentChannels;
}

void Client::setNameToChannel(std::map<std::string, Channel> nameToChannel) {
	this->nameToChannel = nameToChannel;
}

void Client::setHostName(std::string hostname) {
    this->hostname = hostname;
}

void Client::setPort(std::string port) {
    this->port = port;
}

// methods, we'l add them later
