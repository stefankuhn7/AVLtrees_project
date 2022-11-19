#ifndef __EVALUATE_TREE_H__
#define __EVALUATE_TREE_H__
#include "hbt.h"

int Is_balanced(Tnode * root, int * is_bal);

Tnode * Build_preorder(int * inserts, char * children, int * count, int * bst, int * valid_tree, int size);

void Eval_main(char * filename, int * valid_in, int * bst, int * bal);

Tnode *Create_eval(int key, Tnode *lptr, Tnode *rptr);

void Free_bst(Tnode * root);

#endif
