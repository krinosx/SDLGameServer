// SDL_net_client.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_net.h"


#include "Client.h"

int main(int argc, char* argv[])
{


	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Could not Init SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}

	if (SDLNet_Init() != 0)
	{
		std::cout << "Could not Init SDL_net: " << SDLNet_GetError() << std::endl;
		exit(2);
	}

	// Creating a client with a defautl constructor
	Client *client = new Client();


	if (!client->connect("localhost", (Uint16)5000))
	{
		std::cout << "Error to stabilish a connection with server. " << std::endl;
	}


	int msgId = 1;
	std::string mensagem = "Mensagem para o servidor ";
	
	std::string messageBuffer;
	
	Uint32 initialTime = SDL_GetTicks();
	Uint32 timeSpent = 0;
	while (true)
	{
		
		if (!client->isConnected())
		{
			std::cout << "Connection lost..." << std::endl;
			break;
		}
		
		//
		// Receive server messages and deal with it
		std::vector<Message*> messages = client->checkForMessages();

		for (Message * message : messages)
		{
			std::cout << "Received: " << message->getMessage() << std::endl;
		}

		while (!messages.empty())
		{
			auto * message = messages.back();
			delete message;
			messages.pop_back();
		}
		messages.clear();
		// RENDER()


		// Send messages to server.
		timeSpent = (SDL_GetTicks() - initialTime);
		if (timeSpent > 3000) {

			timeSpent = 0;
			initialTime = SDL_GetTicks();


			std::string finalMessage = mensagem + std::to_string(msgId++);
			finalMessage += "\r\n";
			
			int len = finalMessage.length();
			if( len < 0 )
			{
				std::cout << " Tamanho invalido " << std::endl;
			}

			if (!client->sendMessage(finalMessage))
			{
				std::cout << "Erro ao enviar mensagem para o servidor" << std::endl;
			}

		}

		SDL_Delay(1);
	}
	
	std::cout << "Finishing application..." << std::endl;
	client->disconnect();

	SDLNet_Quit();
	// Shutdown SDL systems
	atexit(SDL_Quit);


	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();

	return 0;
}

