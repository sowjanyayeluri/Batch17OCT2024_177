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
SRC_FILES = $(SRC_DIR)/cmain.c $(SRC_DIR)/smain.c $(SRC_DIR)/srv_call.c $(SRC_DIR)/clnt_call.c
OBJ_FILES = $(OBJ_DIR)/cmain.o $(OBJ_DIR)/smain.o $(OBJ_DIR)/srv_call.o $(OBJ_DIR)/clnt_call.o

# Targets
all: $(EXC)

$(BIN_DIR)/server: $(OBJ_DIR)/srvcall.o $(OBJ_DIR)/smain.o
	$(CC) $(OBJ_DIR)/srv_call.o $(OBJ_DIR)/smain.o $(OFLAGS) $(BIN_DIR)/server -pthread

$(BIN_DIR)/client: $(OBJ_DIR)/clntcall.o $(OBJ_DIR)/cmain.o
	$(CC) $(OBJ_DIR)/clnt_call.o $(OBJ_DIR)/cmain.o $(OFLAGS) $(BIN_DIR)/client

# Compilation Rules
$(OBJ_DIR)/cmain.o: $(SRC_DIR)/cmain.c
	$(CC) $(INCLUDE_FLAGS) $(CFLAGS)  $(SRC_DIR)/cmain.c $(OFLAGS) $(OBJ_DIR)/cmain.o

$(OBJ_DIR)/smain.o: $(SRC_DIR)/smain.c
	$(CC) $(INCLUDE_FLAGS) $(CFLAGS) $(SRC_DIR)/smain.c $(OFLAGS) $(OBJ_DIR)/smain.o

$(OBJ_DIR)/srvcall.o: $(SRC_DIR)/srv_call.c
	$(CC) $(INCLUDE_FLAGS) $(CFLAGS) $(SRC_DIR)/srv_call.c $(OFLAGS) $(OBJ_DIR)/srv_call.o

$(OBJ_DIR)/clntcall.o: $(SRC_DIR)/clnt_call.c
	$(CC) $(INCLUDE_FLAGS) $(CFLAGS) $(SRC_DIR)/clnt_call.c $(OFLAGS) $(OBJ_DIR)/clnt_call.o

# Clean up
clean:
	@echo "Cleaning..."
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
