
#ifndef MAIN_CPP_
#define MAIN_CPP_

#include <iostream>
#include <string.h>
#include "MessengerClient.h"

using namespace std;

void printMessngerInstructions(){
	cout<<"---------------------CLI:---------------------"<<endl;
	cout <<	"connect <IP> - connect to the server in the given ip" << endl;
	cout <<	"list_users - print the user list from the server" << endl;
	cout <<	"list_connected_users - print the connected users list" << endl;
	cout <<	"login <user> <password> - login with the user and password" << endl;
	cout <<	"register <user> <password> - register the new user with the given password and login the user." << endl;
	cout <<	"open <username> - open a game session with a user" << endl;
	cout <<	"rand - start session with a random user" << endl;
	cout <<	"send <message> - send a game messages (s XY)" << endl;
	cout <<	"status - print the current status of the client (connected to “xyz”/not connected)" << endl;
	cout <<	"close_session - disconnect the open session" << endl;
	cout <<	"disconnect - disconnect from server" << endl;
	cout <<	"exit - close the app" << endl;
	cout <<	"scoreboard - scoreboard" << endl;
	cout<<"----------------------------------------------"<<endl;
	cout<<"----------------------------------------------"<<endl;
}

bool validateConnected(MessengerClient* messenger, bool reverse = false){
	if(!reverse){
		if(!messenger->isConnected()){
			cout<< "Please connect to a server first" << endl;
			return false;
		}
		return true;
	}
	if(messenger->isConnected()){
		cout<< "You already connected to a server" << endl;
		return false;
	}
	return true;
}

bool validateLoggedIn(MessengerClient* messenger, bool reverse = false){
	if(!reverse){
		if(!messenger->isLoggedIn()){
			cout << "you are not logged in, please login first!" << endl;
			return false;
		}
		return true;
	}
	if(messenger->isLoggedIn()){
		cout << "you are already logged in as: " << messenger->getCurrentUser()->getUserName() << endl;
		return false;
	}
	return true;
}

bool validateActiveSession(MessengerClient* messenger){
	if(!messenger->isActiveSession()){
		cout<<"You don't have active session!" << endl;
		return false;
	}
	return true;
}

void handleLogin(MessengerClient* messenger){
	string userName;
	string password;
	cin >> userName;
	cin >> password;
	if(!validateConnected(messenger) || !validateLoggedIn(messenger,true)) return;

	messenger->login(userName, password);
}

void handleRegister(MessengerClient* messenger){
	string userName;
	string password;
	cin >> userName;
	cin >> password;
	if(!validateConnected(messenger) || !validateLoggedIn(messenger, true)) return;

	messenger->registerUser(userName, password);
}

void printStatus(MessengerClient* messenger){
	string status = "STATUS: ";
	if(!messenger->isConnected()){
		status += "not connected to a server";
	} else{
		status += "connected";
		if(messenger->isLoggedIn()){
			status += (", logged in as: " + messenger->getCurrentUser()->getUserName());
			if(messenger->isActiveSession()){
				status += ", in active session with: ";
				if(messenger->isConnectedWithUser()){
					status += "USER - " + messenger->getActiveSessionUser()->getUserName();
				}
			}
		}
	}
	cout << status << endl;
}

bool handleCommand(string command, MessengerClient* messenger){
	if(command == "connect"){
		string ip;
		cin >> ip;
		if(validateConnected(messenger, true))
			messenger->connect(ip);
	}else if(command == "list_users"){
		if(validateConnected(messenger))
			messenger->printAllUsers();
	}else if(command == "list_connected_users"){
		if(validateConnected(messenger))
			messenger->printConnectedUsers();
	}else if(command == "login"){
		handleLogin(messenger);
	}else if(command == "register"){
		handleRegister(messenger);
	}else if(command == "open"){
		string userName;
		cin >> userName;
		if(validateConnected(messenger) && validateLoggedIn(messenger))
			messenger->connectUser(userName);
	}else if(command == "rand"){
		if(validateConnected(messenger) && validateLoggedIn(messenger))
					messenger->connectRandUser();
	}else if(command == "send"){
		string message;
		do
		{
		getline(cin, message);
		}while(message.length()!=3);
		if(validateConnected(messenger) && validateLoggedIn(messenger) && validateActiveSession(messenger))
			messenger->send(message);
	}else if(command == "status"){
		printStatus(messenger);
	}else if(command == "close_session"){
		if(validateActiveSession(messenger))
			messenger->closeActiveSession();
	}else if(command == "y"){
		if(validateActiveSession(messenger))
			messenger->closeActiveSession();
	}else if(command == "disconnect"){
		if(validateConnected(messenger))
			messenger->disconnect();
	}else if(command == "exit"){
		return false;
	}else if(command == "soreboard"){
				if(validateConnected(messenger))
					messenger->scoreBoard();
	}else{
		cout<<"wrong input, please use the cli guide"<<endl;
		printMessngerInstructions();
	}
	return true;
}

int main(){
	cout<<"----------Welcome To Messenger Client---------"<<endl;
	printMessngerInstructions();
	MessengerClient* messenger = new MessengerClient();
	bool running = true;
	while(running){
		string msg;
		string command;
		cin >> command;
		running = handleCommand(command, messenger);
	}
	messenger->disconnect();
	delete messenger;
	cout<<"messenger was closed!. bye bye"<<endl;
	return 0;
}


#endif
