
#include "MessengerClient.h"

MessengerClient::MessengerClient() {
	this->currentUser = NULL;
	this->udpMessenger = NULL;
	this->serverSocket = NULL;

	this->connectingUserName = "";
	this->isHandlingServerData = false;

	this->isRunning = false;
}

void MessengerClient::login(string userName, string password){
	this->isHandlingServerData = true;
	TCPMessanger::sendCommand(this->serverSocket, LOGIN);
	TCPMessanger::sendData(this->serverSocket, userName);
	TCPMessanger::sendData(this->serverSocket, password);

	//now we wait for the server to answer if the login succeeded
	int command = TCPMessanger::readCommand(this->serverSocket);
	if(command == LOGIN){
		this->currentUser = new User(userName, "",0); // the ip and port is not relevant for our user..
		cout << "Login succeeded, you are now logged as: " << userName << " :) " << endl;
	}
	else{
		cout<< "Login failed, please try again, your user name or password are incorrect" <<  endl;
	}
	this->isHandlingServerData = false;
}

void MessengerClient::registerUser(string userName, string password){
	this->isHandlingServerData = true;
	TCPMessanger::sendCommand(this->serverSocket, REGISTER);
	TCPMessanger::sendData(this->serverSocket, userName);
	TCPMessanger::sendData(this->serverSocket, password);

	//now we wait for the server to answer if the register succeeded
	int command = TCPMessanger::readCommand(this->serverSocket);
	if(command == REGISTER){
		this->currentUser = new User(userName, "",0); // the ip and port is not relevant for our user..
		cout << "Register succeeded, you are now logged as: " << userName << " :) " << endl;
	}
	else{
		cout<< "Register failed, please try again, your user name is probably already in use by other user" <<  endl;
	}
	this->isHandlingServerData = false;
}

bool MessengerClient::isLoggedIn(){
	return this->currentUser != NULL;
}

bool MessengerClient::isConnected(){
	return this->serverSocket != NULL && this->serverSocket->isConnected();
}

bool MessengerClient::isActiveSession(){
	return this->udpMessenger != NULL;
}

bool MessengerClient::isConnectedWithUser(){
	return this->udpMessenger != NULL && this->udpMessenger->getConnectedUser() != NULL;
}

User* MessengerClient::getActiveSessionUser(){
	if(!this->isActiveSession()) return NULL;
	return this->udpMessenger->getConnectedUser();
}

User* MessengerClient::getCurrentUser(){
	return this->currentUser;
}

void MessengerClient::printAllUsers(){
	this->isHandlingServerData = true;

	cout<<"All Users: " << endl;
	TCPMessanger::sendCommand(this->serverSocket, GET_USERS_LIST);
	// now we wait for the server to answer with the users list
	int command = TCPMessanger::readCommand(this->serverSocket);
	if(command == GET_USERS_LIST){
		int usersLength = TCPMessanger::readInt(this->serverSocket);
		for(int i = 0; i< usersLength; i++){
			cout<< TCPMessanger::readData(this->serverSocket) << endl;
		}
	}
	else{
		perror("Failed to get users list");
		cout << "disconnecting from server" << endl;
		this->disconnect();
	}

	this->isHandlingServerData = false;
}

void MessengerClient::printConnectedUsers(){
	this->isHandlingServerData = true;

	cout<<"Connected Users: " << endl;
	vector<User*> users;
	TCPMessanger::sendCommand(this->serverSocket, GET_CONNECTED_USERS);
	// now we wait for the server to answer with the connected users list
	int command = TCPMessanger::readCommand(this->serverSocket);
	if(command == GET_CONNECTED_USERS){
		int usersLength = TCPMessanger::readInt(this->serverSocket);
		for(int i = 0; i< usersLength; i++){
			cout<< TCPMessanger::readData(this->serverSocket) << endl;
		}
	}
	else{
		perror("Failed to get connected users list");
		cout << "disconnecting from server" << endl;
		this->disconnect();
	}

	this->isHandlingServerData = false;
}

void MessengerClient::send(string msg){
	if(this->udpMessenger == NULL) {
		cout<< "no active session, unable to send message" << endl;
		return;
	}

	this->udpMessenger->sendMessage(msg);
}

void MessengerClient::connect(string ip){
	this->serverSocket = new TCPSocket(ip, MSNGR_PORT);
	if(!this->serverSocket->isConnected()){
		this->serverSocket = NULL;
		cout << "failed to connect.. try again" << endl;
		return;
	}
	this->start();
}

void MessengerClient::disconnect(){
	this->closeActiveSession();

	this->close(); // stop the run loop that listening to the server socket...

	if(this->currentUser != NULL){
		delete this->currentUser;
		this->currentUser = NULL;
	}

	if(this->serverSocket != NULL){
		TCPMessanger::sendCommand(this->serverSocket, EXIT);
		this->serverSocket->cclose();
		delete this->serverSocket;
		this->serverSocket = NULL;
	}

}

void MessengerClient::scoreBoard(){
	this->isHandlingServerData = true;

		cout<<"Score table: " << endl;
		TCPMessanger::sendCommand(this->serverSocket, GET_SCORE);
		// now we wait for the server to answer with the users list
		int command = TCPMessanger::readCommand(this->serverSocket);
		if(command == GET_SCORE){
			int usersLength = TCPMessanger::readInt(this->serverSocket);
			for(int i = 0; i< usersLength; i++){
				string tag1 = TCPMessanger::readData(this->serverSocket);
				int tag2 = TCPMessanger::readInt(this->serverSocket);
				cout<< tag1 << "    " << tag2 << endl;
			}
		}
		else{
			perror("Failed to get users list");
			cout << "disconnecting from server" << endl;
			this->disconnect();
		}

		this->isHandlingServerData = false;

}

void MessengerClient::connectUser(string userName){
	this->closeActiveSession();
	if(userName.compare(this->currentUser->getUserName()) == 0){
		cout<<"unable to create session with yourself" << endl;
		return;
	}
	this->isHandlingServerData = true;

	this->connectingUserName = userName;
	TCPMessanger::sendCommand(this->serverSocket, OPEN_SESSION_WITH_USER);
	TCPMessanger::sendData(this->serverSocket, userName);
	//now we wait for the server to answer with the user ip address
	int command = TCPMessanger::readCommand(this->serverSocket);
	if(command == SESSION_ESTABLISHED){
		int listeningPort = TCPMessanger::readInt(this->serverSocket);
		this->currentUser->setListeningPort(listeningPort);
		User* targetUser = this->readUser();
		this->udpMessenger = new UDPMessenger(this->currentUser, targetUser);
		this->udpMessenger->setTurn();
		this->udpMessenger->setGameCharX();
		cout<< "open session with user: " << userName << " succeeded. You are " << this->udpMessenger->getGameChar() << endl;
	}
	else {
		cout << "open session with user: " << userName << " failed" << endl;
	}
	this->connectingUserName = "";
	this->isHandlingServerData = false;
}

void MessengerClient::connectRandUser(){
	this->isHandlingServerData = true;
	string userName = "";
	TCPMessanger::sendCommand(this->serverSocket, GET_CONNECTED_USERS);
	// now we wait for the server to answer with the connected users list
	int command = TCPMessanger::readCommand(this->serverSocket);
	if(command == GET_CONNECTED_USERS){
		int usersLength = TCPMessanger::readInt(this->serverSocket);
		int randUsr = rand() % usersLength;
		for(int i = 0; i< usersLength; i++){
			string temp =  TCPMessanger::readData(this->serverSocket);
			if (i == randUsr)
				userName = temp;
		}
	}
	else{
		perror("Failed to get connected users list");
		cout << "disconnecting from server" << endl;
		this->disconnect();
	}
	this->isHandlingServerData = false;
	if (userName!=""){
		this->connectUser(userName);
	}else{
			perror("Failed to get connected users list");
			cout << "disconnecting from server" << endl;
			this->disconnect();
		}

}

void MessengerClient::closeActiveSession(bool remote){
	this->connectingUserName = "";
	if(this->udpMessenger != NULL){
		if(this->udpMessenger->hasWon)
		{
			cout << "updating scores... " << endl;
			TCPMessanger::sendCommand(this->serverSocket, UPDATE_SCORE);
			TCPMessanger::sendData(this->serverSocket, this->currentUser->getUserName());
		}
		if(remote){
			cout << "session closed by remote user" << endl;
		}
			string userName = this->udpMessenger->getConnectedUser()->getUserName();
			cout << "closing session with: " << userName << endl;
			TCPMessanger::sendCommand(this->serverSocket, CLOSE_SESSION_WITH_USER);
			TCPMessanger::sendData(this->serverSocket, userName);
		this->udpMessenger->close();
		delete this->udpMessenger;
		this->udpMessenger = NULL;
		cout << "active session closed" << endl;
	}
}

void MessengerClient::run(){
	this->isRunning = true;
	while(this->isRunning){
		if(this->isHandlingServerData){
			sleep(1);
			continue;
		}
		MultipleTCPSocketsListener* listener = new MultipleTCPSocketsListener();
		listener->addSocket(this->serverSocket);
		TCPSocket* readySocket = listener->listenToSocket();
		listener->close();
		delete listener;
		if(readySocket == NULL || this->isHandlingServerData){
			continue;
		}
		handle();
	}
}

void MessengerClient::handle(){
	int command = TCPMessanger::readCommand(this->serverSocket);
	string userName;
	User* user;
	int listeningPort;
	switch(command){
		case OPEN_SESSION_WITH_USER:
			cout << "received open session request" << endl;
			listeningPort = TCPMessanger::readInt(this->serverSocket);
			user = this->readUser();
			this->closeActiveSession();
			this->currentUser->setListeningPort(listeningPort);
			this->udpMessenger = new UDPMessenger(this->currentUser, user);
			this->udpMessenger->setGameCharO();
			cout << "opening session by remote request with: " << user ->getUserName() << " you are " << this->udpMessenger->getGameChar() << endl;
			cout << "If you are unwilling to continue playing with " << user ->getUserName() << " you can press 'y' to disconnect " << endl;
			break;
		case CLOSE_SESSION_WITH_USER:
			this->closeActiveSession(true);
			break;
		case EXIT:
			cout << "disconnect from the server by remote command" << endl;
			this->disconnect();
			break;
		default:
			break;
	}
}

User* MessengerClient::readUser(){
	string userName = TCPMessanger::readData(this->serverSocket);
	string userIp = TCPMessanger::readData(this->serverSocket);
	int userPort = TCPMessanger::readInt(this->serverSocket);
	int listeningPort = TCPMessanger::readInt(this->serverSocket);
	return new User(userName, userIp, userPort, listeningPort);
}



void MessengerClient::close(){
	this->isRunning = false;
	this->waitForThread();
}

MessengerClient::~MessengerClient() {

}

