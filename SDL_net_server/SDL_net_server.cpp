// SDL_net_server.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_net.h"
#include "SDLMud.h"


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


	SDLMud myMud;
	if (!myMud.loadConfigs())
	{
		std::cout << "Error loading game configuration. Check log file for details." << std::endl;
	}

	if (!myMud.init())
	{
		std::cout << "Error starting game systems. Check log file for details." << std::endl;
	}

	/* This will block untill method Mud::stop() is called*/
	myMud.run();


	if (!myMud.shutdown()){
		std::cout << "Error to shutdown game systems. Check log file for details." << std::endl;
	}


	// Shutdown Network
	SDLNet_Quit();
	// Shutdown SDL systems
	atexit(SDL_Quit);
	

	std::cout << "Game Ended. Press any key to continue..." << std::endl;
	std::cin.get();

	return 0;
}



