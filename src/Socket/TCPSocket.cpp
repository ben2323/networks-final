#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include "TCPSocket.h"
using namespace std;

TCPSocket::TCPSocket(int connected_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr){
	// initialize local class properties;
	this->sock = connected_sock;
	this->localAddr = serverAddr;
	this->peerAddr = peerAddr;
	this->connected = true;
}


TCPSocket::TCPSocket(int port){
	this->connected = false;
	// open TCP socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("Error opening socket");
		close(sock);
		return;
	}

	// set the server address for binding
	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	// bind the socket to the address
	if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror ("Error binding the socket");
		close(sock);
		return;
	}

	this->sock = sock;
	this->localAddr = serverAddr;
	this->connected = true;
}


TCPSocket::TCPSocket(string peerIp, int port){
	this->connected = false;
	// open TCP socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("Error opening socket");
		close(sock);
		return;
	}

	// set the peer address to connect to
	struct sockaddr_in peerAddr;
	bzero(&peerAddr, sizeof(peerAddr));
	peerAddr.sin_family = AF_INET;
	peerAddr.sin_addr.s_addr = inet_addr(peerIp.c_str());
	peerAddr.sin_port = htons(port);

	// connect the socket to the peer server
	if (connect(sock, (struct sockaddr *)&peerAddr, sizeof(peerAddr)) < 0)
	{
		perror ((string("Error Connect") + peerIp).c_str());
		close(sock);
		return;
	}

	this->sock = sock;
	this->peerAddr = peerAddr;
	this->connected = true;
}

bool TCPSocket::isConnected(){
	return this->connected;
}


TCPSocket* TCPSocket::listenAndAccept(){
	// listen on the socket
	listen(this->sock, 1);

	// call accept on the socket
	struct sockaddr_in peerAddr;
	//size_t len = sizeof(peerAddr);
	unsigned int len = sizeof(peerAddr);
	int connect_sock = accept(this->sock, (struct sockaddr *)&(peerAddr), &len);
	if(connect_sock == -1){
		return NULL;
	}
	this->peerAddr = peerAddr;

	// return new TCPSocket object holding the new secondary server socket returned from the accept call
	return new TCPSocket(connect_sock, this->localAddr,this->peerAddr);
}


int TCPSocket::recv(char* buffer, int length){
	// read from the socket
	int byteRead = read(this->sock, buffer, length);
	return byteRead;
}


int TCPSocket::send(const char* msg, int len){
	// write on the socket
	int byteSent = write(this->sock, msg, len);
	return byteSent;
}


void TCPSocket::cclose(){
	// shutdown and close the socket
	shutdown(this->sock, SHUT_RDWR);
	close(this->sock);
}

string TCPSocket::fromAddr(){
	// return the session destination peer address
	string seperator = ":";
	string ip = this->getIP();

	char portBuff[50];
	memset((char*)portBuff,'\0', 50);
	sprintf(portBuff,"%i", this->getPort());
	string port(portBuff);
	return  ip + seperator + port;
}

int TCPSocket::getFD(){
	return this->sock;
}

int TCPSocket::getPort(){
	return this->peerAddr.sin_port;
}

string TCPSocket::getIP(){
	return inet_ntoa(this->peerAddr.sin_addr);
}
