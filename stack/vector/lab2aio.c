#include "lab2vio.h"

void infix_to_postfix(char *infix, char*postfix, Stack stack)
{
    int len = strlen(infix);
    int i = 0, j = 0;
    while( i < len )
    {
        if( is_operation(infix[i]) || is_bracket(infix[i]) )
        {
            if(stack.top + 1 >= CAPACITY)
            {
                printf("\nError! Stack Overflow!\n");
                break;
            }
            stack = push(stack, infix[i]);

            if((stack.vector[stack.top] == 41))
            {
                while( stack.vector[stack.top] != 40 )
                {
                    if(is_operation(stack.vector[stack.top]))
                    {
                        postfix[j] = peek(stack);
                        j++;
                    }
                    stack = pop(stack);
                }
                stack = pop(stack);
                i++;
                continue;
            }
            if( ((stack.vector[stack.top - 1] != 40) && (stack.vector[stack.top - 1] != 41)) && ((stack.vector[stack.top] != 40)&&(stack.vector[stack.top] != 41)) ){
                    while( ((whats_priority(stack.vector[stack.top - 1])) >= (whats_priority(stack.vector[stack.top]))) && ((stack.top - 1) >= 0) )
                    {
                        char ch;
                        ch = stack.vector[stack.top - 1];
                        stack.vector[stack.top - 1] = stack.vector[stack.top];
                        stack = pop(stack);
                        postfix[j] = ch;
                        j++;
                        if( ((stack.vector[stack.top - 1] == 40) || (stack.vector[stack.top - 1] == 41)) || ((stack.vector[stack.top] == 40)&&(stack.vector[stack.top] == 41)) )
                            break;
                    }
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
    while(stack.top != -1){
        postfix[j] = peek(stack);
        stack = pop(stack);
        j++;
    }
    postfix[j] = '\0';
	printf("%s\n", postfix);
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
        char left = reverse_bracket( s[i] );
        char right = reverse_bracket( s[len - i - 1]);

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

Stack push(Stack stack, char ch)
{
    stack.top++;
    stack.vector[stack.top] = ch;
    return stack;
}

Stack pop(Stack stack)
{
    stack.top--;
    return stack;
}

char peek(Stack stack)
{
    return stack.vector[stack.top];
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
