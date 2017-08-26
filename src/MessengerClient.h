/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */



#ifndef MESSENGERCLIENT_H_
#define MESSENGERCLIENT_H_

#include <string.h>
#include "Core/MThread.h"
#include "Core/TCPSocket.h"
#include "Core/UDPSocket.h"
#include "Core/MultipleTCPSocketsListener.h"
#include "Models/User.h"
#include "UDPMessenger.h"
#include "Common/TCPMessanger.h"
#include "Common/TCPMessengerProtocol.h"


using namespace std;

class MessengerClient : public MThread{
	TCPSocket* serverSocket;
	UDPMessenger* udpMessenger;
	User* currentUser; // the user object after login

	string connectingUserName;
	bool isHandlingServerData;

	bool isRunning;
private:
	User* readUser();
	void handle();
	vector<User*> getAllUsers();
	vector<User*> getConnectedUsers();

	void printUsers(vector<User*> users);
public:
	MessengerClient();
	virtual ~MessengerClient();

	bool isLoggedIn();
	bool isConnected();
	void login(string userName, string password);
	void registerUser(string userName, string password);
	void connect(string ip);
	void disconnect();
	void scoreBoard();
	void connectUser(string userName);
	void connectRandUser();
	void closeActiveSession(bool remote=false);
	bool isActiveSession();
	bool isConnectedWithUser();
	User* getActiveSessionUser();
	void send(string message);

	void printAllUsers();
	void printConnectedUsers();
	User* getCurrentUser();

	void run();
	void close();
};

#endif /* MESSENGERCLIENT_H_ */
