#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdlib.h>

#define FALSE 0
#define MAX_THREADS 9
#define number_of_cells  9

FILE *f;
HANDLE hMutex;
DWORD WINAPI ThreadFunctionForCell(LPVOID lpParam);
char field[number_of_cells];
char who_turn, who_not_turn;
int num_pr1 = 0, num_pr2 = 0, num_pr3;
int pr1[number_of_cells];
int pr2[number_of_cells];
int pr3[number_of_cells];

int HaveWinner() {
	if ((field[0] == field[1] && field[0] == field[2] && (field[0] == 'x' || field[0] == 'o')) ||
		(field[3] == field[4] && field[5] == field[3] && (field[3] == 'x' || field[3] == 'o')) ||
		(field[6] == field[7] && field[8] == field[6] && (field[6] == 'x' || field[6] == 'o')) ||
		(field[0] == field[3] && field[6] == field[0] && (field[0] == 'x' || field[0] == 'o')) ||
		(field[1] == field[4] && field[7] == field[4] && (field[1] == 'x' || field[1] == 'o')) ||
		(field[2] == field[5] && field[8] == field[5] && (field[2] == 'x' || field[2] == 'o')) ||
		(field[0] == field[4] && field[8] == field[0] && (field[0] == 'x' || field[0] == 'o')) ||
		(field[2] == field[4] && field[6] == field[2] && (field[2] == 'x' || field[2] == 'o'))) {
		return 1;
	}
	return 0;
}

void Example3()
{
	if (HaveWinner()) {
		fprintf(f,"Game over\n");
		return;
	}
	HANDLE  hThreadArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	hMutex = CreateMutex(NULL, FALSE, NULL);

	for (int i = 0; i < MAX_THREADS; i++)
	{
		hThreadArray[i] = CreateThread(NULL,
			0,
			ThreadFunctionForCell,
			i,
			NULL,
			&dwThreadIdArray[i]);
	}
	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
	for (int i = 0; i < MAX_THREADS; i++) CloseHandle(hThreadArray[i]);
	
	if (num_pr1 > 0) {
		for (int i = 0; i < num_pr1; i++) {
			fprintf(f ,"Cell %d\n", pr1[i]);
		}
	}
	else if(num_pr2 > 0) {
		for (int i = 0; i < num_pr2; i++) {
			fprintf(f, "Cell %d\n", pr2[i]);
		}
	}
	else {
		if (field[4] != 'x' && field[4] != 'o') {
			fprintf(f, "Cell 4\n");
		}
		else {
			int already_find = 0;
			if (field[0] != 'x' && field[0] != 'o') {
				fprintf(f, "Cell %d\n", 0);
				already_find = 1;
			}
			if (field[2] != 'x' && field[2] != 'o') {
				fprintf(f, "Cell %d\n", 2);
				already_find = 1;
			}
			if (field[6] != 'x' && field[6] != 'o') {
				fprintf(f, "Cell %d\n", 6);
				already_find = 1;
			}
			if (field[8] != 'x' && field[8] != 'o') {
				fprintf(f,"Cell %d\n", 8);
				already_find = 1;
			}
			if (!already_find) {
				for (int i = 0; i < number_of_cells; ++i) {
					if (field[i] != 'x' && field[i] != 'o') {
						fprintf(f, "Cell %d\n", i);
					}
				}
			}
			
		}
	}
}

DWORD WINAPI ThreadFunctionForCell(LPVOID lpParam)
{
	WaitForSingleObject(hMutex, INFINITE);
	int myNumber = (int)lpParam;
	if (field[myNumber] == 'x' || field[myNumber] == 'o') {
		return myNumber;
	}
	switch (myNumber) {
	case 0:
		if ((field[1] == field[2] && field[1] == who_turn)
		 || (field[3] == field[6] && field[3] == who_turn)
		 || (field[4] == field[8] && field[4] == who_turn)) {
			pr1[num_pr1++] = 0;
		} else if((field[1] == field[2] && field[1] == who_not_turn)
			|| (field[3] == field[6] && field[3] == who_not_turn)
			|| (field[4] == field[8] && field[4] == who_not_turn)) {
			pr2[num_pr2++] = 0;
		}
		break;
	case 1:
		if ((field[0] == field[2] && field[0] == who_turn)
			|| (field[4] == field[7] && field[4] == who_turn)) {
			pr1[num_pr1++] = 1;
		}
		else if ((field[0] == field[2] && field[0] == who_not_turn)
			|| (field[4] == field[7] && field[4] == who_not_turn)) {
			pr2[num_pr2++] = 1;
		}
		break;
	case 2:
		if ((field[0] == field[1] && field[1] == who_turn)
			|| (field[5] == field[8] && field[8] == who_turn)
			|| (field[4] == field[6] && field[6] == who_turn)) {
			pr1[num_pr1++] = 2;
		}
		else if ((field[0] == field[1] && field[1] == who_not_turn)
			|| (field[5] == field[8] && field[8] == who_not_turn)
			|| (field[4] == field[6] && field[6] == who_not_turn)) {
			pr2[num_pr2++] = 2;
		}
		break;
	case 3:
		if ((field[0] == field[6] && field[0] == who_turn)
			|| (field[6] == field[5] && field[5] == who_turn)) {
			pr1[num_pr1++] = 3;
		}
		else if ((field[0] == field[6] && field[0] == who_not_turn)
			|| (field[6] == field[5] && field[5] == who_not_turn)) {
			pr2[num_pr2++] = 3;
		}
		break;
	case 4:
		if ((field[0] == field[8] && field[8] == who_turn)
			|| (field[2] == field[6] && field[6] == who_turn)
			|| (field[3] == field[5] && field[5] == who_turn)
			|| (field[1] == field[7] && field[7] == who_turn)) {
			pr1[num_pr1++] = 4;
		}
		else if ((field[0] == field[8] && field[8] == who_not_turn)
			|| (field[2] == field[6] && field[6] == who_not_turn)
			|| (field[3] == field[5] && field[5] == who_not_turn)
			|| (field[1] == field[7] && field[7] == who_not_turn)) {
			pr2[num_pr2++] = 4;
		}
		break;
	case 5:
		if ((field[3] == field[4] && field[4] == who_turn)
			|| (field[8] == field[2] && field[2] == who_turn)) {
			pr1[num_pr1++] = 5;
		}
		else if ((field[3] == field[4] && field[4] == who_not_turn)
			|| (field[8] == field[2] && field[2] == who_not_turn)) {
			pr2[num_pr2++] = 5;
		}
		break;
	case 6:
		if ((field[0] == field[3] && field[3] == who_turn)
			|| (field[7] == field[8] && field[8] == who_turn)
			|| (field[4] == field[2] && field[2] == who_turn)) {
			pr1[num_pr1++] = 6;
		}
		else if ((field[0] == field[3] && field[3] == who_not_turn)
			|| (field[7] == field[8] && field[8] == who_not_turn)
			|| (field[4] == field[2] && field[2] == who_not_turn)) {
			pr2[num_pr2++] = 6;
		}
		break;
	case 7:
		if ((field[1] == field[4] && field[4] == who_turn)
			|| (field[8] == field[6] && field[6] == who_turn)) {
			pr1[num_pr1++] = 7;
		}
		else if ((field[1] == field[4] && field[4] == who_not_turn)
			|| (field[8] == field[6] && field[6] == who_not_turn)) {
			pr2[num_pr2++] = 7;
		}
		break;
	case 8:
		if ((field[0] == field[4] && field[4] == who_turn)
			|| (field[2] == field[5] && field[5] == who_turn)
			|| (field[7] == field[6] && field[6] == who_turn)) {
			pr1[num_pr1++] = 8;
		}
		else if ((field[0] == field[4] && field[4] == who_not_turn)
			|| (field[2] == field[5] && field[5] == who_not_turn)
			|| (field[7] == field[6] && field[6] == who_not_turn)) {
			pr2[num_pr2++] = 8;
		}
		break;
	}
	ReleaseMutex(hMutex);
	return myNumber;
}



int main()
{

	f = fopen("result.txt", "a");


	printf("Enter the field:\n");
	for (int i = 0; i < number_of_cells; ++i) {
		field[i] = getchar();
		if (field[i] == '\n' || field[i] == ' ') {
			--i;
		}
	}

	for (int i = 0; i < number_of_cells; ++i) {
		fprintf_s(f, "%c ", field[i]);
		if((i + 1) % 3 == 0) {
			fprintf(f, "\n");
		}
	}

	printf("Enter whose turn:\n");
	while (scanf_s("%c", &who_turn) != 1  || !(who_turn == 'o' || who_turn == 'x')) {}
	if (who_turn == 'o') {
		who_not_turn = 'x';
	}
	else {
		who_not_turn = 'o';
	}

	Example3();
	system("pause");
	return 0;
}





/*

int turn = 0;// 0 turn 'X', 1 turn 'O'
for (int i = 0; i < number_of_cells; ++i) {
if (field[i] == 'x') {
turn++;
}
else if (field[i] == 'o') {
turn--;
}
}
who_turn = 'x';
if (turn == 1) {
who_turn == 'o';
}*/