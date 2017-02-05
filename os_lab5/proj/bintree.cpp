#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "bintree.h"

/*struct _bintree_leave {
Item value;
Bintree_leave *branches[2];
};*/

struct _bintree {
	Item value;
	Bintree *left;
	Bintree *right;
};

Bintree* bintree_create(Item value)
{
	Bintree *nw = (Bintree *)calloc(1, sizeof(Bintree));
	nw->left = NULL;
	nw->right = NULL;
	nw->value = value;
	return nw;
}
void bintree_destroy(Bintree **root)
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
void bintree_push(Bintree *root, Item value)
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
bool bintree_pop(Bintree **root, Item value)
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
	//Bintree *rm = tmp->left;
	//tmp->left = NULL;
	bintree_destroy(&(tmp->left));
	return false;
}
void bintree_print(Bintree *root, int lvl)
{
	if (root == NULL) {
		printf("Tree is empty\n");
		return;
	}
	for (int i = 0; i < lvl; i++)
		printf("  ");
	printf("%lld\n", root->value);
	if (root->left != NULL)
		bintree_print(root->left, lvl + 1);
	if (root->right != NULL)
		bintree_print(root->right, lvl + 1);
}
/*bool bintree_is_empty(Bintree *bintree) {
}*/
Bintree *bintree_min(Bintree *root)
{
	if (root->left == NULL)
		return root;
	return bintree_min(root->left);
}
Bintree *bintree_max(Bintree *root)
{
	if (root->right == NULL)
		return root;
	return bintree_max(root->right);
}

Bintree *bintree_get_branch_left(Bintree *root)
{
	return root->left;
}
Bintree *bintree_get_branch_right(Bintree *root)
{
	return root->right;
}
Item bintree_get_value(Bintree *root)
{
	return root->value;
}