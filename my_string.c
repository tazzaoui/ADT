#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "status.h"
#include "my_string.h"

typedef struct my_string{
    int size;
    int capacity;
    char* data;
}My_String;

MY_STRING my_string_init_default(void){
    
    My_String* myString = (My_String*) malloc(sizeof(My_String));
    
    if(myString !=  NULL){
        myString->size = 0;
        myString->capacity = 8;
        
        myString->data = (char*) malloc(sizeof(char) * myString->capacity);
        if(myString->data == NULL){
            free(myString); myString = NULL;
        }
        myString->data[0] = '\0';
    }
    return (MY_STRING) myString;
}

MY_STRING my_string_init_c_string(const char* c_string){
    My_String* myString = (My_String*) malloc(sizeof(My_String));
    int i, size = 0;
    
    if(myString !=  NULL){
        
        while(c_string[size] != '\0') //Calculate the size of the c-string
            size++;
        
        myString->size = size;
        myString->capacity = size + 1;
        
        myString->data = (char*) malloc(sizeof(char) * (size+2));
        
        if(myString->data == NULL){
            free(myString); myString = NULL;
        }else{
            for(i = 0; i <= size; i++) //copy c-string to data with null-terminator
                myString->data[i] = c_string[i];
        }
    }
    
    return (MY_STRING) myString;
}

int my_string_get_capacity(MY_STRING hMy_String){
    My_String *pMystring = (My_String*) hMy_String;
    return pMystring->capacity;
}

int my_string_get_size(MY_STRING hMy_String){
    My_String *pMystring = (My_String*) hMy_String;
    return pMystring->size;
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string){
    int size = 0;
    My_String* pLeft_string = (My_String*) hLeft_string;
    My_String* pRight_string = (My_String*) hRight_string;
    
    char* leftData = pLeft_string->data;
    char* rightData = pRight_string->data;
    
    while(leftData[size] != '\0' && rightData[size] != '\0'){
        if(leftData[size] >  rightData[size])
            return 1;
        else if(leftData[size] < rightData[size])
            return -1;
        size++;
    }
    
    
    if(leftData[size] == '\0' && rightData[size] == '\0') //both same size and word
        return 0;
    else if(leftData[size] == '\0')
        return -1; // prefix case
    else
        return 1; //prefix cases
}

Status my_string_extraction(MY_STRING hMy_string, FILE* fp){
    My_String* pMy_String = (My_String*) hMy_string;
    if(pMy_String == NULL || fp == NULL) return FAILURE;
    char c = '\0';
    int status = fscanf(fp,"%c", &c);
    if(status){
        pMy_String->size = 0;
        while(status){
            //Resize if capacity = size to 2*size
            if(pMy_String->capacity == pMy_String->size){
                char* newData = (char*) malloc(sizeof(char) * pMy_String->capacity * 2);
                if(newData == NULL) return FAILURE;
                for(int i = 0; i<pMy_String->size; i++)
                    newData[i] = pMy_String->data[i];
                
                free(pMy_String->data);
                pMy_String->data = newData;
                pMy_String->capacity *= 2;
            }
            if(c == '\n' || c == EOF || c == '\0')
                break;
            if(c == ' '){
                ungetc(c, fp);
                c = '\0';
                break;
            }
            pMy_String->data[pMy_String->size++] = c;
            status = fscanf(fp,"%c", &c);
        }
    }
    if(status != 1 && c == '\0') return FAILURE;
    
    return SUCCESS;
}

Status my_string_insertion(MY_STRING hMy_string, FILE* fp){
    My_String* pMy_String = (My_String*) hMy_string;
    
    if(pMy_String == NULL || pMy_String->size == 0)
        return FAILURE;
    
    
    for(int i = 0; i<=pMy_String->size; i++)
        fprintf(fp, "%c", pMy_String->data[i]);
    
    pMy_String->size = 0;
    
    return SUCCESS;
}

Status my_string_push_back(MY_STRING hMy_string, char item){
    My_String* pMy_String = (My_String*) hMy_string;
    
    if(pMy_String->size + 1 >= pMy_String->capacity){
        char* newData = (char*) malloc(sizeof(char) * pMy_String->capacity * 2);
        
        if(newData == NULL) return FAILURE;
        
        for(int i = 0; i<=pMy_String->size; i++)
            newData[i] = pMy_String->data[i];
        
        free(pMy_String->data);
        pMy_String->data = newData;
        pMy_String->capacity *= 2;
    }
    
    pMy_String->data[pMy_String->size] = item;
    pMy_String->data[++pMy_String->size] = '\0';
    
    return SUCCESS;
}

Status my_string_pop_back(MY_STRING hMy_string){
    My_String* pMy_String = (My_String*) hMy_string;
    
    if(pMy_String->size == 0) return FAILURE;
    
    pMy_String->data[--pMy_String->size] = '\0';
    
    return SUCCESS;
}

char* my_string_at(MY_STRING hMy_string, int index){
    My_String* pMy_String = (My_String*) hMy_string;
    
    if(index < 0 || index > pMy_String->size)
        return NULL;
    
    return (char*) pMy_String->data + index;
}

char* my_string_c_str(MY_STRING hMy_string){
    My_String* pMy_String = (My_String*) hMy_string;
    
    return (char*) pMy_String->data;
}

Status my_string_concat(MY_STRING hResult, MY_STRING hAppend){
    My_String* pResult = (My_String*) hResult;
    My_String* pAppend = (My_String*) hAppend;
    
    if(pResult->size + pAppend->size > pResult->capacity){
        char* newData = (char*) malloc(sizeof(char) * (pResult->size + pAppend->size + 2));
        if(newData == NULL) return FAILURE;
        
        for(int i = 0; i<=pResult->size;i++)
            newData[i] = pResult->data[i];
        
        free(pResult->data);
        pResult->data = newData;
        pResult->capacity = (pResult->size + pAppend->size);
    }
    
    for(int i = 0; i<=pAppend->size;i++)
        pResult->data[i+pResult->size] = pAppend->data[i];
    
    pResult->size =(pResult->size + pAppend->size) + 1;
    pResult->data[pResult->size] = '\0';
    
    return SUCCESS;
}

Boolean my_string_empty(MY_STRING hMy_string){
    My_String* pMy_String = (My_String*) hMy_string;
    return (Boolean) pMy_String->size==0;
}

Status my_string_assignment(ITEM* pLeft, ITEM Right){
    My_String *leftString = (My_String*) *pLeft;
    My_String *rightString = (My_String*) Right;
    
    if(leftString != NULL && leftString->capacity < rightString->size){
        my_string_destroy(pLeft); leftString = NULL;
    }
    
    if(leftString == NULL){
        
        leftString = (My_String*) malloc(sizeof(My_String));
        
        if(leftString !=  NULL){
            
            leftString->capacity = rightString->capacity;
            
            leftString->data = (char*) malloc(sizeof(char) * rightString->capacity);
            if(leftString->data == NULL){
                free(leftString); leftString = NULL;
                return FAILURE;
            }
            
            leftString->data[0] = '\0';
        }
        
        else return FAILURE;
    }
    
    for(int i = 0; i < rightString->size; i++)
        leftString->data[i] = rightString->data[i];

    leftString->data[rightString->size] = '\0';
    
    leftString->size = rightString->size;
    
    *pLeft = leftString;
    
    return SUCCESS;
}

void my_string_destroy(ITEM* pItem){
    My_String* pMy_String = (My_String*) *pItem;
    free(pMy_String->data); pMy_String->data = NULL;
    free(pMy_String); pMy_String = NULL;
}

Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word, char guess){
    int size = my_string_get_size(word);
    char *element;
    
    char words[size + 1];
    
    for(int i = 0; i <= size; i++)
        words[i] = *my_string_at(current_word_family, i);
    
    My_String* newKey = (My_String*) new_key;

    for(int i = 0; i < size; i++){
        element = my_string_at(word, i);
        if(*element == guess)
            words[i] = tolower(guess);
    }
    
    if(newKey->capacity < size + 1){
        char* temp = (char *) malloc((size + 1) * sizeof(char));
        
        if(temp == NULL)
            return FAILURE;
        
        free(newKey->data);
        newKey->data = temp;
    }
    
    for(int i = 0; i <= size; i++)
        newKey->data[i] = words[i];
    
    return SUCCESS;
}

