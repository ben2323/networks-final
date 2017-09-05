
#include "User.h"

User::User(string userName, string ip, int port) {
	this->userName = userName;
	this->ip = ip;
	this->port = port;
	this->listeningPort = 0;
}

User::User(string userName, string ip, int port, int listeningPort) {
	this->userName = userName;
	this->ip = ip;
	this->port = port;
	this->listeningPort = listeningPort;
}

string User::getIP(){
	return this->ip;
}

string User::getUserName(){
	return this->userName;
}

int User::getPort(){
	return this->port;
}

void User::setListeningPort(int port){
	this->listeningPort = port;
}

int User::getListeningPort(){
	return this->listeningPort;
}

User::~User() {
}

