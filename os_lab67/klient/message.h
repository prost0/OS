#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#define STR_SIZE 100

typedef struct {
	int clientId;
	int sum;
	int action;
	int receiverClientId;
	char message[STR_SIZE];
} MessageData;

#endif
