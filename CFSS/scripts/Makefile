# Compiler and Flags
CC = gcc
CFLAGS = -Wall -g
INCLUDE_FLAGS = -I../inc

# Directories
SRC_DIR = ../src
OBJ_DIR = ../obj
BIN_DIR = ../bin

# Source and Object Files
SRC_FILES = $(SRC_DIR)/cmain.c $(SRC_DIR)/smain.c $(SRC_DIR)/server.c $(SRC_DIR)/client.c
OBJ_FILES = $(OBJ_DIR)/cmain.o $(OBJ_DIR)/smain.o $(OBJ_DIR)/server.o $(OBJ_DIR)/client.o

# Targets
$(BIN_DIR)/server: $(OBJ_DIR)/server.o $(OBJ_DIR)/smain.o
	$(CC) $(OBJ_DIR)/server.o $(OBJ_DIR)/smain.o -o $(BIN_DIR)/server -pthread

$(BIN_DIR)/client: $(OBJ_DIR)/client.o $(OBJ_DIR)/cmain.o
	$(CC) $(OBJ_DIR)/client.o $(OBJ_DIR)/cmain.o -o $(BIN_DIR)/client

# Compilation Rules
$(OBJ_DIR)/cmain.o: $(SRC_DIR)/cmain.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/cmain.c -o $(OBJ_DIR)/cmain.o

$(OBJ_DIR)/smain.o: $(SRC_DIR)/smain.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/smain.c -o $(OBJ_DIR)/smain.o

$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/server.c -o $(OBJ_DIR)/server.o

$(OBJ_DIR)/client.o: $(SRC_DIR)/client.c
	@echo "Compiling client.c..."
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/client.c -o $(OBJ_DIR)/client.o

# Clean up
clean:
	@echo "Cleaning..."
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
