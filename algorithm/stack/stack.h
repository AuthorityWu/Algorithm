#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef int DataType;
typedef struct Stack{
	DataType data;
	struct Stack *next;
}Stack;

//³õÊ¼»¯Õ»
void StackInitial(Stack **s)
{
	(*s)=(Stack *)malloc(sizeof(Stack));
	(*s)->next=NULL;
	if(!(*s)){
		printf("ÉêÇëÄÚ´æ¿Õ¼äÊ§°Ü!\n");
		exit(0);
	}
}
//ÅÐ¶ÏÕ»ÊÇ·ñÎª¿Õ
int StackNotEmpty(Stack *s)
{
	if(s->next==NULL)
		return 0;
	else
		return 1;
}

//ÈëÕ»
int StackPush(Stack *s,DataType x)
{
	Stack *p;
	p=(Stack *)malloc(sizeof(Stack));
	if(!p){
		printf("ÉêÇëÄÚ´æ¿Õ¼äÊ§°Ü!\n");
		exit(0);
	}
	p->data=x;
	p->next=s->next;
	s->next=p;
	return 1;
}

//³öÕ»
int StackPop(Stack *s,DataType *d)
{
	Stack *p=s->next;
	if(s->next==NULL){
		printf("¶ÑÕ»ÒÑ¿Õ£¬ÎÞÊý¾ÝÔªËØ³öÕ»!\n");
		return 0;
	}
	else{
		s->next=p->next;
		*d=p->data;
		free(p);
		return 1;
	}
}

//È¡Õ»¶¥ÔªËØ
int StackTop(Stack *s,DataType *d)
{
	if(s->next==NULL){
		printf("¶ÑÕ»ÒÑ¿Õ!\n");
		return 0;
	}
	else{
		*d=s->next->data;
		return 1;
	}
}

//ÊÍ·ÅÉêÇëµÄÄÚ´æ¿Õ¼ä
void Destroy(Stack *s)
{
	Stack *p,*p1;
	p=s;
	while(p!=NULL){
		p1=p;
		p=p->next;
		free(p1);
	}
}
#endif