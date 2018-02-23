#pragma once
#include <string>
#include "Connection.h"
class Message
{
private:
	std::string message;
	Connection *connection;

public:
	Message(Connection *connection, std::string message);
	~Message();
	Connection* getConnection();
	std::string getMessage();

	static void writeByteInt(std::string * buffer, unsigned char value);
	static void writeByteShort(std::string * buffer, unsigned short value);

	static int readByteInt(std::string * buffer, unsigned int position);
	static unsigned short readByteShort(std::string * buffer, unsigned int position);

};

