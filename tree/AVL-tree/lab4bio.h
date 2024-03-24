#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

typedef struct Node{
    int hleft, hright;
    unsigned int key;
    int offset;
    short int version;
    char* info;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
}Node;

int  dialog();
int  getInt(int *a);
char* readline();
void clear_tree(Node* root);

void formatted_show(Node* root);
void order_traversall(Node* ptr, int indent);

void Add(Node** root);
void insert(Node** root, unsigned int key, char* info, int offset);
struct Node* createNode(unsigned int key, char* info, int offset);
void find_unbalance_node(Node** root, Node* prev);
void balancing_tree(Node** root, Node* unbalanceNode);
void right_rotation(Node** root, Node* unbalanceNode);
void left_rotation(Node** root, Node* unbalanceNode);
void right_left_rotation(Node** root, Node* unbalanceNode);
void left_right_rotation(Node** root, Node* unbalanceNode);

void Delete(Node** root);
int delete_element(Node** root, int key);

void Find(Node* root);
int find_element(Node* root, int key, short int t);
void find_differ(Node* root, int key);
void order_trav_differ(Node* root, int key, int* diffkey, int difference);

void Show(Node* root);
void show_traversall(Node* root, unsigned int level_digit);
int lvdigcheck(unsigned int digit);

void Special_Operations(Node** root);

void loadtree_fromtxt(Node** root);
void graphiz_option(Node* root);
void order_traversall_graphiz(Node* ptr, FILE* file);

void BIN_menu(Node** root);
void readbin_fromfile(Node** root);
void savetree_tobin(Node* root);
void savebin_traversall(Node* root, FILE* file);
