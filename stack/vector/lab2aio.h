#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

#define CAPACITY 32

typedef struct Stack{
    char vector[CAPACITY];
    int top;
}Stack;

//int - если завершение успешно то 1, нет - 0
int is_operand(char c);
int whats_priority(char a);
int dialog_enter(char *infix);
int is_infixcorrect(char *str);
int bracket_count( const char* s );
//char
char *readline();
Stack pop(Stack stack);
Stack push(Stack stack, char b);
char peek(Stack stack);
char reverse_bracket(const char ch);
const char* is_bracket(const char ch);
const char* is_operation(const char ch);
//void
void reverse(char *str);
void printres(char *in);
void infix_to_postfix(char *infix, char* postfix, Stack stack);