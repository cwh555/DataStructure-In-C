#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
#include <stdbool.h>
#include "Element.h"


typedef struct stack Stack;


Stack *initStack(int size);
bool pushStack(Stack *s, Element element);
Element popStack(Stack *s);
bool Stackisempty(Stack *s);
bool Stackisfull(Stack *s);
void destroyStack(Stack *s);

Element peek(Stack *s);
int getNum(Stack *s);

#endif