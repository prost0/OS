#include < iostream >
#pragma comment( lib, "ws2_32.lib" )
#include < Windows.h >
#include < conio.h >

using namespace std;
#define PORT 666
#define SERVERADDR "127.0.0.1"



const char error = -1;
const char same_field = -2;
const char the_end = -3;
const char fox = -4;
const char send_end = 2;
const char send_again = 1;
const char border = 10;

int still_not_found;
int number_of_steps;
int best_result;


void Print_menu();
void Print_table();
void Print_new_char(int x, int y, int c);
void Print_text(char* str);

int main(int argc, char* argv[])
{
	//Print_menu();
	char buff[1024];
	std::printf("TCP DEMO CLIENT\n");

	// Шаг 1 - инициализация библиотеки Winsock
	if (WSAStartup(0x202, (WSADATA *)&buff[0]))
	{
		std::printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}

	// Шаг 2 - создание сокета
	SOCKET my_sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
		std::printf("Socket() error %d\n", WSAGetLastError());
		return -1;
	}

	// Шаг 3 - установка соединения

	// заполнение структуры sockaddr_in
	// указание адреса и порта сервера
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	HOSTENT *hst;

	// преобразование IP адреса из символьного в
	// сетевой формат
	if (inet_addr(SERVERADDR) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else
		// попытка получить IP адрес по доменному
		// имени сервера
		if (hst = gethostbyname(SERVERADDR))
			// hst->h_addr_list содержит не массив адресов,
			// а массив указателей на адреса
			((unsigned long *)&dest_addr.sin_addr)[0] =
			((unsigned long **)hst->h_addr_list)[0][0];
		else
		{
			std::printf("Invalid address %s\n", SERVERADDR);
			closesocket(my_sock);
			WSACleanup();
			return -1;
		}

	// адрес сервера получен – пытаемся установить
	// соединение 
	if (connect(my_sock, (sockaddr *)&dest_addr,
		sizeof(dest_addr)))
	{
		std::printf("Connect error %d\n", WSAGetLastError());
		return -1;
	}

	std::printf("Connection with %s successfully done\n\
    Type quit for quit\n\n", SERVERADDR);

	buff[512] = 0;
	Print_table();
	printf("\n");
	while (1)
	{
		printf("\t\t\t\t\t\t\t\tx >> ");
		//std::printf("x = ");
		scanf("%s", &buff[0]);
		printf("\t\t\t\t\t\t\t\tx >> ");
		//std::printf("y = ");
		scanf("%s", &buff[512]);
		// проверка на "quit"
		if (!strcmp(&buff[0], "quit\n"))
		{
			// Корректный выход
			std::printf("Exit...");
			closesocket(my_sock);
			WSACleanup();
			return 0;
		}

		// передаем строку клиента серверу
		send(my_sock, &buff[0], 1023, 0);
		int x = atoi(&buff[0]), y = atoi(&buff[512]);

		char respond = 0;
		recv(my_sock, &respond, sizeof(char), 0);

		if (respond == error) {
			Print_text("You entered wrong coordinates. Try again.");
		}
		else if (respond == same_field) {
			Print_text("You tried to open the same field. Try again.");
		}
		else if (respond == fox) {
			
			Print_new_char(x, y, fox);
			++number_of_steps;
			--still_not_found;
			//Print_text("found");//Print_new_char(msg->x, msg->y, fox);
		}
		else if (respond == the_end) {
			++number_of_steps;
			//Print_text("found end"); //Print_the_end(msg->x, msg->y);
			break;
		} else {
			Print_new_char(x, y, 'o');
			//Print_text("Empty");
		}

		++number_of_steps;
	}

	printf("Recv error %d\n", WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
	return -1;
}

void Print_table() {
	system("cls");
	printf("\n\n\n\t\t\t\t\t   FOXHUNTING\n\n\n\t\t\t\t    ");
	for (int i = 0; i < border; ++i)
		printf(i != 0 ? "  %d" : " %d", i + 1);
	printf("\n\t\t\t\t   +-");
	for (int i = 1; i < border; ++i)
		printf("---");
	printf("--+\n");
	for (int i = 0; i < border; ++i) {
		if (i != 5 && i != 7 && i != 9) {
			i != 9 ? printf("\t\t\t\t %d |", i + 1) : printf("\t\t\t\t%d |", i + 1);
		}
		else if (i == 5) {
			printf("\t\t %d |", i + 1);
		}
		else if (i == 7) {
			printf("\t\t %d |", i + 1);
		}
		else {
			printf("\t\t\t %d|", i + 1);
		}

		for (int j = 0; j < border; ++j)
			printf(j != 0 ? "  ." : " .");
		if (i != 4 && i != 6 && i != 8)
			printf(" |\n");
		else if (i == 4)
			printf(" |\t\tNot found foxes: %d", still_not_found);
		else if (i == 6)
			printf(" |\t\tNumber of steps: %d", number_of_steps);
		else if (i == 8)
			printf(" |\t\tBest result: %d", best_result);
	}
	printf("\t\t\t\t   +-");
	for (int i = 1; i < border; ++i)
		printf("---");
	printf("--+\n");
}

void Print_menu() {
	int length = strlen("WELCOME TO THE GAME: FOXHUNTING!\0");
	printf("\n\n\n\n\n\n\t\t\t\t+---");
	for (int i = 0; i < length; ++i)
		printf("-");
	printf("---+");
	printf("\n\t\t\t\t|***");
	for (int i = 0; i < length; ++i)
		printf("*");
	printf("***|\n");
	printf("\t\t\t\t|*+-");
	for (int i = 0; i < length; ++i)
		printf("-");
	printf("-+*|\n");
	printf("\t\t\t\t|*| ");
	for (int i = 0; i < length; ++i)
		printf(" ");
	printf(" |*|\n");
	printf("\t\t\t\t|*| WELCOME TO THE GAME: FOXHUNTING! |*|\n");
	printf("\t\t\t\t|*| ");
	for (int i = 0; i < length; ++i)
		printf(" ");
	printf(" |*|\n");
	printf("\t\t\t\t|*+-");
	for (int i = 0; i < length; ++i)
		printf("-");
	printf("-+*|\n");
	printf("\t\t\t\t|***");
	for (int i = 0; i < length; ++i)
		printf("*");
	printf("***|\n");
	printf("\t\t\t\t+---");
	for (int i = 0; i < length; ++i)
		printf("-");
	printf("---+\n");
	_getch();

	system("cls");
	printf("Choose 1 to continue or 2 to read the rules \n\n");

	int cnt = 0;
	int rules;
rul:
	//rules = getchar();
	scanf("%d", &rules);
	if (rules != 1 && rules != 2) {
		printf("Wrong choice. Choose 1 or 2.\n\n");
		rules = getchar();
		if (rules == 1 || rules == 2)
			goto after;
		goto rul;
	}

after:
	if (rules == 1)
		return;

	system("cls");
	printf("On the field (10x10) random anonymous way for a player are placed eight \"fox\".\n\n");
	printf("The player outputs its position by entering the coordinates.\n\n");
	printf("In return, he receives the amount of \"fox\", which is located in the same vertical, horizontal and diagonal to the specified cell.\n\n");
	printf("If the player's location coincided with the position of the \"fox\", it is considered to be found.\n\n");
	printf("The game continues until all the \"fox\" will be found.\n\n");
	printf("Now you know everything about what need to know... Let's start the game!\n\n");
	printf("\n\n\n\t\t\t\t\t(Press any button to continue...)\n");
	_getch();
}

void Print_text(char* str) {
	COORD coord;
	coord.X = 35;
	coord.Y = 20;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%90c", ' ');
	coord.Y = 21;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%90c", ' ');
	coord.X = 17;
	coord.Y = 20;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%s", str);
	coord.X = 17;
	coord.Y = 22;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%90c", ' ');
	coord.Y = 23;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%90c", ' ');
	coord.X = 0;
	coord.Y = 22;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Clear_under_the_table() {
	COORD coord;
	coord.X = 17;
	coord.Y = 20;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%90c", ' ');
	coord.X = 17;
	coord.Y = 22;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%90c", ' ');
	coord.X = 17;
	coord.Y = 23;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%90c", ' ');
	coord.X = 17;
	coord.Y = 21;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%90c", ' ');
	coord.X = 0;
	coord.Y = 20;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Print_new_char(int x, int y, int c) {
	COORD coord;
	coord.X = 35 + 3 * x - 1;
	coord.Y = 7 + y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	if (c == fox) {
		printf("%c", 'x');
		coord.X = 17;
		coord.Y = 13;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		printf("%d", still_not_found);
	}
	else
		printf("%c", 'o');
	coord.X = 17;
	coord.Y = 15;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%d", number_of_steps);
	Clear_under_the_table();
}

void Print_end(int x, int y) {
	Print_new_char(x, y, fox);
	Clear_under_the_table();
	COORD coord;
	coord.X = 42;
	coord.Y = 20;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("You won! Congratulations!");
	coord.X = 28;
	coord.Y = 22;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("Do you want to play again? ( 1 - yes / 2 - no )");
	coord.X = 28;
	coord.Y = 24;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf(">> ");
}

void Print_end_error() {
	COORD coord;
	coord.X = 46;
	coord.Y = 24;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%10c", ' ');
	coord.X = 46;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("You entered wrong value (only 1 or 2). Try again.");
	coord.X = 55;
	coord.Y = 26;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%100c", ' ');
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("---> ");
}