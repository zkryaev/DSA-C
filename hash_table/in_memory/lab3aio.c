#include "lab3aio.h"
//
// Функции для Open Adress Table
//
int oat_hash1(char* in, int size)
{
    unsigned int hash2 = 2139062143;
    for(int i = 0; i < strlen(in); i++){
        hash2 = 37 * hash2 + in[i];
    }
    return abs(hash2)%size;
}

int oat_hash2(char* in, int size)
{
    int hash1 = 0;
    for(int i = 0; i < strlen(in); i++){
        hash1 = hash1 + in[i];
    }
    return abs(hash1)%(size);
}

int oat_double_hashing(char* in, int* i, int size)
{
    int dh = 0, h1 = oat_hash1(in, size), h2 = oat_hash2(in, size);
    dh = ( h1 + (*i)*h2 ) % size;
    (*i) = (*i) + 1;
    return dh;
}

int oat_count_release(Table *table, char* in) // подсчет количества версий
{
    int ver = 0, ind = 0, i = 0;
    for(int j = 0; j < table->msize; j++){
        ind = oat_double_hashing(in, &i, table->msize);
        if(table->ks2[ind].busy)
            if(strcmp(table->ks2[ind].key, in) == 0)
                    ver++;
        }
    return ++ver;
}
//--------------------------------------------------------------------
int oat_insert(Table *table, Item* elem, char* in) // вставка в Open Adress Table
{
    if(table->csize1 < table->msize){
        while(in != NULL){
            int ind = 0, version = 0;
            if((ind = oat_search_insert(table, in)) < 0){ // таблица заполнена
                printf("Table is overflow!\n");
                printf("If you want insert, delete any element!\n");
                break;
            }else{
                version = oat_count_release(table, in);
                table->ks2[ind].busy = 1;                          // занятость
                table->ks2[ind].key = (char*)calloc((strlen(in) + 1),sizeof(char));
                strcpy(table->ks2[ind].key, in);
                table->ks2[ind].release = version;                 // установили версию
                table->ks2[ind].element = elem;
                table->ks2[ind].element->p2 = &table->ks2[ind];
                elem = NULL;
                break;
            }
        }
        if(in == NULL){
            printf("Incorrect key!\n");
            return 1;
        }
        if(in != NULL){
            return 0;
        }
            
    }
}

int oat_search_insert(Table *table, char* in) // поиск для вставки в Open Adress Table
{
    int j = 0, ind = 0, i = 0;
    while(j < table->msize){
        ind = oat_double_hashing(in, &i, table->msize);
        i++;
        if(table->ks2[ind].busy == 0){
            printf("ind - %d\n", ind);
            return ind;
        }
        j++;
    }
    return -1;
}
//-------------------------------------------------------------------------
void oat_delmany_elems(Table *table) // удаление для заданного ключа OAT | Delete -> case 3:
{
    while(getchar() != '\n');
    printf("Enter the key 2:\n");
    char* in = readline();
    int ind = 0, i = table->csize1, ver = oat_count_release(table, in);
    if((ver > 0) && (in != NULL)){
            while( (ind = oat_search_delete(table, in, &i)) >= 0 )
            {
                table->ks2[ind].busy = 0;
                free(table->ks2[ind].key);
                table->ks2[ind].key = NULL;
                table->ks2[ind].element->p2 = NULL;
                free(table->ks2[ind].element->info);
                table->ks2[ind].element->info = NULL;
                table->ks2[ind].element->p1->busy = 0;
                free(table->ks2[ind].element->p1->key);
                table->ks2[ind].element->p1->key = NULL;
                table->ks2[ind].element->p1 = NULL;
                free(table->ks2[ind].element);
                table->ks2[ind].element = NULL;
            }
            free(in);
            in = NULL;
    }else{
        printf("The element from these key don't exist!\n");
        free(in);
        in = NULL;
    }
}

int oat_search_delete(Table *table, char* in, int* i)
{
    int ind = 0;
    for(int j = 0; j < table->msize; j++){
        ind = oat_double_hashing(in, i, table->msize);
        if(table->ks2[ind].busy)
            if(strcmp(table->ks2[ind].key, in) == 0)
                return ind;
    }
    return -1;
}
//------------------------------------------------------------------
void oat_find_elems(Table *table) // Find -> case 3:
{
    if(chfull_table){
        while(getchar() != '\n');
        printf("Enter the key 2:\n");
        char* in = readline();
        int ver = oat_count_release(table, in);
        ver--;
        if( (ver > 0) && (in != NULL) ){
            Item* elems_copy = (Item*)calloc(ver, sizeof(Item));
            Item* pitem = (Item*)malloc(sizeof(Item));
            for(int i = 0; i < ver; i++){
                oat_search_elems(table, in, pitem);
                elems_copy[i].info = (char*)calloc((strlen(pitem->info) + 1), sizeof(char));
                strcpy(elems_copy[i].info, pitem->info);
                free(pitem->info);
                elems_copy[i].p1 = NULL;
                elems_copy[i].p2 = NULL;
            }
            int rel = 1;
            for(int i = 0; i < ver; i++){
                printf("[ release: %d || key2: %s || info: %s ]\n", rel, in, elems_copy[i].info);
                rel++;
                free(elems_copy[i].info);
                elems_copy[i].info = NULL;
            }
            free(pitem);
            pitem = NULL;
            free(elems_copy);
            elems_copy = NULL;
            free(in);
            in = NULL;
        }else{
            printf("The element from these key don't exist!\n");
            free(in);
            in = NULL;
        }
    }else
        printf("The table is empty\n");
}

void oat_search_elems(Table *table, char* in, Item* pitem)
{
    int ind = 0, i = table->csize1, rel = 1;
    for(int j = 0; j < table->msize; j++){
        ind = oat_double_hashing(in, &i, table->msize);
        if(table->ks2[ind].busy){
            if( (strcmp(table->ks2[ind].key, in) == 0) && (table->ks2[ind].release == rel) ){
                rel++;
                pitem->info = (char*)calloc((strlen(table->ks2[ind].element->info) + 1), sizeof(char));
                strcpy(pitem->info, table->ks2[ind].element->info);
            }
        }
    }
}
//-------------------------------------------------------------------------
void oat_copy_elems(Table *table) // Special Function -> case 1:
{
    if(chfull_table){
        while(getchar() != '\n');
        printf("Enter the key 2:\n");
        char* in = readline();
        int ind = 0, i = table->csize1, ver = oat_count_release(table, in);
        ver--;
        int check = -1, meter = 0;
        if( (ver > 0) && (in != NULL) ){
            Table* copy_table = (Table*)malloc(sizeof(Table));
            copy_table->msize = table->msize;
            copy_table->ks1 = (Keyspace1*)calloc(table->msize, sizeof(Keyspace1));
            copy_table->ks2 = (Keyspace2*)calloc(table->msize, sizeof(Keyspace2));
            copy_table->csize1 = 0;
            while( ((ind = oat_search_delete(table, in, &i)) >= 0) && (meter < table->msize) )
            {
                if(ind != check){
                    copy_table->ks2[ind].busy = 1;
                    copy_table->ks2[ind].key = (char*)calloc((strlen(in) + 1), sizeof(char));
                    strcpy(copy_table->ks2[ind].key, in);
                    copy_table->ks2[ind].release = table->ks2[ind].release;
                    copy_table->ks2[ind].element = (Item*)malloc(sizeof(Item));
                    copy_table->ks2[ind].element->info = (char*)calloc((strlen(table->ks2[ind].element->info) + 1), sizeof(char));
                    strcpy(copy_table->ks2[ind].element->info, table->ks2[ind].element->info);
                    copy_table->ks2[ind].element->p2 = &copy_table->ks2[ind];
                    copy_table->ks1[copy_table->csize1].busy = 1;
                    copy_table->ks1[copy_table->csize1].key = (char*)calloc((strlen(table->ks2[ind].element->p1->key) + 1), sizeof(char));
                    strcpy(copy_table->ks1[copy_table->csize1].key, table->ks2[ind].element->p1->key);
                    copy_table->ks1[copy_table->csize1].release = table->ks2[ind].release;
                    copy_table->ks1[copy_table->csize1].element = copy_table->ks2[ind].element;
                    copy_table->ks1[copy_table->csize1].element->p1 = &copy_table->ks1[copy_table->csize1];
                    copy_table->csize1++;
                    check = ind;
                }
                meter++;
            }
            if(meter != 0){
                free(in);
                in = NULL;
                for(int i = 0; i < copy_table->msize; i++){ // вывод скопированного
                    if(copy_table->ks1[i].busy){
                        printf("[ release: %d || key1: %s || key2: %s || info: %s ]\n", copy_table->ks1[i].release, copy_table->ks1[i].key, table->ks1[i].element->p2->key, table->ks1[i].element->info);
                        copy_table->ks1[i].busy = 0;
                        copy_table->ks1[i].element->p2->busy = 0;
                        free(copy_table->ks1[i].key);
                        copy_table->ks1[i].key = NULL;
                        free(copy_table->ks1[i].element->p2->key);
                        copy_table->ks1[i].element->p2->key = NULL;
                        free(copy_table->ks1[i].element->info);
                        free(copy_table->ks1[i].element);
                        copy_table->ks1[i].element = NULL;
                    }
                }
                    free(copy_table->ks1);
                    copy_table->ks1 = NULL;
                    free(copy_table->ks2);
                    copy_table->ks2 = NULL;
                    free(copy_table);
                    copy_table = NULL;
            }else{
                printf("The element from these key don't exist!\n");
                free(in);
                in = NULL;
            }
        }else{
            printf("The element from these key don't exist!\n");
            free(in);
            in = NULL;
        }
    }else
        printf("The table is empty!\n");
}
//----------------------------------------------------------------------------
//
// Функции для Look Up Table
//
//----------------------------------------------------------------------------
int lpt_count_release(Table *table, const char* in)
{

    int ver = 0;
    for(int i = 0; i < table->msize; i++){
        if(table->ks1[i].busy)
            if(strcmp(table->ks1[i].key, in) == 0)
                ver++;
    }
    return ++ver;
}
//----------------------------------------------------------------------------
int lpt_insert(Table *table, Item* elem, char* in){ // Вставка в Look Up Table

    while(in != NULL){
        if(table->csize1 < table->msize){
            int ver = lpt_count_release(table, in);
            table->ks1[table->csize1].busy = 1;
            table->ks1[table->csize1].key = (char*)calloc((strlen(in) + 1), sizeof(char));
            strcpy(table->ks1[table->csize1].key, in);
            table->ks1[table->csize1].element = elem;
            table->ks1[table->csize1].element->p1 = &table->ks1[table->csize1];
            table->ks1[table->csize1].release = ver;
            break;
        }else{
            printf("The table is overflow, if you want to add an element\n");
            printf("You need to reorganize it. To do this, go through this path:\n");
            printf("[Special functions] -> [LPT] -> [Reorganize]\n");
            break;
        }
    }
    if(in == NULL){
        printf("Incorrect key!\n");
        return 1;
    }
    if(in != NULL){
        return 0;
    }
}
//-------------------------------------------------------------------------------------
void lpt_delmany_elems(Table *table) // Special Function -> case 2: | Delete -> case 2:
{
    if(chfull_table){
        while(getchar() != '\n');
        printf("Please enter the 1 key:\n");
        char* in = readline();
        int ind = 0, ver = lpt_count_release(table, in);
        ver--;
        if((ver > 0)&&(in != NULL)){
            while( (ind = lpt_search_delete(table, in)) >= 0 )
            {
                table->ks1[ind].busy = 0;
                free(table->ks1[ind].key);
                table->ks1[ind].key = NULL;
                table->ks1[ind].element->p1 = NULL;
                free(table->ks1[ind].element->info);
                table->ks1[ind].element->info = NULL;
                table->ks1[ind].element->p2->busy = 0;
                free(table->ks1[ind].element->p2->key);
                table->ks1[ind].element->p2->key = NULL;
                table->ks1[ind].element->p2 = NULL;
                free(table->ks1[ind].element);
                table->ks1[ind].element = NULL;
            }
            free(in);
            in = NULL;
        }else{
            printf("The element with these keys don't exist!\n");
            free(in);
            in = NULL;
        }
    }else
        printf("The table is empty, nothing to do!\n");
}

int lpt_search_delete(Table *table, char* in)
{
    for(int j = 0; j < table->msize; j++){
        if(table->ks1[j].busy)
            if(strcmp(table->ks1[j].key, in) == 0)
                return j;
    }
    return -1;
}
//--------------------------------------------------------------------
void lpt_find_elems(Table *table) // Find -> case 2:
{
    while(getchar() != '\n');
    printf("Enter the key 1:\n");
    char* in = readline();
    int ver = lpt_count_release(table, in);
    ver--;
    if( (ver > 0) && (in != NULL) ){
        Item* elems_copy = (Item*)calloc(ver, sizeof(Item));
        Item* pitem = (Item*)malloc(sizeof(Item));
        for(int i = 0; i < ver; i++){
            lpt_search_elems(table, in, pitem);
            elems_copy[i].info = (char*)calloc((strlen(pitem->info) + 1), sizeof(char));
            strcpy(elems_copy[i].info, pitem->info);
            free(pitem->info);
            elems_copy[i].p1 = NULL;
            elems_copy[i].p2 = NULL;
        }
        free(pitem);
        int rel = 1;
        for(int i = 0; i < ver; i++){
            printf("[ release: %d || key1: %s || info: %s ]\n", rel, in, elems_copy[i].info);
            free(elems_copy[i].info);
            elems_copy[i].info = NULL;
            rel++;
        }
        free(in);
        in = NULL;
        free(elems_copy);
        elems_copy = NULL;
    }else{
        printf("The element from these key don't exist!\n");
        free(in);
        in = NULL;
    }
}

void lpt_search_elems(Table *table, char* in, Item* pitem)
{
    int rel = 1;
    for(int i = 0; i < table->msize; i++){
        if(table->ks1[i].busy){
            if( (strcmp(table->ks1[i].key, in) == 0) && (table->ks1[i].release == rel) ){
                rel++;
                pitem->info = (char*)calloc((strlen(table->ks1[i].element->info) + 1), sizeof(char));
                strcpy(pitem->info, table->ks1[i].element->info);
            }
        }
    }
}
//----------------------------------------------------------------------------------
void lpt_reorganize(Table* table) // Special Function -> case 3:
{
    if(chfull_table){
        int i = 0, a = 0;
        while( i < table->csize1 ){
            if(table->ks1[i].busy == 0){
                for(int j = i; j < table->msize; j++){
                    if(table->ks1[j].busy == 1){
                        table->ks1[i].busy = 1;
                        table->ks1[j].busy = 0;
                        table->ks1[i].key = (char*)calloc((strlen(table->ks1[j].key) + 1), sizeof(char));
                        strcpy(table->ks1[i].key, table->ks1[j].key);
                        free(table->ks1[j].key);
                        table->ks1[j].key = NULL;
                        table->ks1[i].release = table->ks1[j].release;
                        table->ks1[i].element = table->ks1[j].element;
                        table->ks1[i].element->p1 = &table->ks1[i];
                        table->ks1[j].element = NULL;
                        a++;
                        printf("signal: ! ");
                        break;
                    }
                }
            }
            i++;
        }
        table->csize1 = table->csize1 - a;
        if(a == 0)
            table->csize1 = 0;
    }else
        table->csize1 = 0;
}
//------------------------------------------------------------------------------------
//
// Функции обработки Таблицы
//
//------------------------------------------------------------------------------------
void Add(Table *table){ // Ввод элемента в таблицу
    if(table->csize1 < table->msize){
        int a = 0, b = 0;
        Item* element = (Item*)malloc(sizeof(Item));
        while(getchar() != '\n');
        printf("Enter the information:\n");
        element->info = readline();
        printf("Enter the key 1:\n");
        char* key1 = readline();
        printf("Enter the key 2:\n");
        char* key2 = readline();
        if(check_unique_keys(table, key1, key2)){
            a = lpt_insert(table, element, key1);
            if(a != 1){
                b = oat_insert(table, element, key2);
                table->csize1++;
            }if(b){
                int i = lpt_search_delete(table, element->info);
                table->ks1[i].busy = 0;
                free(table->ks1[i].key);
                table->ks1[i].key = NULL;
                free(table->ks1[i].element->info);
                table->ks1[i].element = NULL;
                element = NULL;
            }
        }else{
            printf("This key's exist, enter another key's!\n");
        }
        element = NULL;
        free(key1);
        key1 = NULL;
        free(key2);
        key2 = NULL;
    }else
        printf("Not enough memory to write!\n");
}
//
//------------------------------------------------------------------------------------
//
void Find(Table* table){ // Поиск элемента в таблице
    if(chfull_table(table)){
        int a;
        printf("1) Finding element by composite key\n");
        printf("2) Finding all elements by key 1\n");
        printf("3) Finding all elements by key 2\n");
        printf("4) Press any others to Quit\n");
        getInt(&a);
        switch(a)
        {
            case 1: find_compkey(table); getchar(); break;
            case 2: lpt_find_elems(table); getchar(); break;
            case 3: oat_find_elems(table); getchar(); break;
        }
    }else{
        printf("The table is empty, nothing to find!\n");
    }
}

void find_compkey(Table* table){ // [Find] -> case 1: (main)
    while(getchar() != '\n');
    printf("Enter the key 1:\n");
    char* key1 = readline();
    printf("Enter the key 2:\n");
    char* key2 = readline();
    find_element(table, key1, key2);
    free(key1);
    free(key2);
    key1 = NULL;
    key2 = NULL;
}

void find_element(Table* table, char* key1, char* key2){ // [Find] -> case 1: (function) 
    int a = 0, ind = 0;
    ind = lpt_search_delete(table, key1);
    if(ind >= 0){
        if(strcmp((table->ks1[ind].element->p2->key), key2) == 0){
            printf("\n[Success!]\n\nDo you want to print it?\n");
            printf("[1 - Yes]\n[Any - No]\n");
            scanf("%d", &a);
            switch(a){
                case 1: printf("[ release: %d || key1: %s || key2: %s || info: %s ]\n", table->ks1[ind].release, table->ks1[ind].key, table->ks1[ind].key, table->ks1[ind].element->info ); break;
                default: printf("Returning...\n"); break;
            }
        }else
            printf("The element from these keys don't exist!");
    }else
        printf("The element from these keys don't exist!");
}
//
//------------------------------------------------------------------------------------
//
void Delete(Table *table){ // Удаление элемента в Таблице
    if(chfull_table(table)){
        int a;
        printf("1) Removing element by composite key\n");
        printf("2) Removing all elements by key 1\n");
        printf("3) Removing all elements by key 2\n");
        printf("4) Press any others to Quit\n");
        getInt(&a);
            switch(a)
            {
                case 1: delete_comkey(table); break;
                case 2: lpt_delmany_elems(table); break;
                case 3: oat_delmany_elems(table); break;
            }
    }else{
        printf("The table is empty, nothing to delete!\n");
    }
}

void delete_comkey(Table* table){ // Delete -> case 1: (main)
    while(getchar() != '\n');
    printf("Enter the key 1:\n");
    char* key1 = readline();
    printf("Enter the key 2:\n");
    char* key2 = readline();
    delete_element(table, key1, key2);
    free(key1);
    free(key2);
    key1 = NULL;
    key2 = NULL;
}

void delete_element(Table* table, char* key1, char* key2){ // Delete -> case 1: (function)
    int ind = 0, i = 0, fail = 0;
    while(i < table->csize1){
        ind = lpt_search_delete_comkey(table, key1, key2);
        if(ind >= 0){
            table->ks1[ind].busy = 0;
            free(table->ks1[ind].key);
            table->ks1[ind].key = NULL;
            table->ks1[ind].element->p2->busy = 0;
            free(table->ks1[ind].element->p2->key);
            table->ks1[ind].element->p2->key = NULL;
            table->ks1[ind].element->p2 = NULL;
            table->ks1[ind].element->p1 = NULL;
            free(table->ks1[ind].element->info);
            table->ks1[ind].element->info = NULL;
            free(table->ks1[ind].element);
            table->ks1[ind].element = NULL;
            fail++;
            break;
        }
    i++;
    }
    if(fail == 0)
        printf("The element from these keys don't exist!\n");
}

int lpt_search_delete_comkey(Table *table, char* key1, char* key2)
{
    for(int j = 0; j < table->msize; j++){
        if(table->ks1[j].busy)
            if((strcmp(table->ks1[j].key, key1) == 0) && (strcmp(table->ks1[j].element->p2->key, key2) == 0))
                return j;
    }
    return -1;
}
//
//------------------------------------------------------------------------------------
//
void Special_Operations(Table* table){ // Индивидуальные функции для каждого из пространств

    int a;
    printf("1) [OAT]: Search in the table of all versions of the element\n");
    printf("2) [LPT]: Removing all versions of an element from the table 1\n");
    printf("3) [LPT]: Reorganize\n");
    printf("4) Press any others to Quit\n");
    getInt(&a);
    switch(a)
    {
        case 1: oat_copy_elems(table); break;
        case 2: lpt_delmany_elems(table); break;
        case 3: lpt_reorganize(table); break;
    }
}

//------------------------------------------------------------------------------------
void Show(Table *table){ // Вывод содержимого Таблицы
    if(chfull_table(table)){
        for(int i = 0; i < table->csize1; i++){
            if(table->ks1[i].busy)
                printf("[ release key1: %d || key1: %s || release key2: %d || key2: %s || info: %s ]\n", table->ks1[i].release, table->ks1[i].key, table->ks1[i].element->p2->release, table->ks1[i].element->p2->key, table->ks1[i].element->info);
        }
    }else{
        printf("The table is empty, nothing to show\n");
    }
}


//------------------------------------------------------------------------------------
//
// Общие функции для Программы
//
int dialog(){ // Интерфейс
	int rc = 0;
	int n = 0;
	do{
		if(rc < 0 || rc >= 6)
            printf("You are wrong. Repeate, please!\n");
        printf("0. Quit\n1. Add\n2. Find\n3. Delete\n4. Show\n5. Special Operations\n");
		printf("Make your choice: -->\n");
		n = getInt(&rc); // ввод номера пункта
		if(n == 0) // конец ввода
		rc = 0;
	} while(rc < 0 || rc >= 6);
	return rc;
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
int getInt(int *a){ // Проверка на тип ввода
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
void clear_table(Table* table){ // Очистка таблицы при выходе
    if(chfull_table){
        for(int i = 0; i < table->msize; i++){
            if(table->ks1[i].busy){
                free(table->ks1[i].key);
                table->ks1[i].key = NULL;
                free(table->ks1[i].element->info);
                table->ks1[i].element->info = NULL;
                free(table->ks1[i].element->p2->key);
                table->ks1[i].element->p2->key = NULL;
                table->ks1[i].element->p2->busy = 0;
                table->ks1[i].element->p2 = NULL;
                table->ks1[i].element->p1 = NULL;
                table->ks1[i].busy = 0;
                free(table->ks1[i].element);
                table->ks1[i].element = NULL;
            }
        }
    }
    free(table->ks1);
    table->ks1 = NULL;
    free(table->ks2);
    table->ks2 = NULL;
    free(table);
    table = NULL;
}
int chfull_table(Table* table){ // Проверка на наличие элементов в таблице
    int a = 0;
    for(int i = 0; i < table->csize1; i++){
        if(table->ks1[i].busy)
            a++;
    }
    return a;
}

void set_busy(Table* table){
    for(int i = 0; i < table->msize; i++){
        table->ks1[i].busy = 0;
        table->ks2[i].busy = 0;
    }
}

int check_unique_keys(Table* table, char* key1, char* key2){
    int ind = 0;
    ind = lpt_search_delete(table, key1);
    if(ind >= 0)
        if(strcmp((table->ks1[ind].element->p2->key), key2) == 0)
            return 0;
        return 1;
}
