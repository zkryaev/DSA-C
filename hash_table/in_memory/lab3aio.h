#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct Item{
  char* info;        // информация лежащая в элементе
  struct Keyspace1 *p1;     // связь с элементом 2го пространства ключей по указателю
  struct Keyspace2 *p2;     // связь с элементом 2го пространства ключей по указателю
}Item;

typedef struct Keyspace1{
  int busy;                    // признак занятости элемента
  char* key;                   // ключ эемента
  int release;                 // номер версии элемента
  struct Item *element;        // указатель на информацию
}Keyspace1;

typedef struct Keyspace2{
  int busy;   
  char* key;       
  int release;   
  struct Item *element;
}Keyspace2;

typedef struct{
  struct Keyspace1 *ks1;   // указатель на первое пространство ключей
  struct Keyspace2 *ks2;   // указатель на второе пространство ключей
  int msize;               // размер таблицы
  int csize1;              // активное количество элементов в Look Up Table
}Table;

// OAT - Open Address Table, LPT - Look Up Table.
//  General functions
int dialog();
char* readline();
int  getInt(int *a);
void Add(Table *table);
void Find(Table* table);
void Delete(Table *table);
void Show(Table *table);
void Special_Operations(Table* table);
void set_busy(Table* table);
void clear_table(Table* table);
int  chfull_table(Table* table);
void delete_comkey(Table* table); // удаление составным ключем
void find_compkey(Table* table);
void delete_element(Table* table, char* key1, char* key2); // удаление заданным ключем
void find_element(Table* table, char* key1, char* key2);
int  check_unique_keys(Table* table, char* key1, char* key2);

//   Open Address Table
int  oat_hash1(char* in, int size);
int  oat_hash2(char* in, int size);
int  oat_double_hashing(char* in, int* i, int size);
int  oat_insert(Table *table, Item* elem, char* key2);      // Вставка во 2е пространство
int  oat_search_insert(Table *table, char* in); // поиск для своб.места |
void oat_delmany_elems(Table *table);                   // удаление всех элементов заданным 2м ключем
int  oat_search_delete(Table *table, char* in, int* i); // поиск для него |
void oat_find_elems(Table *table);                          // нахождение всех элементов заданным ключем 2го пространства и их копир-е
void oat_search_elems(Table *table, char* in, Item* pitem); // поиск для него |
void oat_copy_elems(Table *table);                   // Копирование таблицы (индив. задание)
int  oat_count_release(Table *table, char* in);

//   Look Up Table
int  lpt_insert(Table *table, Item* elem, char* key1);
void lpt_delmany_elems(Table *table);
int  lpt_search_delete(Table *table, char* in);
void lpt_find_elems(Table *table);
void lpt_search_elems(Table *table, char* in, Item* pitem);
void lpt_reorganize(Table* table);
int  lpt_count_release(Table *table, const char* in);
int  lpt_search_delete_comkey(Table *table, char* key1, char* key2);