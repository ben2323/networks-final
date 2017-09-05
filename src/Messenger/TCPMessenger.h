
#ifndef TCPMESSANGER_H_
#define TCPMESSANGER_H_

#include <string.h>
#include "../Socket/TCPSocket.h"
#include "TCPMessengerProtocol.h"
#include "../Users/User.h"

using namespace std;

class TCPMessanger {
public:
	static void sendCommand(TCPSocket* peer, int command){
		int commandNumber = htonl(command);
		peer->send((char*)&commandNumber,4);
	}

	static void sendData(TCPSocket* peer, string msg){
		int dataLength = htonl(msg.length());
		peer->send((char*)&dataLength, 4);
		peer->send(msg.data(), msg.length());
	}

	static void sendInt(TCPSocket* peer, int number){
		number = htonl(number);
		peer->send((char*)&number,4);
	}

	static int readCommand(TCPSocket* peer){
		int commandType;
		int rc = peer->recv((char*)&commandType, 4);
		if(rc != 4){
			return -1;
		}
		commandType = ntohl(commandType);
		return commandType;
	}

	static int readInt(TCPSocket* peer){
		int number;
		int rc = peer->recv((char*)&number, 4);
		if(rc != 4){
			return -1;
		}
		number = ntohl(number);
		return number;
	}

	static string readData(TCPSocket* peer){
		int dataLen;
		int rc = peer->recv((char*)&dataLen,4);
		if(rc != 4){
			return "";
		}
		dataLen = ntohl(dataLen);
		char buffer[dataLen + 1];
		memset((char*)buffer,'\0',dataLen + 1);
		peer->recv(buffer, dataLen);
		string data = string(buffer);
		return data;
	}

	static void sendUser(User* user, TCPSocket* sock){
		sendData(sock, user->getUserName());
		sendData(sock, user->getIP());
		sendInt(sock, user->getPort());
		sendInt(sock, user->getListeningPort());
	}

	static User* readUser(TCPSocket* sock){
		string userName = readData(sock);
		string userIp = readData(sock);
		int userPort = readInt(sock);
		int listeningPort = readInt(sock);
		return new User(userName, userIp, userPort, listeningPort);
	}
};

#endif /* TCPMESSANGER_H_ */
