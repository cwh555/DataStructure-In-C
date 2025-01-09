#include "LinkedList.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct listnode{
    Element data;
    struct listnode *next;
}ListNode;

struct linkedlist{
    ListNode *head;
    ListNode *tail;
};

//--helper
ListNode *newNode(Element data){
    ListNode *new = (ListNode *)malloc(sizeof(ListNode));
    new->next = NULL;
    new->data = data;
    return new;
}

//--end

LinkedList* initList(){
    LinkedList * list = (LinkedList *)malloc(sizeof(LinkedList));
    list->head = NULL;

    return list;
}

void addlistHead(LinkedList *list, Element data){
    ListNode *new = newNode(data);

    new->next = list->head;
    list->head = new;

    if(list->tail == NULL)
        list->tail = new;

    return;
}

void addlistTail(LinkedList *list, Element data){
    ListNode *new = newNode(data);

    if(list->tail == NULL){
        list->head = list->tail = new;
        return;
    }else{
        list->tail->next = new;
        list->tail = new;
        return;
    }
}

Element deletelistTop(LinkedList *list){
    Element temp;
    if(list->head == NULL){
        temp.key = -1;
        return temp;
    }
    
    temp = list->head->data;
    ListNode *old = list->head;
    list->head = old->next;

    if(list->head == NULL)
        list->tail = NULL;

    free(old);
    return temp;
}

Element getlistTop(LinkedList *list){
    Element temp;
    if(list->head == NULL){
        temp.key = -1;
        return temp;
    }
    
    temp = list->head->data;
    return temp;
}

void destroylist(LinkedList *list){
    ListNode *now = list->head;

    while(now != NULL){
        ListNode *temp = now;
        now = now->next;

        free(temp);
    }

    free(list);
    return;
}

void printList(LinkedList *list){
    ListNode *now = list->head;

    while(now != NULL){
        printf("%d ", now->data.key);
        now = now->next;
    }
    printf("\n");
}

bool listEmpty(LinkedList *list){
    return list->head == NULL;
}