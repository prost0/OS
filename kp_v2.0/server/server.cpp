#include < iostream >
#include <stdlib.h>
#include <time.h>
#pragma comment( lib, "ws2_32.lib" )
#include < Windows.h >
#include < conio.h >
using namespace std;

#define MY_PORT    666 // ����, ������� ������� ������



const char send_empty = 0;
const char send_error = -1;
const char send_same = -2;
const char send_end = -3;
const char send_fox = -4;
const char send_again = -5;
const char open_field = -2;
const char fox = 1;
const char open_fox = -3;
const char max_foxes = 8;
const char border = 10;

// ������ ���������� ����������  ���� ��� ������ ������ ����� ������ �������   
char* cbuff = "common buffer"; // ����� ����� ��� ������ ��������� �� ���� ��������


							   // ������ ��� ������ ���������� ��������
							   // ������������� 
#define PRINTNUSERS if (nclients)\
  std::printf("%d user on-line\n",nclients);\
  else std::printf("No User on line\n");

							   // �������� �������, �������������
							   // �������������� �������������
DWORD WINAPI WorkWithClient(LPVOID client_socket);



// ���������� ���������� � ����������
// �������� ������������� 
int nclients = 0;
int field[border][border];
int counter = 1;
void FillField();

int main(int argc, char* argv[])
{
	FillField();
	char buff[1024];    // ����� ��� ��������� ����

	std::printf("TCP SERVER DEMO\n");

	// ��� 1 - ������������� ���������� �������
	// �.�. ������������ �������� ����������
	// �� ������������ �� ���������� ��������� ��
	// ������� �����, �������������
	// � ���������  �� ��������� WSADATA.
	// ����� ����� ��������� ���������� ����
	// ����������, ������, ����� ������ ���� �� �����
	// ������������ �������� (��������� WSADATA
	// �������� 400 ����)
	if (WSAStartup(0x0202, (WSADATA *)&buff[0]))
	{
		// ������!
		std::printf("Error WSAStartup %d\n",
			WSAGetLastError());
		return -1;
	}

	// ��� 2 - �������� ������
	SOCKET mysocket;
	// AF_INET     - ����� ���������
	// SOCK_STREAM  - ��������� ����� (�
	//      ���������� ����������)
	// 0      - �� ��������� ���������� TCP ��������
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		// ������!
		std::printf("Error socket %d\n", WSAGetLastError());
		WSACleanup();
		// �������������� ���������� Winsock
		return -1;
	}

	// ��� 3 ���������� ������ � ��������� �������
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(MY_PORT);
	// �� �������� � ������� �������!!!
	local_addr.sin_addr.s_addr = 0;
	// ������ ��������� �����������
	// �� ��� IP-������

	// �������� bind ��� ����������
	if (bind(mysocket, (sockaddr *)&local_addr,
		sizeof(local_addr)))
	{
		// ������
		std::printf("Error bind %d\n", WSAGetLastError());
		closesocket(mysocket);  // ��������� �����!
		WSACleanup();
		return -1;
	}

	// ��� 4 �������� �����������
	// ������ ������� � 0x100
	if (listen(mysocket, 20))
	{
		// ������
		std::printf("Error listen %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	std::printf("Waiting for connection\n");

	// ��� 5 ��������� ��������� �� �������
	SOCKET client_socket;    // ����� ��� �������
	sockaddr_in client_addr;    // ����� �������
								// (����������� ��������)

								// ������� accept ���������� �������� ������
								// ���������
	int client_addr_size = sizeof(client_addr);

	// ���� ���������� �������� �� ����������� ��
	// �������
	/* accept - ������ ���������� � �� ��� ����� ���������
	(�� ���� �� ��� ������- ���� ����������� ������)
	���� �� �������� ��������� ������ �� ����������*/
	while ((client_socket = accept(mysocket, (sockaddr *)
		&client_addr, &client_addr_size)))
	{
		nclients++;      // ����������� �������
						 // �������������� ��������

						 // �������� �������� ��� �����
		HOSTENT *hst;
		hst = gethostbyaddr((char *)
			&client_addr.sin_addr.s_addr, 4, AF_INET);

		// ����� �������� � �������
		std::printf("+%s [%s] new connect!\n",
			(hst) ? hst->h_name : "",
			inet_ntoa(client_addr.sin_addr));
		PRINTNUSERS

			DWORD thID;
		CreateThread(NULL, NULL, WorkWithClient, &client_socket, NULL, &thID);

	}
	return 0;
}


DWORD WINAPI WorkWithClient(LPVOID client_socket)
{
	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];
	char buff[1024] = "123124";
	
	while (1)
	{
		int bytes_recv = recv(my_sock, buff, sizeof(buff) - 1, 0);
		
		char x = atoi(&buff[0]) - 1, y = atoi(&buff[512]) - 1;

		if (x >= border || x < 0 || y >= border || y < 0) {
			send(my_sock, &send_error, sizeof(char), 0);
		} else if (field[y][x] == open_field || field[y][x] == open_fox) {
			send(my_sock, &send_same, sizeof(char), 0);
		} else if (field[y][x] == fox) {
			++counter;
			if (counter == max_foxes) {
				send(my_sock, &send_end, sizeof(char), 0);
				break;
			}
			send(my_sock, &send_fox, sizeof(char), 0);
			field[y][x] = open_fox;
			continue;
		} else if(field[y][x] == 0){
			field[y][x] = open_field;
			send(my_sock, &send_empty, sizeof(char), 0);
		}
		cout << (int)x << " " << (int)y << std::endl; // �������� ��������� ���� �������� � ������� �������
	}

	// ���� �� �����, �� ��������� ����� �� ����� ��
	// ������� ���������� �������� recv ������ �
	// ���������� �������� ���������
	nclients--; // ��������� ������� �������� ��������
	std::printf("-disconnect\n"); PRINTNUSERS

		// ��������� �����
		closesocket(my_sock);
	return 0;
}


void FillField() {
	for (int i = 0; i < border; ++i) {
		for (int j = 0; j < border; ++j) {
			field[i][j] = 0;
		}

	}
	for (int i = 0; i < max_foxes; ++i) {
		int rnd = rand() % 100;
		if (field[rnd / 10][rnd % 10] == fox) {
			--i;
			continue;
		}
		field[rnd / 10][rnd % 10] = fox;
		cout << rnd / 10 << "  " << rnd % 10 << endl;
	}
}