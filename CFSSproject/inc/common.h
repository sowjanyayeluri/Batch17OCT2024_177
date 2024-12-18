#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define CREDENTIALS_FILE "./data/UserCredentials.csv"
#define DETAILS_FILE "./data/UserDetails.csv"
#define LOG_FILE "./data/logMsgs.txt"
#define MAXBUFF 1024

typedef enum
{
	FATAL,
	DEBUG,
	INFO,
	WARNING
}LogLevel;


#endif
