#ifndef AVL_TREE_h
#define AVL_TREE_h

#include <stdio.h>
#include "status.h"
#include "my_string.h"
#include "generic_vector.h"

typedef struct node Node;

// An AVL tree node
struct node{
    MY_STRING key;
    GENERIC_VECTOR words;
    Node* left;
    Node* right;
    int height;
};

int max(int a, int b);
int height(Node* N);
int get_balance(Node* N);
Node* right_rotate(Node* root);
Node* left_rotate(Node *x);
Node* new_node(MY_STRING key);
Node* insert(Node* node, MY_STRING key, MY_STRING word);
GENERIC_VECTOR lookup_by_key(Node* node, MY_STRING key);
void preOrder(Node* node);
void destroy(Node** node);

#endif /* AVL_TREE_h */
