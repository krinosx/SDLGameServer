/*
 * Central header
 * Must be used to define some application parameters
 *
 * Sometime in futre this parameters may be externalized and put
 * in a configuration file.
 */

#pragma once

#include <string>
#include <vector>
#include "SDLNet_game.h"



#define PULSES_PER_SECOND	0.25f
#define MAX_BUFFER_SIZE		1024
#define SERVER_PORT			5000




class SDLMud
{
private:
	/* Network server object */
	Server *server;

	bool isRunning = false;
	/* How much ticks server proccess in a seccond */
	float pulsesPerSeccond = 1.0f;
	/* Default network Port*/
	Uint16 networkPort = 5000;

	/* A vector to store incoming messages from Server */
	std::vector<Message*> inputMessages;
	/* A vector to store outgoing messages */
	std::vector<Message*> outputMessages;

public:
	SDLMud();
	~SDLMud();

	/*
	 * Load game configuration file. This should be 
	 * the first thing to be called in this object.
	 */
	bool loadConfigs();
	
	/* Start the network server and all game subsystems */
	bool init();

	/* Start processing events/ticks */
	void run();
	
	/* Stop the processing of events/ticks*/
	void stop();

	/* Shutdown the game */
	bool shutdown();
};
