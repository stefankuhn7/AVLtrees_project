#ifndef __BUILD_TREE_H__
#define __BUILD_TREE_H__
#include "hbt.h"

int Build_tree(char * file1, char * file2);

void Preorder_write(FILE * output, Tnode * root);

void preorder_print(Tnode * root);

Tnode *Search_and_insert (Tnode *root, int key, int * h_change);

Tnode *Search_and_delete (Tnode *root, int key, int * h_change);

Tnode * Delete_predecessor(Tnode * curr, int * h_change, int * pred_key);

Tnode *Create_build(int key, Tnode *lptr, Tnode *rptr);

int Is_balanced(Tnode *root, int *is_bal);

Tnode *Balance_insert(Tnode *root, int * h_change);

Tnode *Balance_delete(Tnode *root, int * h_change);

Tnode * left(Tnode * old);

Tnode * right(Tnode * old);

void Free_tree(Tnode * root);

#endif

