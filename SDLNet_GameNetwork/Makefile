#
# Makefile for SDLNet_client sample
#
# Build by Giuliano Bortolass (giuliano.bortolassi@gmail.com) in 24-Feb-2018
#

# Use to generate a static lib
ARCHIVER=ar
CXX=g++
CXXFLAGS=-Wall -Werror -std=gnu++11

#
# SDL2 Include dirs
#
INCLUDE_DIR=-I/usr/local/include/SDL2

#BUILD DIR. Intermediate files (.o) will be built in 
# this folder.
BUILD_DIR:=build

#
# Static lib name to be generated
#
BINARY_FILE:=SDLNet_game.a

#
# If new objets are added to the lib, increment this list and create new
# targets to it.
#
OBJ_FILES:=Client.o Connection.o Message.o Server.o





OBJS := $(addprefix $(BUILD_DIR)/, $(OBJ_FILES))

all : setup $(BUILD_DIR)/$(BINARY_FILE)


$(BUILD_DIR)/$(BINARY_FILE) : $(OBJS) SDLNet_game.h
	$(ARCHIVER) rcs $(BUILD_DIR)/$(BINARY_FILE) $(OBJS)
	@echo "\r\n BUILDING SDL_net_client. \
		  \r\n To run, enter in ./build and execute the SDL_net_client file. \
		  \r\n "

$(BUILD_DIR)/Connection.o : Connection.h Connection.cpp Message.h
	$(CXX) -c Connection.cpp -o $(BUILD_DIR)/Connection.o $(INCLUDE_DIR) $(CXXFLAGS)

$(BUILD_DIR)/Message.o : Message.h
	$(CXX) -c Message.cpp -o $(BUILD_DIR)/Message.o $(INCLUDE_DIR) $(CXXFLAGS)

$(BUILD_DIR)/Server.o : Server.h
	$(CXX) -c Server.cpp -o $(BUILD_DIR)/Server.o $(INCLUDE_DIR) $(CXXFLAGS)

$(BUILD_DIR)/Client.o : Client.h 
	$(CXX) -c Client.cpp -o $(BUILD_DIR)/Client.o $(INCLUDE_DIR) $(CXXFLAGS)
	


$(BUILD_DIR):
	mkdir $(BUILD_DIR)

setup: |$(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)


#
# To debug some variable just use
#
# > make print-<VARIABLE_NAME>
#
print-% : ; @echo $* = $($*)