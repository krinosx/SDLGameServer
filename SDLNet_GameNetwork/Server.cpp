#include "Server.h"
#include <iostream>
#include <algorithm>


bool checkMessage(std::string message) {

	/*
	* TODO: Check if the message obey some kind of protocol. For this default impl we will just check if
	* the message ends with an \n
	character
	*/
	
	return !message.empty() ?  message.back() == '\n' : false;
}

Server::Server() : Server((Uint16) 4000)
{
}


Server::Server(Uint16 port)
{
	isReady = true;

	IPaddress serverIp;
	if (SDLNet_ResolveHost(&serverIp, NULL, port) != 0)
	{
		std::cout << "Unable to resolve host - Could not find a suitable adress to server." << SDLNet_GetError() << std::endl;
		isReady = false;
	}

	this->serverSocket = SDLNet_TCP_Open(&serverIp);
	if (!serverSocket)
	{
		std::cout << "Failed to open server socket: " << SDLNet_GetError() << std::endl;
		isReady = false;
	}

	this->socketSet = SDLNet_AllocSocketSet(10);
	if (!socketSet)
	{
		std::cout << "Could not create a socket set: " << SDLNet_GetError() << std::endl;
		isReady = false;
	}
	// TODO: if server is not ready we need to rollback previous steps
	SDLNet_TCP_AddSocket(this->socketSet, this->serverSocket);
}

bool Server::isServerReady(){
	return this->isReady;
}

bool Server::sendMessage(Message * message)
{
	Connection * connection = message->getConnection();

	if (!connection->isOpen() || !connection->write(message->getMessage()))
	{
		// bad connection, remove from the list
		connection->closeConnection();
		return false;
	}

	return true;
}

Server::~Server()
{
	if (this->isReady)
	{
		this->shutdown();
	}
}


std::vector<Message*> Server::checkForMessages()
{
	purgeConnections();
	std::vector<Message*> EMPTY_MESSAGE_BUFFER(0);

	if (!isReady)
	{
		std::cout << "Server is not ready or not listening. Check the log files. " << std::endl;
		return EMPTY_MESSAGE_BUFFER;
	}

	int numReady = 0;

	numReady = SDLNet_CheckSockets(this->socketSet, (Uint32)0);
	if (numReady == -1)
	{
		std::cout << "Error checking socket activity: [" << SDLNet_GetError() << "]" << std::endl;
		return EMPTY_MESSAGE_BUFFER;
	}
	
	if (numReady > 0)
	{
		/* check for new connection: only one connection is accepted each tick
		*/
		if (SDLNet_SocketReady(this->serverSocket))
		{
			numReady--;

			Connection *connection = new Connection(SDLNet_TCP_Accept(this->serverSocket),"");
			
			if (connection->getSocket())
			{
				const char *name = SDLNet_ResolveIP(SDLNet_TCP_GetPeerAddress(connection->getSocket()));
				connection->setName(name);
				connection->setId(this->connections.size());
				addConnection(connection);
				std::cout << "New connection added to connection pool: " << connection->getName() << std::endl;
			}

		}
		// Check for client messages (may we divide the socketSets in two? 
		// one for incoming connections and other for player messages?
		if (numReady > 0)
		{
			std::vector<Message*> messages;
			std::string messageBuffer;

			for (Connection * connection : this->connections)
			{
				if (SDLNet_SocketReady(connection->getSocket()))
				{
					numReady--;
					messageBuffer.clear();

					if (!connection->read(&messageBuffer))
					{
						std::cout << "CLOSING CONNECTION " << std::endl;
						connection->closeConnection();
					}
					// TODO: deal with data read and decide when classify the data as a message
					// Check the buffer for a command pattern, if found deal with it, otherwise just discard the message.

					if( checkMessage( messageBuffer ) )
					{
						// Add message to the pool
						messages.push_back(new Message(connection, messageBuffer));
					} 
					else 
					{
						// LOG DISCARDED MESSAGE
						std::cout << "Discarded message: " << messageBuffer << std::endl;
					}

					
				}

				// Check if numReady == 0 and skip the tests
			}
			return messages;
		}
	}
	return EMPTY_MESSAGE_BUFFER;
}




void Server::addConnection(Connection * connection)
{
	this->connections.push_back(connection);
	SDLNet_TCP_AddSocket(this->socketSet, connection->getSocket());
}

// Deprecated method - No use for it
int Server::receiveMessage(TCPsocket socket, std::string *message)
{
	std::string messageBuffer;
	int len = 0;

	messageBuffer.clear();
	char byteBuf = ' ';

	while (byteBuf != '\0')
	{
		len = SDLNet_TCP_Recv(socket, &byteBuf, 1);

		messageBuffer += byteBuf;

		if (len < 0)
		{
			std::cout << "No data received, Aborting: " << SDLNet_GetError() << std::endl;
			byteBuf = '\0';
		}
	}
	// expected to copy the value
	*message = messageBuffer;
	return len;
}


int Server::broadcast(std::string mensagem)
{
	for (Connection*connection : this->connections)
	{
		
		if (!connection->write(mensagem)){
			connection->closeConnection();
		}
	}

	return this->connections.size();
}

bool Server::shutdown()
{
	this->isReady = false;

	// Clear all connections
	for (Connection * connection : this->connections)
	{
		SDLNet_TCP_DelSocket(this->socketSet, connection->getSocket());
		connection->closeConnection(std::string("Server is shutting down. Bye bye "));
	}
	//Finish server socket
	SDLNet_TCP_Close(this->serverSocket);
	SDLNet_TCP_DelSocket(this->socketSet, this->serverSocket);

	// Release socket set	
	SDLNet_FreeSocketSet(this->socketSet);

	return true;
}

void Server::purgeConnections()
{
	for (Connection * connection : this->connections)
	{
		if (!connection->isOpen())
		{
			this->removeConnection(connection);
		}
	}

	this->connections.remove_if([](Connection * connection){return !connection->isOpen(); });

	
}


void Server::removeConnection(Connection * connection)
{
	SDLNet_TCP_DelSocket(this->socketSet, connection->getSocket());
	connection->closeConnection("Server dropped your connection. ");
}
