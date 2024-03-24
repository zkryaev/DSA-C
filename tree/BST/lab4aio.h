#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

typedef struct Element{ // структура для форматированного вывода
    int key; // ключ
    int level; // позиция по глубине
    int indent; // отсуп который будет выставляться при печати
}Element;

typedef struct Node{
    unsigned int key;
    unsigned int info;
    // указатели: родитель, лев.дерев, прав.дерев, нить
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    struct Node* ropeL;
    struct Node* ropeR;
}Node;

int  dialog();
int  getInt(int *a);
void clear_tree(Node* root);
char* readline();

void Add(Node** root, int* count_keys);
struct Node* createNode();
void insert(Node** root, int key, int info);

void Find(Node* root);
void keysearch(Node* root);
void minkeysearch(Node* root);

void Delete(Node** root, int* count_keys);
void delete_element(Node** root, int* count_keys);
void Show(Node* root);
void inOrderTraversall_recursion(Node* ptr);
void inOrderTraversall_iteration(Node* ptr);

void Special_Operations(Node** root);
void formatted_show(Node* root);
void order_traversall(Node* ptr, int indent);
int  leaves_of_input_tree(Node* ptr);
void loadtree_fromtxt(Node** root);

void graphiz_option(Node* root);
void order_traversall_graphiz(Node* ptr, FILE* file);
void binary_option(Node** root, int* count_keys);
void Timing();
int  find(Node* root, int key);
int delete_timing(Node** root, int key);