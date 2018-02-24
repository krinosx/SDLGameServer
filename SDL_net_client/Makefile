#
# Makefile for SDLNet_client sample
#


#
# DO NOT CHANGE (unless you know what are you doing)
#
CXX=g++
CXXFLAGS=-Wall  -std=gnu++11
LIBRARIES= -lSDL2main -lSDL2 -lSDL2_net SDLNet_game.a
BINARY_FILE=SDL_net_client

#
# SDLNet_GameNetwork Libray root folder. If you got the default git repository
# it will be as specified, no need to change. If its not the case, point the
# SDL_NET_LIB_DIR to the root folder (where the Makefile is located) of 
# SDLNet_GameNetwork project.
#
SDL_NET_LIB_DIR=../SDLNet_GameNetwork
SDL_NET_LIB=$(SDL_NET_LIB_DIR)/build/SDLNet_game.a

#
# Point to default SDL2 include dirs, plus the SDLNet_GameNetwork root  
# folder ( the folder where header files '*.h' is located ) 
#
INCLUDE_DIR=-I/usr/local/include/SDL2 -I../SDLNet_GameNetwork
#
# Point to default SDL2 lib dirs, plus the SDLNet_GameNetwork build  
# folder ( the folder where SDLNet_game.a is located ) 
#
LINK_LIB_DIR=-L/usr/local/lib -L../SDLNet_GameNetwork/build/


#BUILD DIR. Intermediate files (.o) and final binary will be built in 
# this folder.
BUILD_DIR:=build
OBJ_FILES:=SDL_net_client.o 



#
# BUILD ROUTINES: NO NEED TO CHANGE BELOW THIS POINT
#

OBJS := $(addprefix $(BUILD_DIR)/,$(OBJS_FILES))

all : $(BUILD_DIR)/$(BINARY_FILE) 

$(BUILD_DIR)/$(BINARY_FILE) : $(SDL_NET_LIB)  | $(BUILD_DIR)
	$(CXX) SDL_net_client.cpp -o $(BUILD_DIR)/$(BINARY_FILE) $(INCLUDE_DIR) \
	 $(CXXFLAGS) $(LINK_LIB_DIR) $(LIBRARIES)

#
# If the library is not build, enter the project folder and execute the build
#
$(SDL_NET_LIB):
	cd $(SDL_NET_LIB_DIR) && make


$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)