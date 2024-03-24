#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

typedef struct Item{
    char data;
    struct Item *next;
}Item;

typedef struct Stack{
    Item *head;
}Stack;

//int - если завершение успешно то 1, нет - 0
int is_operand(char c);
int whats_priority(char a);
int dialog_enter(char *infix);
int is_infixcorrect(char *str);
int bracket_count( const char* s );
int check_list_priority(Stack *stack);
//char
char *readline();
char peek(Stack *stack);
char reverse_bracket(const char ch);
const char* is_bracket(const char ch);
const char* is_operation(const char ch);
//void
void pop(Stack *stack);
void reverse(char *str);
void printres(char *in);
void push(Stack *stack, char ch);
void infix_to_postfix(char *infix, char* postfix, Stack *stack);