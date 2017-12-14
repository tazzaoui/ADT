#include <stdlib.h>
#include <stdio.h>
#include "status.h"
#include "my_string.h"
#include "generic_vector.h"
#include "AVL_TREE.h"


Node* new_node(MY_STRING key){
    
    Node* node = (Node*) malloc(sizeof(Node));
    
    if(node != NULL){
        //Alloc. the key..
        node->key = my_string_init_c_string(my_string_c_str(key));
        
        if(node->key == NULL){
            free(node);
            return NULL;
        }
        
        node->words = generic_vector_init_default
        (my_string_assignment, my_string_destroy);
        
        if(node->words == NULL){
            free(node->key);
            free(node);
            return NULL;
        }
        
        node-> left = NULL;
        node->right = NULL;
        node->height = 1;
    }
    return node;
}

Node* insert(Node* node, MY_STRING key, MY_STRING word){
    if (node == NULL){
        Node* newNode = new_node(key);
        generic_vector_push_back(newNode->words, word);
        return newNode;
    }
    
    if(my_string_compare(key, node->key) == 0){
        generic_vector_push_back(node->words, word);
        return node;
    }
    
    if (my_string_compare(key, node->key) < 0)
        node->left  = insert(node->left, key, word);
    
    else if (my_string_compare(key, node->key) > 0)
        node->right = insert(node->right, key, word);
    
    
    //Update heights
    node->height = 1 +
    max(height(node->left),height(node->right));
    
   
    //Check if unbalanced...
    int balance_factor = get_balance(node);
    
    /*THIS IS WHERE WE BALANCE THE TREE*/
    
    if (balance_factor > 1 && key < node->left->key)
        return right_rotate(node);
    
    
    if (balance_factor < -1 && key > node->right->key)
        return left_rotate(node);
    
    // Left Right zig zag
    if (balance_factor > 1 && key > node->left->key){
        node->left =  left_rotate(node->left);
        return right_rotate(node);
    }
    
    // Right left zig zag
    if (balance_factor < -1 && key < node->right->key){
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    
    return node;
}

GENERIC_VECTOR lookup_by_key(Node* node, MY_STRING key){
    if(node == NULL)
        return NULL;
    
    if(my_string_compare(node->key, key) < 0)
        return lookup_by_key(node->left, key);
    
    if(my_string_compare(node->key, key) > 0)
        return lookup_by_key(node->right, key);
    
    else
         return node->words;
}

Node* right_rotate(Node* root){
    Node *temp = root->left;
    if(temp != NULL){
    Node* right = temp->right;
    
    //right rotation...
    temp->right = root;
    root->left = right;
    
    // Update heights
    root->height = max(height(root->left), height(root->right))+1;
    temp->height = max(height(temp->left), height(temp->right))+1;
    }

    return temp;
}

Node* left_rotate(Node* root){
    Node *temp = root->right;
    if(temp != NULL){
        Node *left = temp->left;
        // left rotation...
        temp->left = root;
        root->right = left;
        // Update heights
        root->height = max(height(root->left), height(root->right))+1;
        temp->height = max(height(temp->left), height(temp->right))+1;
    }
    return temp;
}

//Calculate the max between two integers a and b
int max(int a, int b){
    return (a > b) ? a : b;
}

//Height = the number of nodes below you
int height(Node* root){
    if (root == NULL) return 0;
    return root->height;
}

// Get Balance factor of a node
int get_balance(Node* root){
    if (root == NULL) return 0;
    return height(root->left) - height(root->right);
}

void preOrder(Node *root){
    if(root != NULL)
    {
        printf("%s %d\n", my_string_c_str(root->key), generic_vector_get_size(root->words));
        preOrder(root->left);
        preOrder(root->right);
    }
}

void destroy(Node** node){
    if(*node != NULL){
        destroy(&(*node)->left);
        destroy(&(*node)->right);
        free(*node); *node = NULL;
    }
}

