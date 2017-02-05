#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "dll.h"

using namespace std;

typedef struct _bintree Bintree;


struct _bintree {
	double value;
	Bintree *left;
	Bintree *right;
};

typedef Bintree*(* LPFNDLLFUNCCREATE)(double T);
typedef void(* LPFNDLLFUNCPUSH)(Bintree *root, double value);
typedef void(* LPFNDLLFUNCPRINT)(Bintree *root, int value);
typedef bool(* LPFNDLLFUNCPOP)(Bintree **root, double value);


int main(void)
{
	HINSTANCE hLib = LoadLibrary(L"dll.dll");
	if (hLib == NULL)
	{
		cout << "Unable to load library!" << endl;
		_getch();
		return 1;
	}
	cout << "Library is loaded.." << endl;
	
	Bintree *tree = NULL;
	LPFNDLLFUNCCREATE bintree_create = (LPFNDLLFUNCCREATE)GetProcAddress((HMODULE)hLib, "bintree_create");
	LPFNDLLFUNCPUSH bintree_push = (LPFNDLLFUNCPUSH)GetProcAddress((HMODULE)hLib, "bintree_push");
	LPFNDLLFUNCPRINT bintree_print = (LPFNDLLFUNCPRINT)GetProcAddress((HMODULE)hLib, "bintree_print");
	LPFNDLLFUNCPOP bintree_pop = (LPFNDLLFUNCPOP)GetProcAddress((HMODULE)hLib, "bintree_pop");
	if (bintree_create == NULL || bintree_push == NULL || bintree_print == NULL || bintree_pop == NULL)
	{
		printf("Unable to load function(s).");
		FreeLibrary((HMODULE)hLib);
		return 1;
	}
	
	printf("-------------------------------------------\n");
	printf("Commands:\n");
	printf("a <value> - push to bintree\n");
	printf("d <value>- pop from bintree\n");
	printf("p - print bintree\n");
	printf("q - exit\n");
	printf("-------------------------------------------\n");

	while (true) {
		//printf("%d\n", cnt);
		//cnt++;
		char cmd;
		double value = 0;
		bool is_finished = false;
		scanf("%c", &cmd);
		switch (cmd) {
		case 'q':
			is_finished = true;
			break;
		case 'a':
			scanf("%lf", &value);
			if (tree == NULL) {
				tree = bintree_create(value);
			}
				
			else {
				bintree_push(tree, value);
			}
				
			break;
		case 'd':
			if (tree == NULL) {
				printf("Tree is empty");
				break;
			}
			scanf("%lf", &value);
			bintree_pop(&tree, value);
			break;
		case 'p':
			bintree_print(tree, 0);
			break;
		case '\n':
			break;
		default:
			printf("Invalid command `%c`\n", cmd);
			break;
		}
		if (is_finished) break;
	}
	printf("Goodbye!\n");

	return 0;
}