#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "evaluate_tree.h"


void Eval_main(char * filename, int * valid_tree, int * bst, int * bal){
    // check if number to insert is within the range, and if the char field is 0x3, 0x2, 0x1, or 0x0
    FILE * input = fopen(filename, "rb");

    if (input == NULL){
        fprintf(stderr, "failed to open file.\n");
        *valid_tree = -1;
        return;
    }

    fseek(input, 0, SEEK_END); // set cursor to the end to find size of the file

    int size = ftell(input);

    if(size % (sizeof(int) + sizeof(char)) == 0){
        size /= 5;
    }
    else{
        return;
    }

    fseek(input, 0, SEEK_SET); // set cursor to the beginning to read elements

    int * inserts = malloc(sizeof(int) * size); // create an array that contains the numbers to be inserted into the tree

    char * children = malloc(sizeof(char) * size); // create an array that contains the information on how many children each node has

    for(int i = 0; i < size; i++){
        fread(&inserts[i], sizeof(int), 1, input);
        if(inserts[i] > HBT_MAX || inserts[i] < HBT_MIN){
            //tree is not of valid format
            *valid_tree = 0;
        }
        fread(&children[i], sizeof(char), 1, input);
        if(children[i] > 3 || children[i] < 0){
            *valid_tree = 0;
        }
    }

    int count = 0;

    Tnode *root = Build_preorder(inserts, children, &count, bst, valid_tree, size);

    Is_balanced(root, bal);

    free(children);
    free(inserts);
    Free_bst(root);

    fclose(input);

    return;
    
}

Tnode * Build_preorder(int * inserts, char * children, int * count, int * bst, int * valid_tree, int size){

    if(*count > size){
        *valid_tree = 0;
        return NULL;
    }

    Tnode * new = Create_eval(inserts[*count], NULL, NULL);

    int kids = children[*count];

    if(kids >= 2){
        // node will have a left child
        (*count)++;
        new -> left = Build_preorder(inserts, children, count, bst, valid_tree, size);
        if (new -> left -> key > new -> key)
        {
            *bst = 0;
        }
    }
    
    if(kids % 2 == 1){
        // node will have a right child
        (*count)++;
        new -> right = Build_preorder(inserts, children, count, bst, valid_tree, size);
        if (new->right->key <= new->key){
            *bst = 0;
        }
    }

    return new;
    
}

Tnode *Create_eval(int key, Tnode *lptr, Tnode *rptr){
    Tnode *new = malloc(sizeof(Tnode));

    if (new != NULL){
        new -> key = key;
        new -> balance = 0;
        new -> left = lptr;
        new -> right = rptr;
    }

    return new;
}

int Is_balanced(Tnode * root, int * isBal){
    if(root == NULL){
        return 0;
    }

    int left_height = Is_balanced(root->left, isBal);
    int right_height = Is_balanced(root->right, isBal);
    

    root->balance = left_height - right_height;

    if(abs(root->balance) > 1){
        *isBal = 0; 
    }

    return 1 + (left_height > right_height ? left_height : right_height);
}

void Free_bst(Tnode * root){
    // left right root

    if(root == NULL){
        return;
    }

    Free_bst(root->left);
    Free_bst(root->right);
    free(root);

}
