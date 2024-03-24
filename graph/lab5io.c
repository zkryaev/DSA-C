#include "lab5io.h"

void clear_graph(adjencylist* adlist){
    for(int i = 0; i < adlist->num_vertices; i++){
        free(adlist->array_vertices[i].name);
        adlist->array_vertices[i].name = NULL;
        adlist->array_vertices[i].prev = NULL;
        edge* scanedge = adlist->array_vertices[i].edge;
        edge* prev = NULL;
        adlist->array_vertices[i].edge = NULL;
        while(scanedge){
            prev = scanedge;
            scanedge = scanedge->nextedge;
            prev->pointvertx = NULL;
            prev->nextedge = NULL;
            free(prev);
        }
    }
    free(adlist->array_vertices);
    adlist->array_vertices = NULL;
}


// Топологическая сортировка + Ацикличность
// -1 - не посещал (белое), 0 - посетил и положил в стек(серое), 1 - посетил и вынул из стека(черное)
int check_acyclicity(adjencylist* adlist, int* detect){
    for(int i = 0; i < adlist->num_vertices; i++){
        for(int j = 0; j < adlist->num_vertices; j++)
            adlist->array_vertices[j].visit = -1;
        if(adlist->array_vertices[i].edge){ // если у вершины есть ребра то ее стоит проверить
            DFS_acycle(&(adlist->array_vertices[i]), detect);
            if( (*detect) > 0)
                return 1;
        }
    }
    return 0;
}

void DFS_acycle(vertex* scan, int* detect){
    scan->visit = 0; // посетил
    vertex* tmp = scan;
    edge* scanedge = scan->edge;
    while(scanedge){
        scan = scanedge->pointvertx;
        if(scan->visit == -1) // если не посещал
            DFS_acycle(scan, detect);
        if(scan->visit == 0){ // если посещал -> цикл | срабатывает когда signature - 1 т.е DFS используется для поиска цикла
            (*detect)++;
        }
        scanedge = scanedge->nextedge;
    }
    tmp->visit = 1; // посещал и вышел
    tmp = NULL;
    return;
}

void topologic_sort(adjencylist* adlist){
    int detect = 0;
    int detect_cycle = check_acyclicity(adlist, &detect);
    if(detect_cycle == 0){
        Stack* stack = (Stack*)malloc(sizeof(Stack));
        stack->mass = (vertex**)malloc(sizeof(vertex*));
        stack->top = 0;
        for(int j = 0; j < adlist->num_vertices; j++) // вновь помечаю все как не посещенные если в графе нет циклов
            adlist->array_vertices[j].visit = 0;
        for(int i = 0; i < adlist->num_vertices; i++){
                if(adlist->array_vertices[i].visit == 0) // если не посещал
                    DFS_topologic( &(adlist->array_vertices[i]), stack);
        }
        printf("    [ Топологически отсортированный граф ]\n");
        printfsortGraph(stack);
        for(int i = 0; i < (stack->top-1); i++){
            stack->mass[i] = NULL;
        }
        free(stack->mass);
        stack->mass = NULL;
        free(stack);
        stack = NULL;
    }else
        printf("    [ Ошибка: В графе имеется цикл! ]\n");
}

void printfsortGraph(Stack* stack){
    vertex* scan = NULL;
    for(int i = 0; i < stack->top; i++){
        scan = stack->mass[i];
        edge* scanedge = scan->edge;
        printf("[%s]", scan->name);
        while(scanedge){
            printf("-->[%s]", scanedge->pointvertx->name);
            scanedge = scanedge->nextedge;
        }
        printf("\n");
    }
}

void push(Stack* stack, vertex** scan){
    stack->mass = (vertex**)realloc((stack->mass) ,((stack->top) + 1)*(sizeof(vertex*)));
    stack->mass[stack->top] = (*scan);
    stack->top++;
}

void DFS_topologic(vertex* scan, Stack* stack){
    scan->visit = 1; // посетил
    vertex* tmp = scan;
    edge* scanedge = scan->edge;
    while(scanedge){
        scan = scanedge->pointvertx;
        if(scan->visit == 0) // если не посещал
            DFS_topologic(scan, stack);
        scanedge = scanedge->nextedge;
    }   
    push(stack, &tmp);
    tmp = NULL;
}

//BlmnFord
void blmnford_shortpath(adjencylist* adlist){
    vertex* scan = NULL;
    double minway = -1;
    int index;
    while(getchar() != '\n');
    printf("    Введите имя вершины -Начало пути-\n");
    printf("Имя: ");
    char* fstvertex = readline(); // от какой вершине
    printf("    Введите имя вершины -Конец пути-\n");
    printf("Имя: ");
    char* scndvertex = readline(); // к какой
    for(int i = 0; i < (adlist->num_vertices); i++){ // выставляем вершинам значения 
        adlist->array_vertices[i].visit = DBL_MAX;
        adlist->array_vertices[i].prev = NULL;
        if( strcmp(fstvertex, adlist->array_vertices[i].name) == 0 ){
            scan = &(adlist->array_vertices[i]);
            scan->visit = 0;
        }
        if( strcmp(scndvertex, adlist->array_vertices[i].name) == 0 ){
            index = i;
        }
    }
    double det = DBL_MAX;
    if(scan){
        for(int i = 1; i < adlist->num_vertices; i++){
            for(int j = 0; j < adlist->num_vertices; j++){
                if(adlist->array_vertices[j].visit != det){
                    scan = &(adlist->array_vertices[j]);
                    edge* scanedge = scan->edge;
                    while(scanedge){
                        if(scanedge->pointvertx->visit > scan->visit + scanedge->weight){
                            scanedge->pointvertx->visit = scan->visit + scanedge->weight;
                            scanedge->pointvertx->prev = scan;
                            if( strcmp(scndvertex, scanedge->pointvertx->name) == 0)
                                minway = scanedge->pointvertx->visit;
                        }
                    scanedge = scanedge->nextedge;
                    }
                }
            }
        }
        if(minway != -1){
            vertex* seen = &(adlist->array_vertices[index]);
            printf("[%s]", seen->name);
            seen = seen->prev;
            while(seen){
                printf("<--[%s]", seen->name);
                seen = seen->prev;
            }
            printf("\n");
            printf("    [ Расстояние от %s до %s: %.5lf ]\n", fstvertex, scndvertex, minway);
        }else
            printf("    [ Пути от вершины %s к %s -не- существует! ]\n", fstvertex, scndvertex);
    }else
        printf("    [ Такой вершины не существует! ]\n");
    scan = NULL;
    free(fstvertex);
    fstvertex = NULL;
    free(scndvertex);
    scndvertex = NULL;
}

// Graphviz
void graphviz(adjencylist* adlist){
    if(adlist->array_vertices){
        printf("Введите имя файла: \n");
        while(getchar() != '\n');
        char* fname = readline();
        FILE* file = fopen(fname, "w");
        if(file){
            fprintf(file, "digraph Tree{\n");
            fprintf(file, "label = \"Полученный граф:\";\n");
            for(int i = 0; i < adlist->num_vertices; i++){
                edge* tmp = adlist->array_vertices[i].edge;
                while(tmp){
                    fprintf(file, "%s->%s\n", adlist->array_vertices[i].name, tmp->pointvertx->name);
                    tmp = tmp->nextedge;
                }
            }
            fprintf(file, "}\n");
            char* output = (char*)calloc( 200, sizeof(char) );
            strcpy(output, "dot ");
            strcat(output, fname);
            strcat(output, " -Tpng ");
            strcat(output, " -o ");
            strcat(output, fname);
            strcat(output, ".png");
            output = (char*)realloc(output, (strlen(output) + 1)*sizeof(char));
            fclose(file);
            system(output);
            free(fname);
            fname = NULL;
            free(output);
            output = NULL;
            printf("  [ Success ]\n");
        }else
            printf("Дело дрянь\n");
    }else
        printf(" Графа нет!\n");
}
// BFS / 1 - серое / 0 - белое
void BFS(adjencylist* adlist){
    vertex* scan = NULL;
    while(getchar() != '\n');
    printf("    Введите имя вершины -Начало пути-\n");
    printf("Имя: ");
    char* fstvertex = readline(); // от какой вершине
    printf("    Введите имя вершины -Конец пути-\n");
    printf("Имя: ");
    char* scndvertex = readline(); // к какой
    for(int i = 0; i < adlist->num_vertices; i++){ // зануляем все вершины
        if( strcmp(fstvertex, adlist->array_vertices[i].name) == 0 )
            scan = &(adlist->array_vertices[i]);
        adlist->array_vertices[i].visit = 0;
        adlist->array_vertices[i].prev = NULL;
    }
    if(scan){ // если вершина откуда мы хотим идти существует
        Queue* queue = (Queue*)malloc(sizeof(Queue)); // инициализируем очередь
        queue->mass = (vertex**)malloc(sizeof(vertex*));
        queue->tail = 0;
        queue->head = 0;
        scan->visit = 1;
        enqueue(queue, &scan); // добавление в очередь
        while(queue->tail != queue->head){
            dequeue(queue, &scan); // изымание из очереди
            edge* scanedge = scan->edge;
            vertex* prev = NULL;
            prev = scan;
            while(scanedge){
                scan = scanedge->pointvertx;
                if(scan->visit == 0){ // если вершина не посещалась
                    scan->visit = 1; // вершина становится серой
                    scan->prev = prev;
                    enqueue(queue, &scan);
                    if( strcmp(scndvertex, scan->name) == 0){
                        printf("[%s]", scan->name);
                        scan = scan->prev;
                        while(scan){
                            printf("<--[%s]", scan->name);
                            scan = scan->prev;
                        }
                        printf("\n");
                        for(int i = 0; i < queue->tail + 1; i++)
                            queue->mass[i] = NULL;
                        free(queue->mass);
                        queue->mass = NULL;
                        free(queue);
                        free(scndvertex);
                        scndvertex = NULL;
                        free(fstvertex);
                        fstvertex = NULL;
                        return;
                    }
                }
                scanedge = scanedge->nextedge;
            }
        }
        free(queue->mass);
        queue->mass = NULL;
        free(queue);
        free(scndvertex);
        scndvertex = NULL;
        free(fstvertex);
        fstvertex = NULL;
        printf("    [Путь -НЕ- найден!]\n");
        return;
    }else
        printf("Такой вершины не существует!\n");
}
// Очередь
void enqueue(Queue* queue, vertex** scan)
{
    queue->mass = (vertex**)realloc( queue->mass, (queue->head + 1)*sizeof(vertex*) );
    queue->mass[queue->head] = (*scan);
    queue->head++;
}

void dequeue(Queue* queue, vertex** scan)
{
    (*scan) = queue->mass[queue->tail];
    queue->tail++;
}
//--------------------------------------------------------->
//
// Основные функции для работы с графом
//
//---------------------------------------------------------> вершины
void add_vertex(adjencylist* adlist){
        vertex new_vertex;
        while(getchar() != '\n');
        printf("    Введите имя вершины:\n");
        printf("Имя: ");
        char* name_vertex = readline();
        printf("    Введите координаты вершины:\n");
        printf("Координата X: ");
        scanf("%lf", &(new_vertex.x) );
        printf("Координата Y: ");
        scanf("%lf", &(new_vertex.y) );
        new_vertex.name = name_vertex;
        new_vertex.prev = NULL;
        new_vertex.edge = NULL;
    if( adlist->array_vertices == NULL ){
        adlist->num_vertices = 1;
        adlist->array_vertices = (vertex*)malloc(sizeof(vertex));
        adlist->array_vertices[adlist->num_vertices - 1] = new_vertex;
    }else{
        int detect = 0;
        for(int i = 0; i < adlist->num_vertices; i++){
            if( strcmp(adlist->array_vertices[i].name, name_vertex) == 0)
                detect++;
            if( (adlist->array_vertices[i].x == new_vertex.x) && (adlist->array_vertices[i].y == new_vertex.y) )
                detect++;
        }
        if(detect == 0){
            adlist->num_vertices++;
            adlist->array_vertices = (vertex*)realloc(adlist->array_vertices, adlist->num_vertices*sizeof(vertex));
            adlist->array_vertices[adlist->num_vertices - 1] = new_vertex;
        }else{
            printf("    [ Такая вершина уже есть! ]\n");
        }
    }
    name_vertex = NULL;
}

void delete_vertex(adjencylist* adlist){
    if(adlist->num_vertices != 0){
        while(getchar() != '\n');
        printf("    Введите имя вершины к-ю хотите удалить:\n");
        printf("Имя: ");
        char* delete_name = readline();
        int del_ind = -1;
        for(int i = 0; i < adlist->num_vertices; i++){
            if(strcmp(delete_name, adlist->array_vertices[i].name) == 0)
                del_ind = i;
        }
        if( del_ind != -1 ){
            for(int i = 0; i < adlist->num_vertices; i++){ // удаление ребер
                if(strcmp(adlist->array_vertices[i].name, delete_name) != 0){
                    edge* tmp = adlist->array_vertices[i].edge;
                    edge* prev = adlist->array_vertices[i].edge;
                    while(tmp){
                        if( (tmp->pointvertx) == &(adlist->array_vertices[del_ind]) ){
                            if(tmp == adlist->array_vertices[i].edge){
                                adlist->array_vertices[i].edge = tmp->nextedge;
                                tmp->pointvertx = NULL;
                                free(tmp);
                                tmp = adlist->array_vertices[i].edge;
                                continue;
                            }else{
                                prev->nextedge = tmp->nextedge;
                                tmp->pointvertx = NULL;
                                free(tmp);
                                tmp = prev->nextedge;
                                continue;
                            }
                        }
                        prev = tmp;
                        tmp = tmp->nextedge;
                    }
                    tmp = NULL;
                    prev = NULL;
                }
            }
            edge* tmp = adlist->array_vertices[del_ind].edge;
            edge* next = NULL;
            while(tmp){ // удаление ребер принадлежащих удаляемой вершине
                next = tmp->nextedge;
                tmp->pointvertx = NULL;
                tmp->nextedge = NULL;
                free(tmp);
                tmp = next;
            }
            free(adlist->array_vertices[del_ind].name);
            adlist->array_vertices[del_ind].name = NULL;
            adlist->array_vertices[del_ind].edge = NULL;
            reorganize_adlist(adlist);
        }else
            printf("Такой вершины нет!\n");
        free(delete_name);
        delete_name = NULL;
    }else
        printf("Я не запрограммирован работать с пустыми графами ч_ч\n");
}

void reorganize_adlist(adjencylist* adlist){
    for(int i = 0; i < adlist->num_vertices; i++){
        if( (adlist->array_vertices[i].name == NULL) && (i != adlist->num_vertices - 1) ){
            int j = i + 1;
            adlist->array_vertices[i].x = adlist->array_vertices[j].x;
            adlist->array_vertices[i].y = adlist->array_vertices[j].y;
            adlist->array_vertices[i].name = adlist->array_vertices[j].name;
            adlist->array_vertices[i].edge = adlist->array_vertices[j].edge;
            adlist->array_vertices[j].name = NULL;
            adlist->array_vertices[j].edge = NULL;
        }
    }
    adlist->num_vertices--;
    adlist->array_vertices = (vertex*)realloc(adlist->array_vertices, adlist->num_vertices*sizeof(vertex));
}

//---------------------------------------------------------> ребра
void add_edge(adjencylist* adlist){
    while(getchar() != '\n');
    printf("    Введите имя вершины -от- которой хотите провести ребро:\n");
    printf("Имя: ");
    char* from_name = readline();
    printf("    Введите имя вершины -к- которой хотите провести ребро:\n");
    printf("Имя: ");
    char* to_name = readline();
    int from_ind = -1, to_ind = -1;
    for(int i = 0; i < adlist->num_vertices; i++){
        if(strcmp(adlist->array_vertices[i].name, from_name) == 0)
            from_ind = i;
        if(strcmp(adlist->array_vertices[i].name, to_name) == 0)
            to_ind = i;
    }
    if( (from_ind != -1) && (to_ind != -1) ){
        edge* tmp = adlist->array_vertices[from_ind].edge;
        edge* prev = adlist->array_vertices[from_ind].edge;
        while(tmp){
            prev = tmp;
            tmp = tmp->nextedge;
        }
        tmp = (edge*)malloc(sizeof(edge));
        tmp->pointvertx = &(adlist->array_vertices[to_ind]);
        tmp->nextedge = NULL;
        double x = adlist->array_vertices[to_ind].x - adlist->array_vertices[from_ind].x;
        double y = adlist->array_vertices[to_ind].y - adlist->array_vertices[from_ind].y;
        tmp->weight = sqrt(pow(x,2) + pow(y,2));
        if(prev == NULL){
            adlist->array_vertices[from_ind].edge = tmp;
        }else{
            prev->nextedge = tmp;
        }
        tmp = NULL;
        prev = NULL;
    }else
        printf("Одной из двух заданных вершин не существует!\n");
    free(from_name);
    from_name = NULL;
    free(to_name);
    to_name = NULL;
}

void delete_edge(adjencylist* adlist){
    while(getchar() != '\n');
    printf("    Введите имя вершины -откуда- ребро исходит:\n");
    printf("Имя: ");
    char* from_name = readline();
    printf("    Введите имя вершины -куда- ребро подходит:\n");
    printf("Имя: ");
    char* to_name = readline();
    int from_ind = -1, to_ind = -1;
    for(int i = 0; i < adlist->num_vertices; i++){
        if(strcmp(adlist->array_vertices[i].name, from_name) == 0)
            from_ind = i;
        if(strcmp(adlist->array_vertices[i].name, to_name) == 0)
            to_ind = i;
    }
    if( (from_ind != -1) && (to_ind != -1) ){
        edge* tmp = adlist->array_vertices[from_ind].edge;
        edge* prev = adlist->array_vertices[from_ind].edge;
        while( tmp ){
            if( tmp->pointvertx == &(adlist->array_vertices[to_ind]) )
                break;
            prev = tmp;
            tmp = tmp->nextedge;
        }
        if( (prev) && ( tmp->pointvertx == &(adlist->array_vertices[to_ind]) ) ){
            if(tmp == adlist->array_vertices[from_ind].edge){
                adlist->array_vertices[from_ind].edge = tmp->nextedge;
                tmp->pointvertx = NULL;
                tmp->nextedge = NULL;
                prev = NULL;
                free(tmp);
                tmp = NULL;
            }else{
                prev->nextedge = tmp->nextedge;
                tmp->pointvertx = NULL;
                tmp->nextedge = NULL;
                prev = NULL;
                free(tmp);
                tmp = NULL;
            }
        }else{
            printf("Такого ребра не существует!\n");
        }
    }else
        printf("Одна из заданных вершин не существует!\n");
    free(from_name);
    from_name = NULL;
    free(to_name);
    to_name = NULL;
}
//--------------------------------------------------------->
void show_adjancylist(adjencylist* adlist){
    for(int i = 0; i < adlist->num_vertices; i++){
        printf("[%s]-->", adlist->array_vertices[i].name);
        edge* tmp = adlist->array_vertices[i].edge;
        while(tmp){
            printf("[%s:%.4lf]-->", tmp->pointvertx->name, tmp->weight);
            tmp = tmp->nextedge;
        }
        printf("\n");
    }
}

//---------------------------------------------------------------------------ОБЩЕЕ
int dialog(){
	int rc = 0;
	int n = 0;
	do{
		if(rc < 0 || rc >= 11)
            printf("You are wrong. Repeate, please!\n");
        printf("0. Выход\n1. Добавить вершину\n2. Добавить ребро\n3. Удалить вершину\n4. Удалить ребро\n5. Вывод графа\n6. Поиск в ширину\n7. Поиск -кратчайшего- пути [Беллмана-Форда]\n8. Топологическая сортировка\n--------------\n9. Графиз\n10. Поиск кратчайшего пути на основе дорожных карт\n");
		printf("Выберите: -->\n");
		n = getInt(&rc); // ввод номера пункта
		if(n == 0) // конец ввода
		rc = 0;
	} while(rc < 0 || rc >= 11);
	return rc;
}

int getInt(int *a){
  int n;
  do{
    n = scanf("%d", a);
    if (n < 0) // конец ввода (enter)
      return 0;
    if (n == 0){ // введен не int
      printf("%s\n", "Error! Repeat input");
      scanf("%*c");
    }
  } while (n == 0);
  return 1;
}

char *readline(){ // Чтение из ввода
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


void work_withroutes(adjencylist* adlist){
    printf("Enter the name of file: \n");
    while(getchar() != '\n');
    char* fname = readline();
    FILE* file = fopen(fname, "r");
    if(file){
        char* str = (char*)malloc(10*sizeof(char));
        char* vertex1 = malloc(10*sizeof(char));
        char* vertex2 = malloc(10*sizeof(char));
        char* str_weight = malloc(10*sizeof(char));
        double weight = 0;
        while(!feof(file)){ // создаем вершины
            fscanf(file, "%s", str);
            add_vert_route(adlist, str);
            fscanf(file, "%s", str);
            fscanf(file, "%s", str);
        }
        fseek(file, 0, SEEK_SET);
        free(str);
        str = (char*)malloc(30*sizeof(char));
        while(!feof(file)){ // создаем ребра
            fscanf(file, "%[^\n]s", str);

            vertex1 = strtok(str, " ");

            vertex2 = strtok(NULL, " ");

            str_weight = strtok(NULL, " ");

            sscanf(str_weight, "%lf", &weight);
  
            add_edge_route(adlist, vertex1, vertex2, weight);
            free(str);
            str = (char*)malloc(30*sizeof(char));
            fscanf(file, "%*c");
        }
        free(fname);
        free(str);
        str = NULL;
        printf("    [ Success ]\n");
        fclose(file);
    }else
        printf("    [ Такого файла не существует! ]\n");
}

void add_vert_route(adjencylist* adlist, char* name_vert){
        vertex new_vertex;
        char* name_vertex = (char*)malloc( (strlen(name_vert) + 1)*sizeof(char));
        strcpy(name_vertex, name_vert);
        name_vertex[strlen(name_vert) + 1] = '\0';
        new_vertex.name = name_vertex;
        new_vertex.edge = NULL;
        new_vertex.prev = NULL;
    if( adlist->array_vertices == NULL ){
        adlist->num_vertices = 1;
        adlist->array_vertices = (vertex*)malloc(sizeof(vertex));
        adlist->array_vertices[adlist->num_vertices - 1] = new_vertex;
    }else{
        int detect = 0;
        for(int i = 0; i < adlist->num_vertices; i++){
            if( strcmp(adlist->array_vertices[i].name, name_vertex) == 0)
                detect++;
        }
        if(detect == 0){
            adlist->num_vertices++;
            adlist->array_vertices = (vertex*)realloc(adlist->array_vertices, adlist->num_vertices*sizeof(vertex));
            adlist->array_vertices[adlist->num_vertices - 1] = new_vertex;
        }
    }
    name_vertex = NULL;
}

void add_edge_route(adjencylist* adlist, char* vertex1, char* vertex2, double weight){
    char*  from_name = (char*)malloc( (strlen(vertex1) + 1)*sizeof(char));
    strcpy(from_name, vertex1);
    from_name[strlen(vertex1) + 1] = '\0';
    char*  to_name = (char*)malloc( (strlen(vertex2) + 1)*sizeof(char));
    strcpy(to_name, vertex2);
    to_name[strlen(vertex2) + 1] = '\0';
    int from_ind = -1, to_ind = -1;
    for(int i = 0; i < adlist->num_vertices; i++){
        if(strcmp(adlist->array_vertices[i].name, from_name) == 0)
            from_ind = i;
        if(strcmp(adlist->array_vertices[i].name, to_name) == 0)
            to_ind = i;
    }
    if( (from_ind != -1) && (to_ind != -1) ){
        edge* tmp = adlist->array_vertices[from_ind].edge;
        edge* prev = adlist->array_vertices[from_ind].edge;
        while(tmp){
            prev = tmp;
            tmp = tmp->nextedge;
        }
        tmp = (edge*)malloc(sizeof(edge));
        tmp->pointvertx = &(adlist->array_vertices[to_ind]);
        tmp->nextedge = NULL;
        tmp->weight = weight;
        if(prev == NULL){
            adlist->array_vertices[from_ind].edge = tmp;
        }else{
            prev->nextedge = tmp;
        }
        tmp = NULL;
        prev = NULL;
    }else
    free(from_name);
    from_name = NULL;
    free(to_name);
    to_name = NULL;
}