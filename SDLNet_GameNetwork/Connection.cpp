#include "Connection.h"
#include "Message.h"

#include <iostream>
#include <limits.h>

Connection::Connection()
{
	this->isConnectionOpen = false;
	this->socket = NULL;
}

Connection::Connection(TCPsocket socket,std::string name)
{
	this->name = name;
	this->socket = socket;
	this->isConnectionOpen = true;
}

Connection::~Connection()
{
	// Ensure socket is closed
	SDLNet_TCP_Close(this->socket);
}

bool Connection::connect(std::string ipaddress, Uint16 port)
{
	IPaddress serverIp;
	if (SDLNet_ResolveHost(&serverIp, ipaddress.c_str(), port) != 0)
	{
		std::cout << "Unable to resolve server Host: " << SDLNet_GetError() << std::endl;
		return false;
	}

	//just check if socket is closed
	if (this->socket != NULL) {
		this->socket = NULL;
		//SDLNet_TCP_Close(this->socket);
	}
	
	this->socket = SDLNet_TCP_Open(&serverIp);
	if (!this->socket)
	{
		std::cout << "Unable to open a connection with specified server " << SDLNet_GetError() << std::endl;
		return false;
	}

	const char * name = SDLNet_ResolveIP(&serverIp);
	this->setName( std::string(name) );
	isConnectionOpen = true;
	return true;
}

bool Connection::isOpen()
{
	return isConnectionOpen;
}

void Connection::setName(std::string name)
{
	this->name = name;
}

std::string Connection::getName()
{
	return this->name;
}
bool Connection::closeConnection()
{
	return this->closeConnection(std::string(""));
}

bool Connection::closeConnection(std::string message)
{

	if (isConnectionOpen)
	{
		//bool result = this->write(message);
		SDLNet_TCP_Close(this->socket);
	}

	isConnectionOpen = false;

	return true;
}

bool Connection::write(std::string message)
{


	// Get the text provided (we expect it is a stringbuffer with some data)
	// and add the msg_size in the font
	if (message.size() > USHRT_MAX)
	{
		return false;
	}

	unsigned short dataSize = (unsigned short)message.size();
	int messageSize = dataSize + 2;

	std::string sizeData;
	Message::writeByteShort(&sizeData, dataSize+2);
	message.insert(0, sizeData);


	
	int writeResult = 0;
	if (messageSize > 0)
	{
		writeResult = SDLNet_TCP_Send(this->socket, message.c_str(), messageSize);
	
		if (writeResult < messageSize)
		{ // log and error
			std::cout << "Error sending message to the connection " << this->name << " ErrorMessage: " << SDLNet_GetError() << std::endl;
			
			return false;
		}
		return true;
	}
	//if message is empty, return false
	return false;
}

/*
* FIXME: We must optimize this code. The \0 is not a good indicative that 
* socket has no more data to be read.
*
* IDEA: We may read one byte at time and just return it to the high level function. 
* The high level function will be responsible to concatenate the read bytes and decide when a 
* fully message is received, this way we may not block the socket reading.
* Using this approach we must run the message checking routine more than once in a tick to 
* check if the data is read before putting the loop to sleep.
*
*
* IDEA2: We must determine a protocol where the first byte specify the total amount of bytes sent. So
* we can use a buffer to read and validate the protocol.
* TO use this approach we must ensure that if a socket do not obey the protocil it must be dropped
* or the server may crash.
*
*
*
*
*/
bool Connection::read(std::string *buffer)
{
	std::string messageBuffer;
	int len = 0;

	messageBuffer.clear();
	char byteBuf[MAX_MESSAGE_SIZE];

	memset(byteBuf, '\0', MAX_MESSAGE_SIZE);


	const int shortSize = 2;
	char sizeData[shortSize];
	unsigned short messageSize = 0;

	len = SDLNet_TCP_Recv(this->socket, &sizeData, shortSize);

	if (len != shortSize)
	{
		//LOG ERROR
	}

	messageSize = sizeData[0] << 8;
	messageSize = messageSize | sizeData[1];
	
	int remainindData = messageSize - shortSize;

	len = SDLNet_TCP_Recv(this->socket, &byteBuf, remainindData);
	if (len <= 0)
	{
		std::cout << "No data received, Aborting: " << SDLNet_GetError() << std::endl;
		byteBuf[0] = '\0';
		buffer = '\0';
		return false;
	}
	
	Message::writeByteShort( &messageBuffer, messageSize );
	for (int i = 0; i < remainindData; i++)
	{
		messageBuffer.push_back(byteBuf[i]);
	}	

	if (messageBuffer.size() > this->MAX_MESSAGE_SIZE - 1)
	{
			std::cout << "Message too big, truncating at MAX_MESSAGE_SIZE [ " << this->MAX_MESSAGE_SIZE << "]" << std::endl;
			byteBuf[0] = '\0';
			messageBuffer += '\0';
	}

	
	(*buffer).append(messageBuffer);
	return true;
}

TCPsocket Connection::getSocket()
{
	return this->socket;
}

void Connection::setSocket(TCPsocket socket)
{
	this->socket = socket;
}

void Connection::setId(int id)
{
	this->id = id;
}
/** 
 * Used to comparasion purposes
 */
bool Connection::operator==(const Connection &other)
{
	return this->id == other.id;
}

bool Connection::operator!=(const Connection &other)
{
	return !(*this == other);
}