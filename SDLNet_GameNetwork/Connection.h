#pragma once

#include <string>
#include "SDL.h"
#include "SDL_net.h"

class Connection
{
public:
	Connection();
	Connection(TCPsocket socket, std::string name);
	~Connection();

	void setSocket(TCPsocket socket);
	TCPsocket getSocket();


	bool isOpen();
	/**
	* Set a name for the connection
	*/
	void setName(std::string name);

	std::string getName();

	/**
	 * Connect to a given IPAddress and port.
	 */
	bool connect(std::string ipaddress, Uint16 port);

	/**
	* Write the content of message parameter to the socket
	*/
	bool write(std::string message);

	/** 
	 * Read the the socket a byte each time untill the end of line ( '\0' ).
	 * Write the contet read into buffer parameter
	*/
	bool read(std::string * buffer);

	/**
	* Send a message to user and close the connection
	*/
	bool closeConnection(std::string message);
	/**
	* Close the connection
	*/
	bool closeConnection();

	void setId(int id);
	
	bool operator==(const Connection &other);
	bool operator!=(const Connection &other);
private:
	static const size_t MAX_MESSAGE_SIZE = 1024; // The array size must be the same for all objects
	std::string name;
	TCPsocket socket;
	int id;
	bool isConnectionOpen = false;

};

