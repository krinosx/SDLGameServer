#pragma once
#include <vector>
#include <list>
#include "SDL.h"
#include "SDL_net.h"
#include "Connection.h"
#include "Message.h"

class Server
{
private:
	std::list<Connection*> connections;
	TCPsocket serverSocket;
	bool isReady = false;
	SDLNet_SocketSet socketSet = NULL;

	void addConnection(Connection *connection);
	void removeConnection(Connection *connection);
	int receiveMessage(TCPsocket socket, std::string *message);
	void purgeConnections();
public:
	Server();
	Server(Uint16 port);
	~Server();

	std::vector<Message*> checkForMessages();
	bool isServerReady();
	bool sendMessage(Message * message);
	int broadcast(std::string mensagem);
	bool shutdown();

};

