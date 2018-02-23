#include "Message.h"

Message::Message(Connection *connection, std::string message)
{
	this->connection = connection;
	// expected to copy the string value
	this->message = message;
}


Message::~Message()
{
	this->connection = NULL;
	// is std::string automatic destroyed? hope so.
}


Connection* Message::getConnection()
{
	return this->connection;
}

std::string Message::getMessage()
{
	return this->message;
}

void Message::writeByteInt(std::string * buffer, unsigned char value)
{
	(*buffer).push_back(value);
}

void Message::writeByteShort(std::string * buffer, unsigned short value)
{
	char data[2];
	data[0] = value >> 8;
	data[1] = value & 0xFF;
	(*buffer).push_back(data[0]);
	(*buffer).push_back(data[1]);
}

int Message::readByteInt(std::string * buffer, unsigned int position)
{
	if (position < (*buffer).size())
	{
		return 0;
	}
	return -1;
}

unsigned short Message::readByteShort(std::string * buffer, unsigned int position)
{

	if (position + 1 < (*buffer).size()) {
		unsigned short idRead = (*buffer)[position] << 8;
		idRead = idRead | (*buffer)[position + 1];
	}
	return -1;
}
