

#include "UDPMessenger.h"

UDPMessenger::UDPMessenger(User* currentUser, User* connectedUser){
	this->sem = new Semaphore(1);
	this->currentUser = currentUser;
	this->connectedUser = connectedUser;
	this->isRunning = false;
	this->myturn=1;
	this->numberToGuess = rand() % 10;
	this->hasWon=0;
	//creating socket
	this->socket = new UDPSocket(currentUser->getListeningPort());
	//starting the thread
	this->start();
}



void UDPMessenger::run(){
	this->isRunning = true;
	//create a loop in which you read from the socket and print the received messages
	while(this->isRunning){
		this->receiveMessage();
	}
}

void UDPMessenger::receiveMessage(){
	this->myturn=1;
	// receive the sender name
	char userBuffer[4000];
	memset((char*)userBuffer,'\0',1000);
	this->socket->recv(userBuffer, 1000);
	string userName = string(userBuffer);

	// receive the message
	char messageBuffer[4000];
	memset((char*)messageBuffer,'\0',4000);
	this->socket->recv(messageBuffer, 4000);
	string message = string(messageBuffer);

	if(userName.length() == 0 || message.length() == 0)
		return;
	if(message.length() > 8)
	{
		cout << "The other player has guessed the number first!" << endl;
		this->myturn=0;
		return;
	}

}

User* UDPMessenger::getConnectedUser(){
	return this->connectedUser;
}

bool UDPMessenger::isConnectedToUser(){
	return this->connectedUser != NULL;
}

void UDPMessenger::setTurn(){
	this->myturn=0;
}

void UDPMessenger::sendMessage(string msg){

		if (this->myturn)
		{
			char messageBuffer[4000];
			strcpy(messageBuffer,msg.c_str());

			if (std::to_string(this->numberToGuess) != msg){
				this->sendMessageToUser(this->connectedUser, msg);
			}
			else {
				this->sendMessageToUser(this->connectedUser, "77");
			}
			this->myturn=0;

			if(std::to_string(this->numberToGuess) == msg)
			{
				cout << "you guessed the number correctly! " << endl;
				this->hasWon=1;
				this->myturn=0;
				//a msg longer than 8 letters - indicates the user has won.
				this->sendMessageToUser(this->connectedUser, "abcabcxyzxyz");
			}
		}
		else cout << "It's not your turn to play." << endl;

}







void UDPMessenger::sendMessageToUser(User* user, string msg){
	string currentUserName = this->currentUser->getUserName();
	this->send(user->getIP(), user->getListeningPort(), currentUserName);
	this->send(user->getIP(), user->getListeningPort(), msg);
}

void UDPMessenger::send(string ip, int port, string data){
	this->socket->sendTo(data, data.length(), ip, port);
}

void UDPMessenger::close(){
	this->isRunning = false;
	this->socket->cclose();
	this->waitForThread();
}

UDPMessenger::~UDPMessenger(){
	if(this->socket != NULL){
		delete this->socket;
		this->socket = NULL;
	}
	if(this->connectedUser != NULL){
		delete this->connectedUser;
		this->connectedUser = NULL;
	}
	if(this->sem != NULL){
		delete this->sem;
		this->sem = NULL;
	}
	this->currentUser = NULL;

}

