/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#include "MultipleTCPSocketsListener.h"
using namespace std;

MultipleTCPSocketsListener::MultipleTCPSocketsListener(){
	this->socketList = new vector<TCPSocket*>();
	this->sem = new Semaphore(1);
}

/* Add the given socket to the socket list */
void MultipleTCPSocketsListener::addSocket(TCPSocket* socket){
	sem->lock();
	// add he given socket to the socket list
	this->socketList->push_back(socket);
	sem->unlock();
}

/* Set the given sockets to the socket */
void MultipleTCPSocketsListener::addSockets(vector<TCPSocket*> socketVec){
	sem->lock();
	// set the given sockets as the socket list to select from
	this->socketList->insert(this->socketList->begin(), socketVec.begin(), socketVec.end());
	sem->unlock();
}

/*
 * This method checks to see if any of the previous given sockets are ready for reading
 */
TCPSocket* MultipleTCPSocketsListener::listenToSocket(){
	if(this->socketList->size() == 0){
		return NULL;
	}
	fd_set set;
	FD_ZERO(&set);

	sem->lock();
	// fill the set with file descriptors from the socket list using (FD_SET macro)
	for(vector<TCPSocket*>::iterator it = this->socketList->begin(); it != this->socketList->end(); ++it){
		FD_SET((*it)->getFD(), &set);
	}
	sem->unlock();

	struct timeval time = {5,0};
	int result = select(sizeof(set)*8,&set,NULL,NULL,&time);

	// check the returned value from the select to find the socket that is ready
	if(result == -1){
		perror("select failed");
		return NULL;
	}

	// no waiting sockets
	if(result == 0){
		return NULL;
	}
	sem->lock();
	for(vector<TCPSocket*>::iterator it = this->socketList->begin(); it != this->socketList->end(); ++it){
		if(FD_ISSET((*it)->getFD(), &set)){
			sem->unlock();
			// return a valid socket return the matching TCPSocket object
			return (*it);
		}
	}
	sem->unlock();

	// otherwise return NULL
	return NULL;
}

MultipleTCPSocketsListener::~MultipleTCPSocketsListener(){
	if(this->sem == NULL)
		return;
	sem->lock();
	delete this->socketList;
	this->socketList = NULL;
	sem->unlock();

	delete this->sem;
	this->sem = NULL;
}

void MultipleTCPSocketsListener::close(){
	sem->lock();
	this->socketList->clear();
	sem->unlock();
}
