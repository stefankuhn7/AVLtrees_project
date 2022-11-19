#include <stdio.h>
#include <stdlib.h>
#include "build_tree.h"
#include "hbt.h"

int Build_tree(char * file1, char * file2){
    // open the file
    
    FILE * input = fopen(file1, "rb");

    if (input == NULL){
        fprintf(stderr, "failed to open file.\n");
        return -1;
    }
    fseek(input, 0, SEEK_END);

    int size = ftell(input);

    if(size % (sizeof(int) + sizeof(char)) == 0){
        size /= 5;
    }
    else{
        return -1;
    }

    fseek(input, 0, SEEK_SET);

    Tnode *root = NULL;

    int num = 0;
    char c = 0;
    int h_change = 0;

    for(int i = 0; i < size; i++){ 
        fread(&num, sizeof(int), 1, input);
        fread(&c, sizeof(char), 1, input);
        h_change = 0;

        if (c == 'i'){
            root = Search_and_insert(root, num, &h_change);
        }
        else if (c == 'd'){
            root = Search_and_delete(root, num, &h_change);
        }
        else{
            fprintf(stderr, "Invalid instructions.\n");
            fclose(input);
            return -1;
        }
    }
    
    fclose(input);
    FILE * output = fopen(file2, "wb");
    if(output == NULL){
        fprintf(stderr, "Invalid output type\n");
        return -1;
    }
    
    Preorder_write(output, root);

    fclose(output);

    Free_tree(root);
    
    return 1;

}

Tnode *Search_and_insert (Tnode *root, int key, int * h_change){

    if(root == NULL){
        *h_change = 1;
        return Create_build(key, NULL, NULL);
    }


    if(key <= root->key){
        root->left = Search_and_insert(root->left, key, h_change);
        if(*h_change){
            root->balance++;
        }
    }
    else{
        root->right = Search_and_insert(root->right, key, h_change);
        if(*h_change){
            root->balance--;
        }
    }


    int delta = 0; // h_change of subcall
    //after node is inserted, need to check if balanced
    if(*h_change){
        if (abs(root->balance) > 1){
            // balanced the tree
            root = Balance_insert(root, &delta);
            *h_change += delta;
            // also need to recalculate new balances
        }
        else if(abs(root->balance) == 1){
            *h_change = 1;
        }
        else{
            *h_change = 0;
        }
    }
    

    return root;
}

Tnode *Search_and_delete (Tnode *root, int key, int * h_change){

    if(root == NULL){
        return NULL;
    }

    if(key > root->key){

        root->right = Search_and_delete(root->right, key, h_change);

        if(*h_change) {
            if(++(root->balance) == -2) {
                root = Balance_delete(root, h_change);
            }
            else{
                *h_change = - !root->balance;
            }
        }

        return root;
    }
    else if(key < root->key){

        root->left = Search_and_delete(root->left, key, h_change);
        
        if(*h_change) {
            if(--(root->balance) == 2) {
                root = Balance_delete(root, h_change);
            }
            else{
                *h_change = - !root->balance;
            }
        }

        return root;
    }

    if (root->left == NULL){
        *h_change = -1;
        Tnode * temp = root->right;
        free(root);
        return temp;
    }

    if(root->left->right == NULL){
        root->key = root->left->key;
        root->left->balance = root->balance - 1;
        root->left->right = root->right;
        Tnode * temp = root->left;
        free(root);
        root = temp;
        
        if(root -> balance == -2){
            *h_change = -1;
            root = Balance_delete(root, h_change);
        }
        else{
            *h_change = - !root -> balance;
        }
    }
    else{
        int delta = 0;
        int pred_key = 0;
        root -> left = Delete_predecessor(root -> left, &delta, &pred_key);
        root -> key = pred_key;
        if (delta){
            root -> balance += delta;
            
            if(root -> balance == -2) {
                root = Balance_delete(root, h_change);
            }
            else{
                *h_change = - !root->balance;
            }
        }
    }
    return root;
}


Tnode * Delete_predecessor(Tnode * curr, int * h_change, int * pred_key) {
    if(curr -> right == NULL){
        *pred_key = curr -> key;
        Tnode * temp = curr -> left;
        free(curr);
        *h_change = -1;
        return temp;
    }

    curr -> right = Delete_predecessor(curr -> right, h_change, pred_key);

    if(*h_change){
        curr -> balance -= *h_change;
        
        if(curr -> balance == 2){
            curr = Balance_delete(curr, h_change);
        }
        else{
            *h_change = - !curr -> balance;
        }
    }
    return curr;
}

Tnode *Create_build(int key, Tnode *lptr, Tnode *rptr){
    Tnode *new = malloc(sizeof(Tnode));

    if (new != NULL){
        new -> key = key;
        new -> balance = 0;
        new -> left = lptr;
        new -> right = rptr;
    }

    return new;
}

Tnode *Balance_insert(Tnode *root, int * h_change){
    if(root->balance == 2){
        if(root->left->balance == 1){ // case 1 L
            root = right(root);
            root->balance = 0;
            root->right->balance = 0;
            *h_change = -1;
        }
        else if(root->left->balance == -1){ // case 2
            root->left = left(root->left);
            root = right(root);
            *h_change = -1;
            
            if(root->right->right == NULL){
                root->right->balance = 0;
                root->left->balance = 0;
            }
            else if(root->balance == 1){ //case 2a 
                root->left->balance = 0;
                root->right->balance = -1;
            }
            else if(root->balance == -1){ //
                root->left->balance = 1;
                root->right->balance = 0;
            }
            root->balance = 0;
        }
    }
    else if(root->balance == -2){ 
        if(root->right->balance == 1){
            root->right = right(root->right);
            root = left(root);
            *h_change = -1;

            if(root -> left -> left == NULL){
                root->right->balance = 0;
                root->left->balance = 0;
            }
            else if(root->balance == 1){
                root->right->balance = -1;
                root->left->balance = 0;
            }
            else if(root->balance == -1){
                root->right->balance = 0;
                root->left->balance = 1;
            }
            root->balance = 0;
        }
        else if(root->right->balance == -1){
            root = left(root);
            root->balance = 0;
            root->left->balance = 0;
            *h_change = -1;
        }
    }
   
    return root;
}

Tnode * Balance_delete(Tnode * root, int * h_change){
    if(root->balance == 2){
        if(root->left->balance == 0){
            root = right(root);
            root->right->balance = 1;
            root->balance = -1;
            *h_change = 0;
        }
        else if(root->left->balance == 1){ // case 1 L
            root = right(root);
            root->balance = 0;
            root->right->balance = 0;
            *h_change = -1;
        }
        else if(root->left->balance == -1){ // case 2
            root->left = left(root->left);
            root = right(root);
            *h_change = -1;
            
            if(root->right->right == NULL){
                root->right->balance = 0;
                root->left->balance = 0;
            }
            else if(root->balance == 1){ //case 2a 
                root->left->balance = 0;
                root->right->balance = -1;
            }
            else if(root->balance == -1){ //
                root->left->balance = 1;
                root->right->balance = 0;
            }
            root->balance = 0;
        }
    }
    else if(root->balance == -2){
        if(root->right->balance == 0){
            root = left(root);
            root->left->balance = -1;
            root->balance = 1;
            *h_change = 0;
        } 
        else if(root->right->balance == 1){
            root->right = right(root->right);
            root = left(root);
            *h_change = -1;

            if(root -> left -> left == NULL){
                root->right->balance = 0;
                root->left->balance = 0;
            }
            else if(root->balance == 1){
                root->right->balance = -1;
                root->left->balance = 0;
            }
            else if(root->balance == -1){
                root->right->balance = 0;
                root->left->balance = 1;
            }
            root->balance = 0;
        }
        else if(root->right->balance == -1){
            root = left(root);
            root->balance = 0;
            root->left->balance = 0;
            *h_change = -1;
        }
    }
   
    return root;
}

Tnode * left(Tnode *old){
    Tnode * new = old -> right;
    old -> right = new -> left;
    new -> left = old;
    return new;
}

Tnode * right(Tnode * old){
    Tnode * new = old -> left;
    old -> left = new -> right;
    new -> right = old;
    return new;
}

void Free_tree(Tnode * root){
    // left right root

    if(root == NULL){
        return;
    }

    Free_tree(root->left);
    Free_tree(root->right);
    free(root);

}

void Preorder_write(FILE * output, Tnode * root){
    if(root == NULL){
        return;
    }
    int key = root->key;
    char kid = 0;
    
    fwrite(&key, sizeof(int), 1, output);
    if(root->left == NULL && root->right == NULL){
        kid = 0;
    }
    else if(root->left == NULL){
        kid = 1;
    }
    else if(root->right == NULL){
        kid = 2;
    }
    else{
        kid = 3;
    }
    fwrite(&kid, sizeof(char), 1, output);
    
    Preorder_write(output, root->left);
    Preorder_write(output, root->right);

}
