typedef struct Line{
	int num_elements; // кол-во элементов в строке
	int *str; // указатель на массив элементов
}Line;

typedef struct Matrix{
	int num_lines; // кол-во строк матрицы
	struct Line *mass_lines; // указатель на структуру Line
}Matrix;

	struct Matrix input_matrix(); // обращение на ввод матрицы и ее элементов
	struct Matrix matrix_process(struct Matrix matrix); // выполняет условие задание
	void print_matrix(struct Matrix matrix); // вывод матрицы на экран
	void clear_matrix(struct Matrix matrix); // очистка выделенной памяти под матрицу
	int check_int(); // проверка на вводимое число ( является ли положительным)