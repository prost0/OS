#include "dll.h"


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>


#pragma pack(push,1)
struct _bintree {
	double value;
	Bintree *left;
	Bintree *right;
};



extern "C" __declspec(dllexport) Bintree* bintree_create(double value)
{
	Bintree *nw = (Bintree *)calloc(1, sizeof(Bintree));
	if (nw == NULL) {
		printf("Few memory\n");
	}
	nw->left = NULL;
	nw->right = NULL;
	nw->value = value;
	return nw;
}
extern "C" __declspec(dllexport) void bintree_destroy(Bintree **root)
{
	if ((*root)->left != NULL)
		bintree_destroy(&((*root)->left));
	(*root)->left = NULL;

	if ((*root)->right != NULL)
		bintree_destroy(&((*root)->right));
	(*root)->right = NULL;

	free(*root);
	*root = NULL;
}
extern "C" __declspec(dllexport) void bintree_push(Bintree *root, double value)
{
	if (value < root->value) {
		if (root->left != NULL) {
			bintree_push(root->left, value);
			return;
		}
		root->left = bintree_create(value);
	}
	else {
		if (root->right != NULL) {
			bintree_push(root->right, value);
			return;
		}
		root->right = bintree_create(value);
	}
}
extern "C" __declspec(dllexport) bool bintree_pop(Bintree **root, double value)
{
	if (value < (*root)->value) {
		if ((*root)->left != NULL)
			if (bintree_pop(&((*root)->left), value))
				(*root)->left = NULL;
		return false;
	}
	if (value >(*root)->value) {
		if ((*root)->right != NULL)
			if (bintree_pop(&((*root)->right), value))
				(*root)->right = NULL;
		return false;
	}

	if ((*root)->left == NULL && (*root)->right == NULL) {
		bintree_destroy(root);
		return true;
	}
	if ((*root)->left == NULL) {
		Bintree *tmp = (*root)->right;
		(*root)->value = tmp->value;
		(*root)->left = tmp->left;
		(*root)->right = tmp->right;
		tmp->left = NULL;
		tmp->right = NULL;
		bintree_destroy(&tmp);
		return false;
	}
	if ((*root)->right == NULL) {
		Bintree *tmp = (*root)->left;
		(*root)->value = tmp->value;
		(*root)->left = tmp->left;
		(*root)->right = tmp->right;
		tmp->left = NULL;
		tmp->right = NULL;
		bintree_destroy(&tmp);
		return false;
	}
	Bintree *min = bintree_min((*root)->right);
	(*root)->value = min->value;
	Bintree *tmp = (*root)->right;
	if (tmp == min) {
		(*root)->right = NULL;
		bintree_destroy(&tmp);
		return false;
	}
	while (tmp->left != min)
		tmp = tmp->left;
	bintree_destroy(&(tmp->left));
	return false;
}
extern "C" __declspec(dllexport) void bintree_print(Bintree *root1, int lvl)
{
	if (root1 == NULL) {
		printf("Tree is empty\n");
		return;
	}
	for (int i = 0; i < lvl; i++)
		printf("  ");
	printf("%lf\n", root1->value);
	if (root1->left != NULL)
		bintree_print(root1->left, lvl + 1);
	if (root1->right != NULL)
		bintree_print(root1->right, lvl + 1);
}
/*bool bintree_is_empty(Bintree *bintree) {
}*/
extern "C" __declspec(dllexport) Bintree *bintree_min(Bintree *root)
{
	if (root->left == NULL)
		return root;
	return bintree_min(root->left);
}
extern "C" __declspec(dllexport) Bintree *bintree_max(Bintree *root)
{
	if (root->right == NULL)
		return root;
	return bintree_max(root->right);
}

extern "C" __declspec(dllexport) Bintree *bintree_get_branch_left(Bintree *root)
{
	return root->left;
}
extern "C" __declspec(dllexport) Bintree *bintree_get_branch_right(Bintree *root)
{
	return root->right;
}
extern "C" __declspec(dllexport) double bintree_get_value(Bintree *root)
{
	return root->value;
}