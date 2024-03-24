#include "lab3bio.h"
//
// Функции для Open Address Table
//
int oat_hash1(char* in, int size)
{
    unsigned int hash2 = 2139062143;
    for(int i = 0; i < strlen(in); i++){
        hash2 = 37 * hash2 + in[i];
    }
    return abs(hash2);
}

int oat_hash2(char* in, int size)
{
    int hash1 = 0;
    for(int i = 0; i < strlen(in); i++){
        hash1 = hash1 + in[i];
    }
    return abs(hash1);
}

int oat_double_hashing(char* in, int* i, int size)
{
    int dh = 0, h1 = oat_hash1(in, size), h2 = oat_hash2(in, size);
    dh = ( h1 + (*i)*h2 ) % size;
    (*i) = (*i) + 1;
    return dh;
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

void oat_insert(Table* table, char* info, char* key, int info_size, int offset){ // вставка в Open Adress Table
    int ind = 0, ver = 0;
    ver = oat_count_release(table, key);
    ind = oat_search_insert(table, key);
    table->ks2[ind].busy = 1;                          // занятость
    table->ks2[ind].key = (char*)calloc((strlen(key) + 1), sizeof(char));
    strcpy(table->ks2[ind].key, key);
    table->ks2[ind].release = ver;                 // установили версию
    table->ks2[ind].offset = offset;
    table->ks2[ind].size = info_size;
}

int oat_search_insert(Table *table, char* in) // поиск для вставки в Open Adress Table
{
    int j = 0, ind = 0, i = 0;
    while(1){
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

int oat_count_release(Table *table, char* in) // подсчет количества версий
{
    int ver = 0, ind = 0, i = table->csize;
    for(int j = 0; j < table->msize; j++){
        ind = oat_double_hashing(in, &i, table->msize);
        if(table->ks2[ind].busy)
            if(strcmp(table->ks2[ind].key, in) == 0)
                    ver++;
        }
    return ++ver;
}

void oat_delmany_elems(Table *table) // удаление для заданного ключа OAT | Delete -> case 3:
{
    while(getchar() != '\n');
    printf("Enter the key 2:\n");
    char* in = readline();
    int ind = 0, i = table->csize, ver = oat_count_release(table, in);
    if(ver > 0){
        while((ind = oat_search_delete(table, in, &i)) >= 0 )
        {
            table->ks2[ind].busy = 0;
            free(table->ks2[ind].key);
            table->ks2[ind].key = NULL;
            for(int i = 0; i < table->csize; i++){
              if(table->ks1[i].offset == table->ks2[ind].offset){
                table->ks1[i].busy = 0;
                free(table->ks1[i].key);
                table->ks1[i].key = NULL;
              }
            }
        }
        free(in);
        in = NULL;
    }else{
        printf("The element from these key don't exist!\n");
        free(in);
        in = NULL;
    }
}

void oat_find_elems(Table *table, FILE* file) // Find -> case 3:
{
    if(chfull_table){
        while(getchar() != '\n');
        printf("Enter the key 2:\n");
        char* in = readline();
        int ver = oat_count_release(table, in);
        ver--;
        if( ver > 0 ){
            Item* elems_copy = (Item*)calloc(ver, sizeof(Item));
            Item* pitem = (Item*)malloc(sizeof(Item));
            for(int i = 0; i < ver; i++){
                oat_search_elems(table, in, pitem, file);
                elems_copy[i].info = (char*)calloc((strlen(pitem->info) + 1), sizeof(char));
                strcpy(elems_copy[i].info, pitem->info);
                free(pitem->info);
            }
            int rel = 1;
            for(int i = 0; i < ver; i++){
                printf("[ release: %d || key2: %s || info: %s ]\n", rel, in, elems_copy[i].info);
                rel++;
                free(elems_copy[i].info);
                elems_copy[i].info = NULL;
            }
            int total_offset = 0;
            for(int i = 0; i < table->msize; i++){
                total_offset = total_offset + table->ks1[i].size;
            }
            fseek(file, Indent + total_offset, SEEK_SET);
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

void oat_search_elems(Table *table, char* in, Item* pitem, FILE* file)
{
    int ind = 0, i = table->csize, rel = 1;
    for(int j = 0; j < table->msize; j++){
        ind = oat_double_hashing(in, &i, table->msize);
        if(table->ks2[ind].busy){
            if( (strcmp(table->ks2[ind].key, in) == 0) && (table->ks2[ind].release == rel) ){
                rel++;
                pitem->info = (char*)calloc(table->ks2[ind].size, sizeof(char));
                fseek(file, table->ks2[ind].offset, SEEK_SET);
                fread(pitem->info, sizeof(char), table->ks2[ind].size, file);
            }
        }
    }
}

void oat_copy_elems(Table *table, FILE* file) // Special Function -> case 1:
{
    if(chfull_table){
        while(getchar() != '\n');
        printf("Enter the key 2:\n");
        char* in = readline();
        int ind = 0, i = table->csize, ver = oat_count_release(table, in);
        ver--;
        int check = -1, meter = 0;
        if(ver > 0){
            Table* copy_table = (Table*)malloc(sizeof(Table));
            copy_table->msize = table->msize;
            copy_table->ks1 = (Keyspace1*)calloc(table->msize, sizeof(Keyspace1));
            copy_table->ks2 = (Keyspace2*)calloc(table->msize, sizeof(Keyspace2));
            copy_table->csize = 0;
            while( ((ind = oat_search_delete(table, in, &i)) >= 0) && (meter < table->msize) )
            {
                if(ind != check){
                    copy_table->ks2[ind].busy = 1;
                    copy_table->ks2[ind].key = (char*)calloc((strlen(in) + 1), sizeof(char));
                    strcpy(copy_table->ks2[ind].key, in);
                    copy_table->ks2[ind].release = table->ks2[ind].release;
                    copy_table->ks2[ind].offset = table->ks2[ind].offset;
                    copy_table->ks2[ind].size = table->ks2[ind].size;
                    copy_table->ks1[copy_table->csize].busy = 1;
                    copy_table->ks1[copy_table->csize].key = (char*)calloc((strlen(in) + 1), sizeof(char));
                    strcpy(copy_table->ks1[copy_table->csize].key, in);
                    copy_table->ks1[copy_table->csize].release = table->ks2[ind].release;
                    copy_table->ks1[copy_table->csize].offset = table->ks2[ind].offset;
                    copy_table->ks1[copy_table->csize].size = table->ks2[ind].size;
                    copy_table->csize++;
                    check = ind;
                }
                meter++;
            }
            if(meter != 0){
                free(in);
                in = NULL;
                for(int i = 0; i < copy_table->msize; i++){ // вывод скопированного
                    if(copy_table->ks1[i].busy){
                        int ind = show_search(table, i);
                        char* key2 = (char*)calloc((strlen(copy_table->ks2[ind].key) + 1), sizeof(char));
                        strcpy(key2, copy_table->ks2[ind].key);
                        free(copy_table->ks2[ind].key);
                        copy_table->ks2[ind].key = NULL;
                        char* info = (char*)calloc(copy_table->ks1[i].size, sizeof(char));
                        fseek(file, copy_table->ks1[i].offset, SEEK_SET);
                        fread(info, sizeof(char), copy_table->ks1[i].size, file);
                        printf("[ release: %d || key1: %s || key2: %s || info: %s ]\n", copy_table->ks1[i].release, copy_table->ks1[i].key, key2, info);
                        copy_table->ks1[i].busy = 0;
                        free(copy_table->ks1[i].key);
                        copy_table->ks1[i].key = NULL;
                        free(key2);
                        key2 = NULL;
                        free(info);
                        info = NULL;
                    }
                }
                int total_offset = 0;
                for(int i = 0; i < table->msize; i++){
                    total_offset = total_offset + table->ks1[i].size;
                }
                fseek(file, Indent + total_offset, SEEK_SET);
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
//
// Функции для Look Up Table
//
void lpt_reorganize(Table* table, FILE* file) // Special Function -> case 3:
{
    if(chfull_table){
        int i = 0, a = 0, tmp = 0;
        while( i < table->csize ){
            if(table->ks1[i].busy)
                tmp = tmp + table->ks1[i].size;
            if(table->ks1[i].busy == 0){
                for(int j = i; j < table->msize; j++){
                    if(table->ks1[j].busy){
                        table->ks1[i].busy = 1;
                        table->ks1[j].busy = 0;
                        table->ks1[i].key = (char*)calloc((strlen(table->ks1[j].key) + 1), sizeof(char));
                        strcpy(table->ks1[i].key, table->ks1[j].key);
                        free(table->ks1[j].key);
                        table->ks1[j].key = NULL;
                        table->ks1[i].release = table->ks1[j].release;
                        table->ks1[i].size = table->ks1[j].size;
                        int ind = show_search(table, j);
                        char *tmpstr = (char*)calloc(table->ks1[j].size, sizeof(char));
                        fseek(file, table->ks1[j].offset, SEEK_SET);
                        fread(tmpstr, sizeof(char), table->ks1[j].size, file);
                        fseek(file, (Indent + tmp), SEEK_SET);
                        fwrite(tmpstr, sizeof(char), table->ks1[j].size, file);
                        table->ks1[i].offset = (ftell(file) - table->ks1[j].size);
                        table->ks2[ind].offset = table->ks1[i].offset;
                        tmp = tmp + table->ks1[j].size;
                        free(tmpstr);
                        tmpstr = NULL;
                        break;
                    }
                }
            }
            i++;
        }
        for(int i = 0; i < table->msize; i++){
            if(table->ks1[i].busy == 0)
                a++;
        }
        table->csize = table->csize - a;
        printf("[ The Table reorganized! ]\n");
    }else{
        table->csize = 0;
        for(int i = 0; i < table->msize; i++){
            table->ks1[i].size = 0;
            table->ks1[i].offset = 0;
            table->ks2[i].size = 0;
            table->ks2[i].offset = 0;
        }
        printf("[ The Table reorginized! ]\n");
    }
}

void lpt_delmany_elems(Table *table) // Special Function -> case 2: | Delete -> case 2:
{
    if(chfull_table){
        while(getchar() != '\n');
        printf("Please enter the 1 key:\n");
        char* in = readline();
        int ind = 0, ver = lpt_count_release(table, in);
        ver--;
        if(ver > 0){
            while( (ind = lpt_search_delete(table, in)) >= 0 )
            {
                table->ks1[ind].busy = 0;
                free(table->ks1[ind].key);
                table->ks1[ind].key = NULL;
                int oat = show_search(table, ind);
                table->ks2[oat].busy = 0;
                free(table->ks2[oat].key);
                table->ks2[oat].key = NULL;
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

int lpt_insert(Table *table, char* info, char* key, int info_size, int offset){ // Вставка в Look Up Table
    if(table->csize < table->msize){
        int ver = lpt_count_release(table, key);
        table->ks1[table->csize].busy = 1;
        table->ks1[table->csize].key = (char*)calloc((strlen(key) + 1), sizeof(char));
        strcpy(table->ks1[table->csize].key, key);
        table->ks1[table->csize].release = ver;
        table->ks1[table->csize].offset = offset;
        table->ks1[table->csize].size = info_size;
        return 1;
    }else{
        printf("The table is overflow, if you want to add an element\n");
        printf("You need to reorganize it. To do this, go through this path:\n");
        printf("[Special functions] -> [LPT] -> [Reorganize]\n");
        return 0;
    }
}

int lpt_search_delete_comkey(Table *table, char* key1, char* key2)
{
    int ind = 0, i = 0;
    for(int j = 0; j < table->msize; j++){
        if(table->ks1[j].busy){
            for(int i = 0; i < table->msize; i++){
                ind = oat_search_delete(table, key2, &i);
                if(table->ks2[ind].busy)
                    if( ((strcmp(table->ks1[j].key, key1) == 0) && (strcmp(table->ks2[ind].key, key2) == 0)) ){
                        table->ks2[ind].busy = 0;
                        free(table->ks2[ind].key);
                        table->ks2[ind].key = NULL;
                        return j;
                    }
            }
        }
    }
    return -1;
}

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

void lpt_find_elems(Table *table, FILE* file) // Find -> case 2:
{
    while(getchar() != '\n');
    printf("Enter the key 1:\n");
    char* in = readline();
    int ver = lpt_count_release(table, in);
    ver--;
    if(ver > 0){
        Item* elems_copy = (Item*)calloc(ver, sizeof(Item));
        Item* pitem = (Item*)malloc(sizeof(Item));
        for(int i = 0; i < ver; i++){
            lpt_search_elems(table, in, pitem, file);
            elems_copy[i].info = (char*)calloc((strlen(pitem->info) + 1), sizeof(char));
            strcpy(elems_copy[i].info, pitem->info);
            free(pitem->info);
        }
        free(pitem);
        int rel = 1;
        for(int i = 0; i < ver; i++){
            printf("[ release: %d || key1: %s || info: %s ]\n", rel, in, elems_copy[i].info);
            free(elems_copy[i].info);
            elems_copy[i].info = NULL;
            rel++;
        }
        int total_offset = 0;
        for(int i = 0; i < table->msize; i++){
            total_offset = total_offset + table->ks1[i].size;
        }
        fseek(file, Indent + total_offset, SEEK_SET);
        free(in);
        in = NULL;
        pitem = NULL;
        free(elems_copy);
        elems_copy = NULL;
    }else{
        printf("The element from these key don't exist!\n");
        free(in);
        in = NULL;
    }
}

void lpt_search_elems(Table *table, char* in, Item* pitem, FILE* file)
{
    int rel = 1;
    for(int i = 0; i < table->msize; i++){
        if(table->ks1[i].busy){
            if( (strcmp(table->ks1[i].key, in) == 0) && (table->ks1[i].release == rel) ){
                pitem->info = (char*)calloc(table->ks1[i].size, sizeof(char));
                fseek(file, table->ks1[i].offset, SEEK_SET);
                fread(pitem->info, sizeof(char), table->ks1[i].size, file);
            }
        }
    }
}
//
// Функции обработки Таблицы
//
FILE* file_interaction(Table* table){
    int a = 0;
    printf("Enter the name of file:\n");
    char* fname = readline();
    FILE* file = fopen(fname, "r+b");
    if(file == NULL){
        printf("The file \"%s\" wasn't found. Do you want to create file with this name?\n", fname);
        printf("[Yes - 1]\n[No - press any button]\n");
        scanf("%d", &a);
        if(a == 1){
            file = fopen(fname, "w+b");
            printf("Enter the size of table\n");
            int size = 0;
            while(getInt(&size) == 0){
                printf("You can't allocate memory this way, enter an integer:\n");
            }
            table->msize = size;
            table->csize = 0;
            table->ks1 = (Keyspace1*)calloc(table->msize, sizeof(Keyspace1));
            table->ks2 = (Keyspace2*)calloc(table->msize, sizeof(Keyspace2));
            set_table_default(table);
            fseek(file, Indent, SEEK_SET);
            free(fname);
            fname = NULL;
            return file;
        }else{
            printf("Leaving...\n");
            free(fname);
            fname = NULL;
            return NULL;
		}
    }else{
        printf("The file \"%s\" was found!. Do you want to open file with this name?\n", fname);
        printf("[Yes - 1]\n[No - press any button]\n");
        scanf("%d", &a);
        if(a == 1){
            fseek(file, 0, SEEK_END);
            if(ftell(file) != 0){
                fseek(file, 0, SEEK_SET);
                fread(&(table->msize), sizeof(int), 1, file);
                fread(&(table->csize), sizeof(int), 1, file);
                table->ks1 = (Keyspace1*)calloc(table->msize, sizeof(Keyspace1));
                table->ks2 = (Keyspace2*)calloc(table->msize, sizeof(Keyspace2));
                for(int i = 0; i < table->csize; i++){ // считали 1е пространство (просматриваемая)
                    int key_size = 0;
                    fread(&(table->ks1[i].busy), sizeof(int), 1, file);
                    if(table->ks1[i].busy){
                        fread(&key_size, sizeof(int), 1, file); // узнаю длину ключа
                        table->ks1[i].key = (char*)calloc(key_size, sizeof(char)); // выделяю соотв память
                        fread(table->ks1[i].key, sizeof(char), key_size, file); // читаю ключ
                    }else{
                        table->ks1[i].key = NULL;
                    }
                    fread(&(table->ks1[i].release), sizeof(int), 1, file);
                    fread(&(table->ks1[i].size), sizeof(int), 1, file);
                    fread(&(table->ks1[i].offset), sizeof(int), 1, file);
                }
                for(int i = 0; i < table->msize; i++){ // считали 2е пространство (хеш-бабай)
                    int key_size = 0;
                    fread(&(table->ks2[i].busy), sizeof(int), 1, file);
                    if(table->ks2[i].busy){
                        fread(&key_size, sizeof(int), 1, file); // узнаю длину ключа
                        table->ks2[i].key = (char*)calloc(key_size, sizeof(char)); // выделяю соотв память
                        fread(table->ks2[i].key, sizeof(char), key_size, file); // читаю ключ
                    }else{
                        table->ks1[i].key = NULL;
                    }
                    fread(&(table->ks2[i].release), sizeof(int), 1, file);
                    fread(&(table->ks2[i].size), sizeof(int), 1, file);
                    fread(&(table->ks2[i].offset), sizeof(int), 1, file);
                }
                free(fname);
                fname = NULL;
                return file;
            }else{
                printf("File is empty!\n");
                fclose(file);
                free(fname);
                fname = NULL;
                return NULL;
            }
        }else{
            printf("Leaving...!\n");
            fclose(file);
            free(fname);
            fname = NULL;
            return NULL;
        }
    }    
}

void Add(Table* table, FILE* file){
    if(file != NULL){
        if(table->csize < table->msize){
            while(getchar() != '\n');
            printf("Enter the information:\n");
            char* info = readline();
            printf("Enter the key 1:\n");
            char* key1 = readline();
            printf("Enter the key 2:\n");
            char* key2 = readline();
            int info_size = (strlen(info) + 1), offset = 0;
            fwrite(info, sizeof(char), info_size, file);
            offset = (ftell(file) - info_size);
            if(check_unique_keys(table, key1, key2)){
                if(lpt_insert(table, info, key1, info_size, offset)){
                oat_insert(table, info, key2, info_size, offset);
                table->csize++;
                }else{
                printf("Table is overflow, delete some elements and reorganize the table if you want add\n");
                }
            }else{
                printf("This key's exist, enter another key's!\n");
            }
            free(info);
            info = NULL;
            free(key1);
            key1 = NULL;
            free(key2);
            key2 = NULL;
        }else
            printf("Not enough memory to write!\n");
    }else
        printf("залупа конского масштаба\n");
}

void Find(Table* table, FILE* file){ // Поиск элемента в таблице
    if(chfull_table(table)){
        int a;
        printf("1) Finding element by composite key\n");
        printf("2) Finding all elements by key 1\n");
        printf("3) Finding all elements by key 2\n");
        printf("4) Press any others to Quit\n");
        getInt(&a);
        switch(a)
        {
            case 1: find_compkey(table, file); getchar(); break;
            case 2: lpt_find_elems(table, file); getchar(); break;
            case 3: oat_find_elems(table, file); getchar(); break;
        }
    }else{
        printf("The table is empty, nothing to find!\n");
    }
}

void find_compkey(Table* table, FILE* file){ // [Find] -> case 1: (main)
    while(getchar() != '\n');
    printf("Enter the key 1:\n");
    char* key1 = readline();
    printf("Enter the key 2:\n");
    char* key2 = readline();
    find_element(table, key1, key2, file);
    free(key1);
    free(key2);
    key1 = NULL;
    key2 = NULL;
}

void find_element(Table* table, char* key1, char* key2, FILE* file){ // [Find] -> case 1: (function) 
    int a = 0, ind = 0, detect = 0;
    ind = lpt_search_delete(table, key1);
    if(ind >= 0){
        for(int j = 0; j < table->msize; j++){
            if(table->ks1[ind].offset == table->ks2[j].offset){
                printf("\n[Success!]\n\nDo you want to print it?\n");
                printf("[1 - Yes]\n[Any - No]\n");
                detect++;
                scanf("%d", &a);
                fseek(file, table->ks1[ind].offset, SEEK_SET);
                char* info = (char*)calloc(table->ks1[ind].size, sizeof(char));
                fread(info, sizeof(char), table->ks1[ind].size, file);
                int total_offset = 0;
                for(int i = 0; i < table->msize; i++){
                    total_offset = total_offset + table->ks1[i].size;
                }
                fseek(file, Indent + total_offset, SEEK_SET);
                switch(a){
                    case 1: printf("[ release: %d || key1: %s || key2: %s || info: %s ]\n", table->ks1[ind].release, table->ks1[ind].key, table->ks2[j].key, info); break;
                    default: printf("Returning...\n"); break;
                }
                break;
            }
        }
        if(detect == 0)
            printf("The element from these keys don't exist!");
    }else
        printf("The element from these keys don't exist!");
}


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
    while(i < table->csize){
        ind = lpt_search_delete_comkey(table, key1, key2);
        if(ind >= 0){
            table->ks1[ind].busy = 0;
            free(table->ks1[ind].key);
            table->ks1[ind].key = NULL;
            fail++;
            break;
        }
    i++;
    }
    if(fail == 0)
        printf("The element from these keys don't exist!\n");
}

void Special_Operations(Table* table, FILE* file){ // Индивидуальные функции для каждого из пространств

    int a;
    printf("1) [OAT]: Search in the table of all versions of the element\n");
    printf("2) [LPT]: Removing all versions of an element from the table 1\n");
    printf("3) [LPT]: Reorganize\n");
    printf("4) Press any others to Quit\n");
    getInt(&a);
    switch(a)
    {
        case 1: oat_copy_elems(table, file); break;
        case 2: lpt_delmany_elems(table); break;
        case 3: lpt_reorganize(table, file); break;
    }
}

void Show(Table *table, FILE* file){ // Вывод содержимого Таблицы
    if(chfull_table(table)){
        char* info = NULL;
        for(int i = 0; i < table->csize; i++){
            if(table->ks1[i].busy){
                int ind = show_search(table, i);
                if(ind >= 0){
                    info = (char*)calloc(table->ks1[i].size, sizeof(char));
                    fseek(file, table->ks1[i].offset, SEEK_SET);
                    fread(info, sizeof(char), table->ks1[i].size, file);
                    printf("[ release key1: %d || key1: %s || release key2: %d || key2: %s || info: %s ]\n", table->ks1[i].release, table->ks1[i].key, table->ks2[ind].release, table->ks2[ind].key, info);
                    free(info);
                }else
                    printf("Something goes wrong ;-( \n");
            }
        }
        int total_offset = 0;
        for(int i = 0; i < table->msize; i++){
            total_offset = total_offset + table->ks1[i].size;
        }
        fseek(file, Indent + total_offset, SEEK_SET);
        info = NULL;
    }else{
        printf("The table is empty, nothing to show\n");
    }
}

int show_search(Table* table, int a){
    for(int i = 0; i < table->msize; i++){
        if( (table->ks2[i].busy) && (table->ks2[i].offset == table->ks1[a].offset) )
            return i;
    }
    return -1;
}

int check_unique_keys(Table* table, char* key1, char* key2){
    int indlpt = 0, indoat = 0, i = table->csize;
    indlpt = lpt_search_delete(table, key1);
    indoat = oat_search_delete(table, key2, &i);
    if((indlpt >= 0) && (indoat >= 0))
        if(table->ks1[indlpt].offset == table->ks2[indoat].offset)
            return 0;
        return 1;
}

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
                table->ks1[i].busy = 0;
                free(table->ks1[i].key);
                table->ks1[i].key = NULL;
            }
            if(table->ks2[i].busy){
                table->ks2[i].busy = 0;
                free(table->ks2[i].key);
                table->ks2[i].key = NULL;
            }
        }
    }
    free(table->ks1);
    table->ks1 = NULL;
    free(table->ks2);
    table->ks2 = NULL;
}
int chfull_table(Table* table){ // Проверка на наличие элементов в таблице
    int a = 0;
    for(int i = 0; i < table->csize; i++){
        if(table->ks1[i].busy)
            a++;
    }
    return a;
}

void save_table_to_file(Table* table, FILE* file){
    fseek(file, 0, SEEK_SET);
    fwrite(&(table->msize), sizeof(int), 1, file); // записал размер табл
    fwrite(&(table->csize), sizeof(int), 1, file); // записал актив. элем.
    for(int i = 0; i < table->csize; i++){ // записал 1е пространство (просматриваемая)
        fwrite(&table->ks1[i].busy, sizeof(int), 1, file);
        if(table->ks1[i].busy){
            int key_size = (strlen(table->ks1[i].key) + 1);
            fwrite(&key_size, sizeof(int), 1, file);
            fwrite(table->ks1[i].key, sizeof (char), key_size, file);
        }
        fwrite(&(table->ks1[i].release), sizeof(int), 1, file);
        fwrite(&(table->ks1[i].size), sizeof(int), 1, file);
        fwrite(&(table->ks1[i].offset), sizeof(int), 1, file);
    }
    for(int i = 0; i < table->msize; i++){ // записал 2е пространство (хеш-бабай)
        fwrite(&(table->ks2[i].busy), sizeof(int), 1, file);
        if(table->ks2[i].busy){
            int key_size = (strlen(table->ks2[i].key) + 1);
            fwrite(&key_size, sizeof(int), 1, file);
            fwrite(table->ks2[i].key, sizeof (char), key_size, file);
        }
        fwrite(&(table->ks2[i].release), sizeof(int), 1, file);
        fwrite(&(table->ks2[i].size), sizeof(int), 1, file);
        fwrite(&(table->ks2[i].offset), sizeof(int), 1, file);
    }
    printf("[ The table saved in file! ]\n");
}

void set_table_default(Table* table){
    for(int i = 0; i < table->msize; i++){
        table->ks1[i].busy = 0;
        table->ks1[i].size = 0;
        table->ks1[i].offset = 0;
        table->ks2[i].busy = 0;
        table->ks2[i].size = 0;
        table->ks2[i].offset = 0;
    }
}
