#include "lab2lio.h"

int main(){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->head = NULL;
    char *infix = NULL;
    char *prefix = NULL;
    printf("Enter infix string (without spaces!)\n");
    infix = readline();
    if(dialog_enter(infix) == 0)
    {
        printf("The program has ended...\n");
        return 1;
    }
    printf("Infix notation: \n");
    printres(infix);
    reverse(infix);
    prefix = (char*)calloc(strlen(infix) - bracket_count(infix) + 1, sizeof(char));
    infix_to_postfix(infix, prefix, stack);
    free(infix);
    reverse(prefix);
    printf("\nPrefix notation: \n");
    printres(prefix);
    free(stack);
    free(prefix);
    return 0;
}