#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{
	list_t list = malloc(sizeof(list_t));
	list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;

}


void list_delete(list_t list)
{
    while(list->length > 0) {
        struct node* temp, *newnode;
        temp = list->head;
        if(list->length > 1){     
            newnode = temp->next;
            list->head = newnode;
        } else {
            list->head = NULL;
            list->tail = NULL;
        }
        free(temp);
    }
    free(list);
}

void list_insert(list_t list, int index, int data)
{
    if(list->length == 0||index >= list->length){
        list_append(list, data);
    } else {
        struct node* temp;
        temp = list->head;
        int i;
        for(i = 0; i < index; i++){
            temp = temp->next;
        }
        struct node* inserttemp = (node_ptr)malloc(sizeof(node_t));
        
        inserttemp->data = data;
        temp->prev->next = inserttemp;
        inserttemp->prev = temp->prev;
        temp->next->prev = inserttemp;
        inserttemp->next = temp;
    }
    list->length++;
}

void list_append(list_t list, int data)
{   
    node_ptr temp = (node_ptr)malloc(sizeof(node_t));
    temp->data = data;
    temp->next = NULL;
    if(list->length == 0){
        temp->prev = NULL;
        list->head = temp;
        list->tail = temp;
    } else {
        list->tail->next = temp;
        temp->prev = list->tail;
        list->tail = temp;
    }

    list->length++;
}

void list_print(list_t list)
{
    node_ptr temp;
    temp = list->head;
    int i;
    for(i = 0; i<list->length;i++){
        printf("%i ",temp->data);
        temp = temp->next;
    }
    printf("\n");   
}

long list_sum(list_t list)
{
    node_ptr temp;
    long sum = 0;
    temp = list->head;
    int i;
    for(i = 0; i < list->length;i++){
        sum+=temp->data;
        temp = temp->next;
    }
    return sum;

}

int list_get(list_t list, int index)
{
    node_ptr temp;
    temp = list->head;
    int i;
    for(i = 0; i < index;i++){
        temp = temp->next;        
    }
    return temp->data;
}

int list_extract(list_t list, int index)
{
    int extracted_data;
    struct node* temp;

    temp = list->head;
    if(list->length == 0) {
        return -1;
    } else if (list->length == 1){
        extracted_data = temp->data;
        list->head = NULL;
        list->tail = NULL;
    } else if(index == 0){
        extracted_data = temp->data;
        list->head = temp->next;
        list->head->prev = NULL;
    } else if (index >= list->length){
        temp = list->tail;
        extracted_data = temp->data;
        list->tail = temp->prev;
        list->tail->next = NULL;
    } else {
        int i;
        for(i = 0; i< index; i++){
            temp = temp->next;
        }
        extracted_data = temp->data;
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;

    }

    free(temp);
    list->length--;

    return extracted_data;
}
