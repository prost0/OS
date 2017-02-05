#ifndef _BANK_H_
#define _BANK_H_

#define SUCCESS 1
#define NOT_ENOUGH_MONEY -1
#define NOT_CLIENT -2
#define RECEIVER_NOT_CLIENT -3
#include <stdint.h>

typedef int ID;

typedef struct client {
	int id;
	int sum;
	struct client *nextClient;
} Client;

typedef struct {
	Client *firstClient;
} ClientBase;

void InitClientBase(ClientBase *clientBase);
void AddNewClient(ClientBase *clientBase, int clientint);
void PrintClientBase(ClientBase *clientBase);
Client *FindClient(ClientBase *clientBase, int clientint);
void DestroyClientBase(ClientBase *clientBase);

void IncreaseClientAccount(Client *client, int sum);
void DecreaseClientAccount(Client *client, int sum);
int ClientHasEnoughMoney(Client *client, int sum);
void PutMoney(int clientint, int sum, ClientBase *clientBase);
int GetMoney(int clientint, int sum, ClientBase *clientBase);
int SendMoney(int clientSenderID, ID clientReceiverID, int sum, ClientBase *clientBase);
int CheckAccont(ID clientID, ClientBase *clientBase);

#endif

