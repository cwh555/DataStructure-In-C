#include "Stack.h"

//--dataStructure 
struct stack{
    int maxsize;
    int dataNum;
    Element *data;
};

//--dataStrutcture end

//--function
Stack *initStack(int size){
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->maxsize = size;
    s->dataNum = 0;
    s->data = (Element *)malloc(sizeof(Element) * size);

    return s;
}

bool pushStack(Stack *s, Element element){
    if(Stackisfull(s))
        return false;
    
    s->data[s->dataNum++] = element;
    return true;
}

Element popStack(Stack *s){
    if(Stackisempty(s)){
        Element temp = {.key = -1};
        return temp;
    }

    return s->data[--s->dataNum];
}

bool Stackisempty(Stack *s){
    return s->dataNum == 0;
}

bool Stackisfull(Stack *s){
    return s->dataNum == s->maxsize;
}

void destroyStack(Stack *s){
    free(s->data);
    free(s);

    return;
}

Element peek(Stack *s){
    if(Stackisempty(s)){
        Element temp = {.key = -1};
        return temp;
    }
    return s->data[s->dataNum - 1];
}

int getNum(Stack *s){
    return s->dataNum;
}