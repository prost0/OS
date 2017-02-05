#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "zmq.h"
#include "bank.h"
#include "message.h"


int main(int argc, char const *argv[])
{
	ClientBase clientBase;
	InitClientBase(&clientBase);

	void *context = zmq_ctx_new();
	void *respondSocket = zmq_socket(context, ZMQ_REP);

	char adress[25];
	printf("Enter thee bank adress: ");
	int bankAdress;

	scanf("%d", &bankAdress);
	sprintf(adress, "%s%d", "tcp://*:", bankAdress);

	zmq_bind(respondSocket, adress);

	int maxCount;
	printf("Enter maximum number of operations to be performed without stopping: ");
	scanf("%d", &maxCount);

	int counter = 0;

	while (1) {
		if (counter >= maxCount) {
			printf("Server is interrupted. To restore press 'r'. To turn off press 'q'\n");
			char c;
			while ((c = getchar()) != 'r' && c != 'q');
			if (c == 'q') break;
			printf("Server is running\n");
			counter = 0;
		}
		counter++;
		zmq_msg_t message;

		zmq_msg_init(&message);
		zmq_msg_recv(&message, respondSocket, 0);
		MessageData *md = (MessageData *)zmq_msg_data(&message);
		zmq_msg_close(&message);

		char info[STR_SIZE];

		switch (md->action) {
		case 1: {

			printf("Put money into the account id: %d\n", md->clientId);
			PutMoney((int)md->clientId, md->sum, &clientBase);

			strcpy(info, "Operation was completed successfully\0");

			memcpy(md->message, info, strlen(info) + 1);

			zmq_msg_init_size(&message, sizeof(MessageData));
			memcpy(zmq_msg_data(&message), md, sizeof(MessageData));
			zmq_msg_send(&message, respondSocket, 0);
			zmq_msg_close(&message);


			break;
		}
		case 2: {
			printf("Get money from the account id: %d\n", md->clientId);

			int code = GetMoney(md->clientId, md->sum, &clientBase);
			if (code == SUCCESS) {
				printf("Success\n");
				strcpy(info, "Operation was completed successfully\0");
			}
			else if (code == NOT_ENOUGH_MONEY) {
				printf("Not enough money\n");
				strcpy(info, "You not enough money\0");

			}
			else {
				printf("Not bank client\n");
				strcpy(info, "You are not a client of our bank\0");

			}
			memcpy(md->message, info, strlen(info) + 1);

			zmq_msg_init_size(&message, sizeof(MessageData));
			memcpy(zmq_msg_data(&message), md, sizeof(MessageData));
			zmq_msg_send(&message, respondSocket, 0);
			zmq_msg_close(&message);

			break;
		}
		case 3: {
			printf("Send money from account id: %d to account id: %d\n", md->clientId, md->receiverClientId);
			int code = SendMoney(md->clientId, md->receiverClientId, md->sum, &clientBase);
			if (code == SUCCESS) {
				printf("Success\n");
				strcpy(info, "Operation was completed successfully\0");

			}
			else if (code == NOT_ENOUGH_MONEY) {
				printf("Not enough money\n");
				strcpy(info, "You not enough money\0");

			}
			else if (code == RECEIVER_NOT_CLIENT) {
				printf("Receiver not bank client\n");
				strcpy(info, "Receiver is not a client of our bank\0");


			}
			else {
				printf("Not bank client\n");
				strcpy(info, "You are not a client of our bank\0");

			}

			memcpy(md->message, info, strlen(info) + 1);

			zmq_msg_init_size(&message, sizeof(MessageData));
			memcpy(zmq_msg_data(&message), md, sizeof(MessageData));
			zmq_msg_send(&message, respondSocket, 0);
			zmq_msg_close(&message);
			break;
		}

		case 4: {
			printf("Check account id: %d\n", md->clientId);

			int code = CheckAccont(md->clientId, &clientBase);
			if (code == NOT_CLIENT) {
				printf("Not bank client\n");
				char info[] = "You are not a client of our bank\0";

			}
			else {
				printf("Client sum is %d\n", code);
				sprintf(info, "%s%d%c", "Your account is ", code, '\0');

			}

			memcpy(md->message, info, strlen(info) + 1);

			zmq_msg_init_size(&message, sizeof(MessageData));
			memcpy(zmq_msg_data(&message), md, sizeof(MessageData));
			zmq_msg_send(&message, respondSocket, 0);
			zmq_msg_close(&message);

			break;
		}

		}
		zmq_msg_close(&message);
		PrintClientBase(&clientBase);


	}

	zmq_close(respondSocket);
	zmq_ctx_destroy(context);

	DestroyClientBase(&clientBase);

	return 0;
}
