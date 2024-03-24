#include "lab2lio.h"

void infix_to_postfix(char *infix, char*postfix, Stack *stack)
{
    int len = strlen(infix);
    int i = 0, j = 0;
    while( i < len )
    {
        if( is_operation(infix[i]) || is_bracket(infix[i]) )
        {
           push(stack, infix[i]);
            if((stack->head->data == 41))
            {
                while(stack->head->data != 40 )
                {
                    if(is_operation(stack->head->data))
                    {
                        postfix[j] = peek(stack);
                        j++;
                    }
                    pop(stack);
                }
                pop(stack);
                i++;
                continue;
            }
            while(check_list_priority(stack))
            {
                Item *tmp = stack->head->next;
                postfix[j] = tmp->data;
                stack->head->next = tmp->next;
                free(tmp);
                tmp = NULL;
                j++;
                if(check_list_priority(stack) == 0)
                    break;
            }
            i++;
        }
        if(is_operand(infix[i]) || isdigit(infix[i]))
        {
            postfix[j] = infix[i];
            j++;
            i++;
        }
    }
    while(stack->head != NULL){
        postfix[j] = peek(stack);
        pop(stack);
        j++;
    }
    postfix[j] = '\0';
}

// проверка приоритетности операции
int whats_priority(const char operation)
{
    int priority = 0;

    switch(operation)
    {
    case '+': priority = 0; break;
    case '-': priority = 0; break;

    case '*': priority = 1; break;
    case '/': priority = 1; break;
    }

    return priority;
}
// проверка приоритетов в нодах
int check_list_priority(Stack *stack)
{
    if(stack->head->next != NULL){
        Item *tmp = stack->head->next;
        if(((tmp->data != 40) && (tmp->data != 41)) && ((stack->head->data != 40) && (stack->head->data != 41)))
            if(whats_priority(tmp->data) >= whats_priority(stack->head->data))
                return 1;
    }
    return 0;
}

//подсчет кол-ва скобок
int bracket_count( const char* s )
{
#define OPEN '('
#define CLOSE ')'
    int counter = 0;

    for(int i = 0; i < strlen(s); i++)
        if( (s[i] == OPEN ) || ( s[i] == CLOSE))
            counter++;

    return counter;
}
// разворот скобок
char reverse_bracket(const char ch)
{
#define OPEN '('
#define CLOSE ')'
    if( ch == OPEN)
        return CLOSE;

    if( ch == CLOSE)
        return OPEN;

    return ch;
}

// переворачиваем строчку
void reverse(char *s)
{
    size_t i;
    size_t len = strlen(s);
    for(i = 0; i <= len / 2; i++)
    {
        char left = reverse_bracket(s[i]);
        char right = reverse_bracket(s[len - i - 1]);

        s[i] = right;
        s[len - i - 1] = left;
    }
}

// является ли символ скобкой
const char* is_bracket(const char ch)
{
#define BRACKETS "()"
    return strchr(BRACKETS, ch);
}

// является ли символ оператором * + - /
const char* is_operation(const char ch)
{
#define OPERATIONS "+-*/"
    return strchr(OPERATIONS, ch);
}

// является ли английской буквой
int is_operand(char c){
    if( (( c >= 97 )&&( c <= 122 )) || (( c >= 65 )&&( c <= 90 )) )
        return 1;
    return 0;
}

void push(Stack *stack, char ch)
{
    Item *tmp = (Item*)malloc(sizeof(Item));
    tmp->data = ch;
    tmp->next = stack->head;
    stack->head = tmp;
    tmp = NULL;
}

void pop(Stack *stack)
{
    Item *tmp = NULL;
    tmp = stack->head;
    stack->head = stack->head->next;
    if(tmp->next != NULL)
        tmp->next = NULL;
    free(tmp);
    tmp = NULL;
}

char peek(Stack *stack)
{
    char ch = stack->head->data;
    return ch;
}


void printres(char *in){
    printf("\n");
    for(int i = 0; i < strlen(in); i++)
        printf("%c", in[i]);
    printf("\n");
    printf("\n");
}

char *readline(){ // принимаем строку из ввода
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;

    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        }
        else if (n > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = (char*)realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    }
    else {
        res = (char*)calloc(1, sizeof(char));
    }

    return res;
}

//проверка на верность ввода
int is_infixcorrect(char *str){
    if(str == NULL) // [ пустота ] 
    {
        printf("\nInput string is empty!\n");
        return 1;
    }
    int lenght = strlen(str);
    int i = 0;
    while(1) // [ пробелы ] 
    {
        if(i < lenght)
        {   
            if(str[i] == 32) // если пробел то выдать ошибку
            {
                printf("\nSpaces are not allowed, write without spaces!\n");
                return 1;
            }
            i++;
        } else break; // если конец то выходим из цикла
    }
    while(1){ // [ правильность расположения скобок ]           
        for(int j = 0; j < lenght; j++)
        {
            if( ((str[j] == 40) && (str[j+1] == 41)) || ((str[j] == 41) && (str[j+1] == 40)) ) // случай если a+b() | a+b)(
            {
                printf("\n111111Brackets placed incorrectly!\n");
                return 1;
            }
        }
        break;
    }
    if(bracket_count(str) % 2 != 0) // [ верность количества скобок ]
    {
            printf("\nBrackets placed incorrectly!\n");
            return 1;
    }
    i = 0;
    while(1){ // [ слитность ]
        int d = 0, amount = 0, operands = 0, operations = 0;
        while(i < lenght){
            if(isdigit(str[i])){ // подсчет чисел типа 4 | 44 | 4444 и тд
                for(int j = i; isdigit(str[j]); j++ ){
                        d++; // подсчитывает количество цифр в записи числа
                }
                amount++;    // количество символов
                i = i + d;   // сдвиг на то количество цифр которое прошел цикл
                d = 0;       // обнуление счетчика после того как разобрались с данным числом
                continue;
            }
            if(is_operation(str[i]))
            {
                operations++;
                i++;
                continue;
            }
            if(is_operand(str[i]))
            {
                operands++;
                i++;
                continue;
            }
            i++;
        }
        if( operations != (operands - 1) + amount){
            printf("\nYou forgot to insert either an operation or an operand!\n");
            return 1;
        }
        break;
    }
    if(is_operation(str[0]) || is_operation(str[lenght - 1]))
    {
                printf("\nDon't set operations in start and end of string!\n");
                return 1;
    }
    // проверка на повторения и некорректные положения операций
    i = 0;
    while(1){
        int d = 0;
        while(i < lenght){
            if(is_operand(str[i]) && is_operand(str[i+1])){ //после каждого операнда все кроме очередного операнда
                printf("\nOperands must be single!\n");
                return 1;
            }
            if(isdigit(str[i])){ // встретил число
                for(int j = i; isdigit(str[j]); j++ ){ //если и далее числа то он их считает
                        d++;
                }
                i = i + d; // сдвиг на то количество цифр которое прошел цикл
                d = 0; // обнуление счетчика после того как разобрались с данным числом
                if(is_operand(str[i])){ // после все числа кроме операнда, т.к если там цифры то цикл пропустит их
                    printf("\nDigits and operand must be separated by an operation!\n");
                    return 1;
                }
                continue;
            }
            i++;
        }
        break;
    }
    return 0;
}

//диалоговое меню ввода строки
int dialog_enter(char *infix)
{
    while(1) // пока не введет правильную строчку
    {
        if(is_infixcorrect(infix) == 1){
            free(infix);
            printf("{ If you want to stop, press [F] }\n");
            char a;
            scanf("%c", &a);
            if(a == 70)
                return 0;
        }else{
            return 1;
        }
        while(getchar() != '\n');
        printf("Enter infix string (without spaces!)\n");
        infix = readline();
    }
}