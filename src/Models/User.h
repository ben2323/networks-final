/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */



#ifndef USER_H_
#define USER_H_

#include <string.h>
#include "../Core/UDPSocket.h"

using namespace std;

class User {
	string userName;
	string ip;
	int port;
	int listeningPort;

public:
	User(string userName, string ip, int port);
	User(string userName, string ip, int port, int listeningPort);
	virtual ~User();

	string getUserName();
	string getIP();
	int getPort();
	void setListeningPort(int port);
	int getListeningPort();
};

#endif /* USER_H_ */
