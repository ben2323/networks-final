
#ifndef UDPMESSENGER_H_
#define UDPMESSENGER_H_

#include <string.h>
#include <iostream>
#include <cstdlib>
#include "Socket/UDPSocket.h"
#include "Socket/MThread.h"
#include "Socket/Semaphore.h"
#include "Messenger/TCPMessengerProtocol.h"
#include "Users/User.h"

using namespace std;

class UDPMessenger: public MThread{
	Semaphore* sem;
	UDPSocket* socket;
	User* connectedUser;
	User* currentUser;

	int numberToGuess;
	bool myturn;
	bool isRunning;

private:
	void receiveMessage();
	void sendMessageToUser(User* user, string msg);
	void send(string ip, int port, string data);

public:
	UDPMessenger(User* currentUser, User* connectedUser);

	User* getConnectedUser();

	void setTurn();

	bool isConnectedToUser();

	void sendMessage(string msg);

	void close();
	void run();

	~UDPMessenger();

	bool hasWon;

};

#endif
