#include <stdlib.h>
#include <stdio.h>

#include "LinkedList.h"

int main(){
    LinkedList *list = initList();

    int num;
    scanf("%d", &num);

    for(int i = 0; i < num; i++){
        int type;
        scanf("%d", &type);

        switch(type){
            case 0:
                //print
                printList(list);
                break;
            case 1:{
                //add
                int data;
                scanf("%d", &data);
                Element new = {.key = data};
                addlistHead(list, new);
                break;
            }
            case 2:{
                int data;
                scanf("%d", &data);
                Element new = {.key = data};
                addlistTail(list, new);
                break;
            }
            case 3:
                //gettop
                printf("%d\n", getlistTop(list).key);
                break;
        }

    }

}
