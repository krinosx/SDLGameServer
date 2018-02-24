#
# Makefile for SDL_net_server sample
#


#
# DO NOT CHANGE (unless you know what are you doing)
#
CXX=g++
CXXFLAGS=-Wall  -std=gnu++11
LIBRARIES= -lSDL2main -lSDL2 -lSDL2_net
BINARY_FILE=SDL_net_server

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
# Point to default SDL2 lib dirs
#
LINK_LIB_DIR=-L/usr/local/lib


#BUILD DIR. Intermediate files (.o) and final binary will be built in 
# this folder.
BUILD_DIR:=build
OBJ_FILES:= SDLMud.o



#
# BUILD ROUTINES: NO NEED TO CHANGE BELOW THIS POINT
#

OBJS := $(addprefix $(BUILD_DIR)/, $(OBJ_FILES))

all : setup $(BUILD_DIR)/$(BINARY_FILE) 

$(BUILD_DIR)/$(BINARY_FILE) : $(SDL_NET_LIB) $(OBJS) 
	$(CXX) SDL_net_server.cpp $(OBJS) -o $(BUILD_DIR)/$(BINARY_FILE) $(INCLUDE_DIR) \
	 $(CXXFLAGS) $(LINK_LIB_DIR) $(LIBRARIES) $(SDL_NET_LIB) 


$(BUILD_DIR)/SDLMud.o: SDLMud.h
	$(CXX) SDLMud.cpp -c -o $(BUILD_DIR)/SDLMud.o $(INCLUDE_DIR) $(CXXFLAGS)
#
# If the library is not build, enter the project folder and execute the build
#
$(SDL_NET_LIB):
	cd $(SDL_NET_LIB_DIR) && make


setup :|$(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)



#
# To debug some variable just use
#
# > make print-<VARIABLE_NAME>
#
print-% : ; @echo $* = $($*)




