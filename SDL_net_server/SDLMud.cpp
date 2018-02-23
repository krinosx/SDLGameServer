#include "SDLMud.h"
#include "SDL_timer.h"
#include <iostream>


void processMessage(Message * message)
{
	std::cout << "Connection: " << message->getConnection()->getName() << "  Message: " << message->getMessage() << std::endl;

}

SDLMud::SDLMud()
{

}


SDLMud::~SDLMud()
{
	if (this->isRunning) { this->isRunning = false; }

	while (!this->inputMessages.empty())
	{
		// DEBUG TO CHECK IF MESSAGES ARE DESTROYED
		this->inputMessages.pop_back();
	}
	this->inputMessages.clear();

	while (!this->outputMessages.empty())
	{
		// DEBUG TO CHECK IF MESSAGES ARE DESTROYED
		this->outputMessages.pop_back();
	}
	this->outputMessages.clear();

}



bool SDLMud::loadConfigs(){
	//TODO: Externalize the config
	this->networkPort = SERVER_PORT;
	this->pulsesPerSeccond = 4;

	return true;
}

bool SDLMud::init()
{

	/*TODO: Load data structures, helps, user list, other modules, etc etc*/

	/* Start network server*/
	this->server = new Server( this->networkPort );
	
	

	// Check if all systems are ok and return TRUE/FALSE
	return this->server->isServerReady();
}

void SDLMud::run(){
	this->isRunning = true;
	
	Uint32 loopMaxTime = (Uint32)(1000 / this->pulsesPerSeccond);


	while (this->isRunning)
	{
		Uint32 loopStartTime = SDL_GetTicks();


		/* Get incoming messages from network server */
		this->inputMessages = server->checkForMessages();

		if (this->inputMessages.size() > 0)
		{ /* Process player commands */

			for (Message * message : this->inputMessages )
			{
				// TODO: Create a 'command parser' module?
				processMessage( message );

				this->outputMessages.push_back(new Message(message->getConnection(), "FromServer: " + message->getMessage()));
				// remove last message
			}

			
			while (!this->inputMessages.empty())
			{
				auto * message = this->inputMessages.back();
				delete message;
				this->inputMessages.pop_back();
			}

		}

		// TODO: Update Game Model (process)



		// Flush outgoing messages
		if (this->outputMessages.size() > 0) 
		{
			for (Message * outgoingMessage : this->outputMessages)
			{
				if (!this->server->sendMessage(outgoingMessage))
				{
					std::cout << "Error to post a message";
				}
				else
				{
					std::cout << "Sent: " + outgoingMessage->getMessage();
				}

			}

			while (!this->outputMessages.empty())
			{
				auto * message = this->outputMessages.back();
				delete message;
				this->outputMessages.pop_back();
			}
			this->outputMessages.clear();
		}



		/* Check the loop time and control the ticks per second */
		Uint32 loopEndTime = SDL_GetTicks();

		Uint32 loopTotalTime = loopEndTime - loopStartTime;

		/* If we have some time to sleep, so sleep*/
		if (loopTotalTime < loopMaxTime)
		{
			Uint32 timeToSleep = loopMaxTime - loopTotalTime;
			SDL_Delay(timeToSleep);
		}
		else
		{
			std::cout << "Found a lagging loop. It took " << loopTotalTime << " to run. The max expected time is: " << loopMaxTime << std::endl;
		}


	}
}




void SDLMud::stop()
{
	this->isRunning = false;
}

bool SDLMud::shutdown()
{
	// Unload all game modules


	// Close Network Server
	this->server->shutdown();
	

	return !this->server->isServerReady();	
}

