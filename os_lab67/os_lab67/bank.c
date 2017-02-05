#include <stdio.h>
#include <stdlib.h>
#include "bank.h"

void InitClientBase(ClientBase *clientBase)
{
	clientBase->firstClient = NULL;
}

void AddNewClient(ClientBase *clientBase, int clientID)
{
	//if (clientBase->firstClient == NULL) {
	//	clientBase->firstClient = (Client *)malloc(sizeof(Client));
	//	clientBase->firstClient->id = clientID;
	//	clientBase->firstClient->sum = 0;
	//	clientBase->firstClient->nextClient = NULL;
	//	return;
	//}
	printf("add new client id is %d\n", clientID);
	Client **client = &(clientBase->firstClient);
	while (*client) {
		client = &(*client)->nextClient;
	}

	*client = (Client *)malloc(sizeof(Client));
	(*client)->id = clientID;
	(*client)->sum = 0;
	(*client)->nextClient = NULL;
}

void PrintClientBase(ClientBase *clientBase)
{
	printf("**********************************\n");
	Client *client = clientBase->firstClient;
	while (client) {
		printf("Id: %d\n", (int)client->id);
		printf("Id: %d Sum: %d\n", (int)client->id, client->sum);
		client = client->nextClient;
	}
	printf("**********************************\n");
}

Client *FindClient(ClientBase *clientBase, int clientint)
{
	if (!clientBase) {
		return 0;
	}
	Client *client = clientBase->firstClient;
	while (client && client->id != clientint) {
		client = client->nextClient;
	}
	return client;
}

void IncreaseClientAccount(Client *client, int sum)
{
	client->sum += sum;
}

void DecreaseClientAccount(Client *client, int sum)
{
	client->sum -= sum;
}

int ClientHasEnoughMoney(Client *client, int sum)
{
	return client->sum >= sum;
}

void PutMoney(int clientint, int sum, ClientBase *clientBase)
{
	printf("put money id is %d\n", clientint);
	Client *client = FindClient(clientBase, clientint);

	if (client) {
		IncreaseClientAccount(client, sum);
	}
	else {
		AddNewClient(clientBase, clientint);
		client = FindClient(clientBase, clientint);
		IncreaseClientAccount(client, sum);
	}
}

int GetMoney(int clientint, int sum, ClientBase *clientBase)
{
	Client *client = FindClient(clientBase, clientint);

	if (client) {
		if (ClientHasEnoughMoney(client, sum)) {
			DecreaseClientAccount(client, sum);
			return SUCCESS;
		}
		else {
			return NOT_ENOUGH_MONEY;
		}
	}
	else {
		return NOT_CLIENT;
	}
}

int SendMoney(int clientSenderID, ID clientReceiverID, int sum, ClientBase *clientBase)
{
	Client *clientSender = FindClient(clientBase, clientSenderID);
	Client *clientReceiver = FindClient(clientBase, clientReceiverID);

	if (clientSender) {
		if (clientReceiver) {
			if (ClientHasEnoughMoney(clientSender, sum)) {
				DecreaseClientAccount(clientSender, sum);
				IncreaseClientAccount(clientReceiver, sum);
				return SUCCESS;
			}
			else {
				return NOT_ENOUGH_MONEY;
			}

		}
		else {
			return RECEIVER_NOT_CLIENT;
		}

	}
	else {
		return NOT_CLIENT;
	}
}

int CheckAccont(ID clientID, ClientBase *clientBase)
{
	Client *client = FindClient(clientBase, clientID);

	if (client) {
		return client->sum;
	}
	else {
		return NOT_CLIENT;
	}
}


void DestroyClientBase(ClientBase *clientBase)
{
	Client *client = clientBase->firstClient;
	while (client) {
		Client *temp = client->nextClient;
		free(client);
		client = temp;
	}
}
