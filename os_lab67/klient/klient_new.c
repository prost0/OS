#include <stdio.h>
#include "zmq.h"
#include "message.h"
#include <inttypes.h>

int main(int argc, char const *argv[])
{
	void *context = zmq_ctx_new();
	
	int id;
	printf("Enter login of client: ");
	scanf("%d", &id);
	
	char adress[25];
	printf("Enter thee bank adress: ");
	int bankAdress;
	scanf("%d", &bankAdress);
	
	sprintf(adress, "%s%d", "tcp://localhost:", bankAdress);
	
	void *senderSocket = zmq_socket(context, ZMQ_REQ);
	zmq_connect(senderSocket, adress);
	
	printf("**********************************\n");
	printf("1) Put money into the account\n");
	printf("2) Get  money from the account\n");
	printf("3) Send money to another account \n");
	printf("4) Check bank account \n");
	printf("5) Leave the bank\n");
	printf("**********************************\n");
	
	int action;
	
	do {
		printf("Select action: ");
		scanf("%d", &action);
	
		MessageData md;
		md.action = action;
		md.clientId = id;
	
		switch (action) {
	
		case 1: {
			int sum;
			printf("Enter the sum: ");
			scanf("%d", &sum);
	
			md.sum = sum;
	
			zmq_msg_t message;
			zmq_msg_init_size(&message, sizeof(MessageData));
			memcpy(zmq_msg_data(&message), &md, sizeof(MessageData));
			zmq_msg_send(&message, senderSocket, 0);
			zmq_msg_close(&message);
	
			zmq_msg_init(&message);
			zmq_msg_recv(&message, senderSocket, 0);
			MessageData *md = (MessageData *)zmq_msg_data(&message);
			printf("%s\n", md->message);
			zmq_msg_close(&message);
			break;
	
		}
		case 2: {
			int sum;
			printf("Enter the sum: ");
			scanf("%d", &sum);
	
			md.sum = sum;
	
			zmq_msg_t message;
			zmq_msg_init_size(&message, sizeof(MessageData));
			memcpy(zmq_msg_data(&message), &md, sizeof(MessageData));
			zmq_msg_send(&message, senderSocket, 0);
			zmq_msg_close(&message);
	
			zmq_msg_init(&message);
			zmq_msg_recv(&message, senderSocket, 0);
			MessageData *md = (MessageData *)zmq_msg_data(&message);
			printf("%s\n", md->message);
			zmq_msg_close(&message);
	
			break;
		}
	
		case 3: {
			int receiverClientId;
			printf("Enter receiver id: ");
			scanf("%d", &receiverClientId);
	
			int sum;
			printf("Enter the sum: ");
			scanf("%d", &sum);
	
			md.sum = sum;
			md.receiverClientId = receiverClientId;
	
			zmq_msg_t message;
			zmq_msg_init_size(&message, sizeof(MessageData));
			memcpy(zmq_msg_data(&message), &md, sizeof(MessageData));
			zmq_msg_send(&message, senderSocket, 0);
			zmq_msg_close(&message);
	
			zmq_msg_init(&message);
			zmq_msg_recv(&message, senderSocket, 0);
			MessageData *md = (MessageData *)zmq_msg_data(&message);
			printf("%s\n", md->message);
			zmq_msg_close(&message);
	
			break;
		}
	
		case 4: {
			zmq_msg_t message;
			zmq_msg_init_size(&message, sizeof(MessageData));
			memcpy(zmq_msg_data(&message), &md, sizeof(MessageData));
			zmq_msg_send(&message, senderSocket, 0);
			zmq_msg_close(&message);
	
			zmq_msg_init(&message);
			zmq_msg_recv(&message, senderSocket, 0);
			MessageData *md = (MessageData *)zmq_msg_data(&message);
			printf("%s\n", md->message);
			zmq_msg_close(&message);
	
			break;
		}
	
		case 5: {
			break;
		}
		default: {
			printf("Incorrect action\n");
			break;
		}
		}
	
	
	} while (action != 5);
	
	zmq_close(senderSocket);
	zmq_ctx_destroy(context);
	
	return 0;
}
