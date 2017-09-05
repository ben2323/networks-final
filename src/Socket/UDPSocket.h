#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class UDPSocket{
	struct sockaddr_in  s_in;
	struct sockaddr_in from;
	unsigned int fsize;
	int socket_fd;
	bool closed;

public:
	UDPSocket(int port = 9999);
	int recv(char* buffer, int length);
	int sendTo(string msg, int length, string ip, int port);
	void cclose();
	string fromAddr();
};

#endif /* UDPSOCKET_H_ */
