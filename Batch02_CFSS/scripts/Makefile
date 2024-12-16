# Compiler and Flags
CC = gcc
CFLAGS = -Wall -g -c
OFLAGS = -o
INCLUDE_FLAGS = -I../inc

# Directories
SRC_DIR = ../src
OBJ_DIR = ../obj
BIN_DIR = ../bin
EXC = $(BIN_DIR)/server $(BIN_DIR)/client

# Source and Object Files
SRC_FILES = $(SRC_DIR)/cmain.c $(SRC_DIR)/smain.c $(SRC_DIR)/srvcall.c $(SRC_DIR)/clntcall.c
OBJ_FILES = $(OBJ_DIR)/cmain.o $(OBJ_DIR)/smain.o $(OBJ_DIR)/srvcall.o $(OBJ_DIR)/clntcall.o

# Targets
all: $(EXC)

$(BIN_DIR)/server: $(OBJ_DIR)/srvcall.o $(OBJ_DIR)/smain.o
	$(CC) $(OBJ_DIR)/srvcall.o $(OBJ_DIR)/smain.o -o $(BIN_DIR)/server -pthread

$(BIN_DIR)/client: $(OBJ_DIR)/clntcall.o $(OBJ_DIR)/cmain.o
	$(CC) $(OBJ_DIR)/clntcall.o $(OBJ_DIR)/cmain.o -o $(BIN_DIR)/client

# Compilation Rules
$(OBJ_DIR)/cmain.o: $(SRC_DIR)/cmain.c
	$(CC) $(INCLUDE_FLAGS) $(CFLAGS)  $(SRC_DIR)/cmain.c $(OFLAGS) $(OBJ_DIR)/cmain.o

$(OBJ_DIR)/smain.o: $(SRC_DIR)/smain.c
	$(CC) $(INCLUDE_FLAGS) $(CFLAGS) $(SRC_DIR)/smain.c $(OFLAGS) $(OBJ_DIR)/smain.o

$(OBJ_DIR)/srvcall.o: $(SRC_DIR)/srvcall.c
	$(CC) $(INCLUDE_FLAGS) $(CFLAGS) $(SRC_DIR)/srvcall.c $(OFLAGS) $(OBJ_DIR)/srvcall.o

$(OBJ_DIR)/clntcall.o: $(SRC_DIR)/clntcall.c
	$(CC) $(INCLUDE_FLAGS) $(CFLAGS) $(SRC_DIR)/clntcall.c $(OFLAGS) $(OBJ_DIR)/clntcall.o

# Clean up
clean:
	@echo "Cleaning..."
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
