#include "lab1io.h"

struct Matrix input_matrix(){
    struct Matrix matrix; // создаем структуру
    int tmp;
    printf("Введите количество строк матрицы:\n");
    matrix.num_lines = check_int(); // вводим кол-во строк и проверяем ввели ли положительное число
    if(matrix.num_lines == 0){
        do{
            printf("Матрицы такого размера не может быть создана!\n");
            printf("Введите положительное количество строк матрицы:\n");
            matrix.num_lines = check_int();
        }while(matrix.num_lines == 0);
    }
    matrix.mass_lines = (struct Line *)calloc( matrix.num_lines, sizeof(struct Line) ); // выделяем память под это все
    for( int i = 0; i < matrix.num_lines; i++ ){
        printf("Введите кол-во элементов в [%d] строке матрицы:\n", i+1);
        (matrix.mass_lines[i]).num_elements = check_int(); // вводим кол-во элем-в в строке
        (matrix.mass_lines[i]).str = (int *)calloc( (matrix.mass_lines[i]).num_elements, sizeof(int) ); //выделяем память
        if((matrix.mass_lines[i]).num_elements != 0){
            printf("Введите [%d] элементов в строку матрицы:\n", (matrix.mass_lines[i]).num_elements);
            for( int j = 0; j < (matrix.mass_lines[i]).num_elements; j++ ){ // до N элементов вызываем ввод       
                (matrix.mass_lines[i]).str[j] = check_int(); // записываем в массив числа
            }
        }
    }
    return matrix;
}

void print_matrix(struct Matrix matrix){
    for(int i = 0; i < matrix.num_lines; i++){
        for(int j = 0; j < (matrix.mass_lines[i]).num_elements; j++)
            printf("%d ", (matrix.mass_lines[i]).str[j]);
        printf("\n");
    }
    printf("\n");
}

struct Matrix matrix_process(struct Matrix matrix){
    struct Matrix new_matrix;
    new_matrix.num_lines = matrix.num_lines;
    new_matrix.mass_lines = (struct Line*)calloc(new_matrix.num_lines, sizeof(struct Line));
    int tmp = 0;
    double amp = 3.14;
    int a = -1, b = 0;
    for(int i = 0; i < new_matrix.num_lines; i++){
        for(int j = 0; j < (matrix.mass_lines[i]).num_elements; j++ ){
            tmp = (matrix.mass_lines[i]).str[j];
            if(amp != tmp){
                for(int d = j; d < (matrix.mass_lines[i]).num_elements; d++){
                    if( tmp == (matrix.mass_lines[i]).str[d] ){ // текущий элемент должен быть равен искомому и не должен быть равен предыдущему, если равен то пропускаем.
                        a++;
                    }
                }
                amp = tmp;
                if(a >= 1){
                    b++;
                }
                a = -1;
            }
        }
        if(b > 0){
            new_matrix.mass_lines[i].num_elements = b;
            new_matrix.mass_lines[i].str = (int *)calloc(b, sizeof(int));
        }
        amp = 3.14;
        b = 0;
    }
    a = -1;
    b = 0;
    // после того как выделили память под каждую строку, запишем каждый элемент из строки исходной матрицы в строку новой матрицы
    for(int i = 0; i < new_matrix.num_lines; i++){
        for(int j = 0; j < (matrix.mass_lines[i]).num_elements; j++ ){
            tmp = (matrix.mass_lines[i]).str[j];
            if(amp != tmp){
                for(int d = j; d < (matrix.mass_lines[i]).num_elements; d++){
                    if( tmp == (matrix.mass_lines[i]).str[d] ){ // текущий элемент должен быть равен искомому и не должен быть равен предыдущему, если равен то пропускаем.
                        a++;
                    }
                }
                amp = tmp;
                if(a >= 1){
                    new_matrix.mass_lines[i].str[b] = (matrix.mass_lines[i]).str[j];
                    b++;
                }
                a = -1;
            }
        }
        amp = 3.14;
        b = 0;
    }
    return new_matrix;
}

void clear_matrix(struct Matrix matrix){
    for(int i = 0; i < matrix.num_lines; i++){
            if((matrix.mass_lines[i]).str != NULL)
                free((matrix.mass_lines[i]).str);
    }
    free(matrix.mass_lines);
}

int check_int(){
    int input_number;
	char c;
    while(1){
        scanf("%d", &input_number);
        if( ( input_number >= 0 ) )
		{
			scanf("%c", &c);
            return input_number;
        }
        printf( "Введено некорректное число!\n" );
    }
}