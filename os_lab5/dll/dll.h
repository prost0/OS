#ifndef DLL_H
#define DLL_H

#include <iostream>
#include <windows.h>

using namespace std;
extern "C" __declspec(dllexport) void Func();
extern "C" __declspec(dllexport) void F(int a);



#include <stdbool.h>

 typedef struct _bintree Bintree;
//typedef _bintree_leave Bintree_leave;
extern "C" __declspec(dllexport) Bintree* bintree_create(double value);
extern "C" __declspec(dllexport) void bintree_destroy(Bintree **root);
extern "C" __declspec(dllexport) void bintree_push(Bintree *root, double value);
extern "C" __declspec(dllexport) bool bintree_pop(Bintree **root, double value);
extern "C" __declspec(dllexport) void bintree_print(Bintree *root1, int lvl);
extern "C" __declspec(dllexport) Bintree *bintree_min(Bintree *root);
extern "C" __declspec(dllexport) Bintree *bintree_max(Bintree *root);
extern "C" __declspec(dllexport) //void bintree_print_correct(Bintree *bintree);
extern "C" __declspec(dllexport) //bool bintree_is_empty(Bintree *bintree);
extern "C" __declspec(dllexport) Bintree *bintree_get_branch_left(Bintree *root);
extern "C" __declspec(dllexport) Bintree *bintree_get_branch_right(Bintree *root);
extern "C" __declspec(dllexport) double bintree_get_value(Bintree *root);
#endif