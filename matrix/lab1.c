#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "lab1io.h"


int main()
{
    struct Matrix matrix, sort_matrix;
    matrix = input_matrix();
    printf("\nВведенная матрица:\n");
    print_matrix(matrix);
    sort_matrix = matrix_process(matrix);
    printf("Матрица построенная по условию задания:\n");
    print_matrix(sort_matrix);
    clear_matrix(sort_matrix);
    clear_matrix(matrix);
    return 0;
}
