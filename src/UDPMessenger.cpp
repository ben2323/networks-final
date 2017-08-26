/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */



#include "UDPMessenger.h"

UDPMessenger::UDPMessenger(User* currentUser, User* connectedUser){
	this->sem = new Semaphore(1);
	this->currentUser = currentUser;
	this->connectedUser = connectedUser;
	this->isRunning = false;
	this->myturn=1;
	this->hasWon=0;
	this->socket = new UDPSocket(currentUser->getListeningPort());
	for (int i=0;i<3;i++)
		for (int j=0; j<3;j++)
			this->game[i][j]=' ';
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
		cout << "Oponent won! Please disconnect (cs)" << endl;
		this->myturn=0;
		return;
	}
	int msg_row = std::atoi(messageBuffer)/10;
	int msg_col = std::atoi(messageBuffer)%10;

	cout<< ">[" << userName << "] : [" << msg_row << "," << msg_col << "]"<<endl;

	if (mygamechar=='x' && msg_row>=0 && msg_row<=2 && msg_col>=0 && msg_col<=2)
		this->game[msg_row][msg_col]='o';
	else
		this->game[msg_row][msg_col]='x';

				cout << "------" << endl;
				for (int i=0;i<3;i++)
					{
						cout << "|";
							for (int j=0; j<3;j++)
								cout << this->game[i][j] <<"|";
						cout << endl << "------";
						cout << endl;
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

void UDPMessenger::setGameCharX(){
	this->mygamechar='x';
}

void UDPMessenger::setGameCharO(){
	this->mygamechar='o';
}

char UDPMessenger::getGameChar(){
	return this->mygamechar;
}

void UDPMessenger::sendMessage(string msg){

		if (this->myturn)
		{
			char messageBuffer[4000];
			strcpy(messageBuffer,msg.c_str());
			int msg_row = std::atoi(messageBuffer)/10;
			int msg_col = std::atoi(messageBuffer)%10;
			if (this->game[msg_row][msg_col]==' ')
				this->sendMessageToUser(this->connectedUser, msg);
			else this->sendMessageToUser(this->connectedUser, "77");
			this->myturn=0;
			if (this->game[msg_row][msg_col]==' ')
			    this->game[msg_row][msg_col]=mygamechar;


			cout << "------" << endl;
			for (int i=0;i<3;i++)
				{
					cout << "|";
						for (int j=0; j<3;j++)
							cout << this->game[i][j] <<"|";
					cout << endl << "------";
					cout << endl;
				}
			if((this->game[0][0]==mygamechar && this->game[0][1]==mygamechar && this->game[0][2]==mygamechar) || (this->game[1][0]==mygamechar && this->game[1][1]==mygamechar && this->game[1][2]==mygamechar) || (this->game[2][0]==mygamechar && this->game[2][1]==mygamechar && this->game[2][2]==mygamechar) || (this->game[0][0]==mygamechar && this->game[1][0]==mygamechar && this->game[2][0]==mygamechar) || (this->game[0][1]==mygamechar && this->game[1][1]==mygamechar && this->game[2][1]==mygamechar) || (this->game[0][2]==mygamechar && this->game[1][2]==mygamechar && this->game[2][2]==mygamechar) || (this->game[0][0]==mygamechar && this->game[1][1]==mygamechar && this->game[2][2]==mygamechar) || (this->game[2][0]==mygamechar && this->game[1][1]==mygamechar && this->game[0][2]==mygamechar))
			{
				cout << "you won! please disconnect (cs)" << endl;
				this->hasWon=1;
				this->myturn=0;
				this->sendMessageToUser(this->connectedUser, "opopopopop");
			}
		}
		else cout << "Not your turn!" << endl;

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

