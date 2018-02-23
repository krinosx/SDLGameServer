#include "Client.h"
#include <iostream>
/**
* Just create the object, does not init internall variables
*/
Client::Client()
{
	this->connection = new Connection();

}

Client::Client(std::string host, Uint16 port)
{
	init(host, port);
}


Client::~Client()
{
}

bool Client::init(std::string host, Uint16 port){

	bool isConnected = this->connection->connect(host, port);

	// Init socket set
	this->socketSet = SDLNet_AllocSocketSet(1);
	
	SDLNet_TCP_AddSocket(this->socketSet, this->connection->getSocket());

	return isConnected;
}

bool Client::isConnected()
{
	return this->connection->isOpen();
}

bool Client::connect(std::string host, Uint16 port)
{
	if (!this->connection->isOpen())
	{
		return init(host, port);
	}
	
	// already connected
	return true;
}

std::vector<Message*> Client::checkForMessages()
{
	std::vector<Message*> EMPTY_MESSAGE_BUFFER(0);
	if (!this->isConnected())
	{
		std::cout << "Client is not connected to a server. Check log files. " << std::endl;
		return EMPTY_MESSAGE_BUFFER;
	}

	int numReady = 0;

	numReady = SDLNet_CheckSockets(this->socketSet, (Uint32)0);
	
	if (numReady == -1)
	{
		std::cout << "Error checking socket activity: " << SDLNet_GetError() << std::endl;
		return EMPTY_MESSAGE_BUFFER;
	}

	if (numReady > 0)
	{
		// If we have only one socket, there is a possibility to have numReady > 1??
		// This way we will return always 1 message by frame, btw, if we need we can change it to read
		// more messages before returning... te use of vector in this case is unecessary.
		// redundance testing???
		if (SDLNet_SocketReady(this->connection->getSocket()))
		{
			numReady--;
			std::vector<Message*> messages;
			std::string messageBuffer;
			// read the next line from socket.
			if (this->connection->read(&messageBuffer)){
				messages.push_back(new Message(this->connection, messageBuffer));
				return messages;
			} 
			else
			{
				std::cout << "Error reading data from connection. Check log for errors. " << std::endl;
				this->connection->closeConnection();
				return EMPTY_MESSAGE_BUFFER;
			}
		}
	}
	return EMPTY_MESSAGE_BUFFER;
}

bool Client::sendMessage(std::string message)
{
	if (this->isConnected())
	{
		return this->connection->write(message);
	}
	else
	{
		return false;
	}
}

/*
* Its possible to add more atributes to the message obcjet and
* format the string in different ways to identify the message type, etc
*/ 
bool Client::sendMessage(Message message)
{
	if (this->isConnected())
	{
		return this->connection->write(message.getMessage());
	}
	else
	{
		return false;
	}
}


bool Client::disconnect()
{
	return this->connection->closeConnection("Dropping...");
}