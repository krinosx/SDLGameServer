#pragma once

#include <vector>
#include "SDL.h"
#include "SDL_net.h"
#include "Message.h"

class Client
{
private:
	Connection* connection = nullptr;
	SDLNet_SocketSet socketSet = NULL;
	bool init(std::string host, Uint16 port);
public:
	Client();
	Client(std::string host, Uint16 port);
	~Client();

	bool connect(std::string host, Uint16 port);

	std::vector<Message*> checkForMessages();
	bool isConnected();
	bool sendMessage(std::string mensagem);
	bool sendMessage(Message mensagem);
	bool disconnect();
};

