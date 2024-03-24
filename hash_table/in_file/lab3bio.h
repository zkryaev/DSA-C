#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define Indent 50000

typedef struct Item{
  char* info;
}Item;

typedef struct Keyspace1{
  int busy;                    // признак занятости элемента
  char* key;                   // ключ эемента
  int release;                 // номер версии элемента
  int size;                 // размер элемента
  int offset;               // смещение
}Keyspace1;

typedef struct Keyspace2{
  int busy;
  char* key;
  int release;
  int size;                
  int offset;              
}Keyspace2;

typedef struct Table{
  int msize;               // общий размер таблицы
  int csize;               // активное количество элементов 
  struct Keyspace1 *ks1;   // указатель на первое пространство ключей
  struct Keyspace2 *ks2;   // указатель на второе пространство ключей
}Table;

//  General functions
FILE* file_interaction(Table* table);
void Add(Table* table, FILE* file);
void Find(Table* table, FILE* file);
void find_compkey(Table* table, FILE* file);
void find_element(Table* table, char* key1, char* key2, FILE* file);
void Delete(Table *table);
void delete_comkey(Table* table);
void delete_element(Table* table, char* key1, char* key2);
void Special_Operations(Table* table, FILE* file);
void Show(Table *table, FILE* file);
int show_search(Table* table, int a);
int check_unique_keys(Table* table, char* key1, char* key2);
int dialog();
char *readline();
int getInt(int *a);
void clear_table(Table* table);
int chfull_table(Table* table);
void save_table_to_file(Table* table, FILE* file);
void set_table_default(Table* table);

//   Open Address Table
int  oat_hash1(char* in, int size);
int  oat_hash2(char* in, int size);
int  oat_double_hashing(char* in, int* i, int size);
int oat_search_delete(Table *table, char* in, int* i);
void oat_insert(Table *table, char* info, char* key, int info_size, int offset);
int oat_search_insert(Table *table, char* in);
int oat_count_release(Table *table, char* in);
void oat_delmany_elems(Table *table);
void oat_find_elems(Table *table, FILE* file);
void oat_search_elems(Table *table, char* in, Item* pitem, FILE* file);
void oat_copy_elems(Table *table, FILE* file);

//   Look Up Table
void lpt_reorganize(Table* table, FILE* file);
void lpt_delmany_elems(Table *table);
int lpt_search_delete(Table *table, char* in);
int lpt_insert(Table *table, char* info, char* key, int info_size, int offset);
int lpt_search_delete_comkey(Table *table, char* key1, char* key2);
int lpt_count_release(Table *table, const char* in);
void lpt_find_elems(Table *table, FILE* file);
void lpt_search_elems(Table *table, char* in, Item* pitem, FILE* file);
