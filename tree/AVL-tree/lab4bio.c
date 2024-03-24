#include "lab4bio.h"

//--------------------------------------------------------------------------- ДОБАВЛЕНИЕ
void Add(Node** root)
{
    int a;
    printf("    1) -Add- element by key\n    2) Press any others to Quit\n");
    int key;
    char* info = NULL;
    getInt(&a);
    switch(a)
    {
        case 1: 
            printf("[ Enter the key: ]\n"); 
            getInt( &(key) ); 
            printf("[ Enter the info: ]\n");
            while(getchar() != '\n'); 
            info = readline(); 
            insert(root, key, info, 0); 
            info = NULL; 
            break;
        default: printf("[ Leaving... ]\n");
    }
}

void insert(Node** root, unsigned int key, char* info, int offset)
{
    Node* newNode = createNode(key, info, offset);
    if( (*root) == NULL ){
        *root = newNode;
    }else{
        Node* scan = *root;
        Node* prev = NULL;
        while( scan != NULL )
        {
            prev = scan;
            if(newNode->key <= scan->key){
                if(newNode->key == scan->key)
                    newNode->version++;
                scan = scan->left;
            }else{
                scan = scan->right;
            }
        }
        if( newNode->key <= prev->key ){
            newNode->parent = prev;
            prev->left = newNode;
        }else{
            newNode->parent = prev;
            prev->right = newNode;
        }
        prev = NULL;
    }
    find_unbalance_node(root, newNode);
    newNode = NULL;
}

struct Node* createNode(unsigned int key, char* info, int offset)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->hleft = 1;
    newNode->hright = 1;
    newNode->version = 1;
    newNode->key = key;
    newNode->offset = offset;
    newNode->info = info;
    return newNode;
}

void find_unbalance_node(Node** root, Node* prev){
    while(prev != NULL){
        if(prev->left || prev->right){ // высчитываю hleft hright
            if(prev->left){
                if( prev->left->hleft == prev->left->hright )
                    prev->hleft = prev->left->hleft + 1;
                if( prev->left->hleft > prev->left->hright )
                    prev->hleft = prev->left->hleft + 1;
                if( prev->left->hleft < prev->left->hright )
                    prev->hleft = prev->left->hright + 1; 
            }else
                prev->hleft = 1;
            if(prev->right){
                if( prev->right->hleft == prev->right->hright )
                    prev->hright = prev->right->hleft + 1;
                if( prev->right->hleft > prev->right->hright )
                    prev->hright = prev->right->hleft + 1;
                if( prev->right->hleft < prev->right->hright )
                    prev->hright = prev->right->hright + 1;
            }else
                prev->hright = 1;
        }
        if( abs(prev->hleft - prev->hright) > 1 ){ // если нашел дисбаланс то балансирую
            balancing_tree(root, prev);
        }
        prev = prev->parent; //перехожу по ветке выше
    }
}

void balancing_tree(Node** root, Node* unbalanceNode){
    if( (unbalanceNode->hleft - unbalanceNode->hright > 1) ){ // ( 3 <-|6 ) <-|10       ( 6|-> 8 ) <-|10
        Node* tmp = unbalanceNode->left;
        if( (tmp->hleft > tmp->hright) ){
            right_rotation(root, unbalanceNode);
        }
        if( (tmp->hleft < tmp->hright) ){
            left_right_rotation(root, unbalanceNode);
        }
        tmp = NULL;
    }
    if( (unbalanceNode->hleft - unbalanceNode->hright < -1) ){ // 10|-> ( 12|-> 13 )       10|-> ( 11 <- |12 ) 
        Node* tmp = unbalanceNode->right;
        if( (tmp->hleft < tmp->hright) ){
            left_rotation(root, unbalanceNode);
        }
        if( (tmp->hleft > tmp->hright) ){
            right_left_rotation(root, unbalanceNode);
        }
        tmp = NULL;
    }
}

void right_rotation(Node** root, Node* unbalanceNode){
    if(unbalanceNode != (*root) ){ // если unbalance node не корень дерева
        Node* tmp = unbalanceNode->left;
        tmp->parent = unbalanceNode->parent;
        if(  ((tmp->parent->hleft - tmp->parent->hleft) > 1) || (tmp->parent->left == unbalanceNode) ){
            tmp->parent->left = tmp;
        }else{
            if( ((tmp->parent->hleft - tmp->parent->hleft) < -1) || (tmp->parent->right == unbalanceNode) )
                tmp->parent->right = tmp;
        }
        unbalanceNode->left = tmp->right;
        if(unbalanceNode->left)
            unbalanceNode->left->parent = unbalanceNode;
        tmp->right = unbalanceNode;
        unbalanceNode->parent = tmp;
        if(unbalanceNode->left){
            if( unbalanceNode->left->hleft == unbalanceNode->left->hright )
                unbalanceNode->hleft = unbalanceNode->left->hleft + 1;
            if( unbalanceNode->left->hleft > unbalanceNode->left->hright )
                unbalanceNode->hleft = unbalanceNode->left->hleft + 1;
            if( unbalanceNode->left->hleft < unbalanceNode->left->hright )
                unbalanceNode->hleft = unbalanceNode->left->hright + 1;
        }else
            unbalanceNode->hleft = 1;
        //tmp->hright:
        if( unbalanceNode->hright == unbalanceNode->hleft )
            tmp->hright = unbalanceNode->hright + 1;
        if( unbalanceNode->hright > unbalanceNode->hleft )
            tmp->hright = unbalanceNode->hright + 1;
        if( unbalanceNode->hright < unbalanceNode->hleft )
            tmp->hright = unbalanceNode->hleft + 1;
        tmp = NULL;
    }else{
        Node* tmp = unbalanceNode->left;
        tmp->parent = unbalanceNode->parent;
        unbalanceNode->left = tmp->right;
        if(unbalanceNode->left)
            unbalanceNode->left->parent = unbalanceNode;
        tmp->right = unbalanceNode;
        unbalanceNode->parent = tmp;
        if(unbalanceNode->left){
            if( unbalanceNode->left->hleft == unbalanceNode->left->hright )
                unbalanceNode->hleft = unbalanceNode->left->hleft + 1;
            if( unbalanceNode->left->hleft > unbalanceNode->left->hright )
                unbalanceNode->hleft = unbalanceNode->left->hleft + 1;
            if( unbalanceNode->left->hleft < unbalanceNode->left->hright )
                unbalanceNode->hleft = unbalanceNode->left->hright + 1;
        }else
            unbalanceNode->hleft = 1;
        //tmp->hright:
        if( unbalanceNode->hright == unbalanceNode->hleft )
            tmp->hright = unbalanceNode->hright + 1;
        if( unbalanceNode->hright > unbalanceNode->hleft )
            tmp->hright = unbalanceNode->hright + 1;
        if( unbalanceNode->hright < unbalanceNode->hleft )
            tmp->hright = unbalanceNode->hleft + 1;
        *root = tmp;
        tmp = NULL;
    }
}

void left_rotation(Node** root, Node* unbalanceNode){
    if(unbalanceNode != *root){ // если unbalance node не корень дерева
        Node* tmp = unbalanceNode->right;
        tmp->parent = unbalanceNode->parent;
        if( ((tmp->parent->hleft - tmp->parent->hleft) > 1) || (tmp->parent->left == unbalanceNode) ){
            tmp->parent->left = tmp;
        }else{
            if( ((tmp->parent->hleft - tmp->parent->hleft) < -1) || (tmp->parent->right == unbalanceNode) )
            tmp->parent->right = tmp;
        }
        unbalanceNode->right = tmp->left;
        if(unbalanceNode->right)
            unbalanceNode->right->parent = unbalanceNode;
        tmp->left = unbalanceNode;
        unbalanceNode->parent = tmp;
        if(unbalanceNode->right){
            if( unbalanceNode->right->hleft == unbalanceNode->right->hright )
                unbalanceNode->hright = unbalanceNode->right->hleft + 1;
            if( unbalanceNode->right->hleft > unbalanceNode->right->hright )
                unbalanceNode->hright = unbalanceNode->right->hleft + 1;
            if( unbalanceNode->right->hleft < unbalanceNode->right->hright )
                unbalanceNode->hright = unbalanceNode->right->hright + 1;
        }else
            unbalanceNode->hright = 1;
        //tmp->hleft:
        if( (unbalanceNode->hright == unbalanceNode->hleft) )
            tmp->hleft = unbalanceNode->hright + 1;
        if( (unbalanceNode->hright > unbalanceNode->hleft) )
            tmp->hleft = unbalanceNode->hright + 1;
        if( (unbalanceNode->hright < unbalanceNode->hleft) )
            tmp->hleft = unbalanceNode->hleft + 1;
        tmp = NULL;
    }else{
        Node* tmp = unbalanceNode->right;
        tmp->parent = unbalanceNode->parent;
        unbalanceNode->right = tmp->left;
        if(unbalanceNode->right)
            unbalanceNode->right->parent = unbalanceNode;
        tmp->left = unbalanceNode;
        unbalanceNode->parent = tmp;
        if(unbalanceNode->right){
            if( unbalanceNode->right->hleft == unbalanceNode->right->hright )
                unbalanceNode->hright = unbalanceNode->right->hleft + 1;
            if( unbalanceNode->right->hleft > unbalanceNode->right->hright )
                unbalanceNode->hright = unbalanceNode->right->hleft + 1;
            if( unbalanceNode->right->hleft < unbalanceNode->right->hright )
                unbalanceNode->hright = unbalanceNode->right->hright + 1;
        }else
            unbalanceNode->hright = 1;
        //tmp->hleft:
        if( (unbalanceNode->hright == unbalanceNode->hleft) )
            tmp->hleft = unbalanceNode->hright + 1;
        if( (unbalanceNode->hright > unbalanceNode->hleft) )
            tmp->hleft = unbalanceNode->hright + 1;
        if( (unbalanceNode->hright < unbalanceNode->hleft) )
            tmp->hleft = unbalanceNode->hleft + 1;
        *root = tmp;
        tmp = NULL;
    }
}

void right_left_rotation(Node** root, Node* unbalanceNode){
    Node* tmp = unbalanceNode->right;
    right_rotation(root, tmp);
    left_rotation(root, unbalanceNode);
}

void left_right_rotation(Node** root, Node* unbalanceNode){
    Node* tmp = unbalanceNode->left;
    left_rotation(root, tmp);
    right_rotation(root, unbalanceNode);
}

//--------------------------------------------------------------------------- ПОИСК
void Find(Node* root)
{
    if(root){
        int a, key;
        printf("    1) -Find- element by key\n    2) -Differ Finding- by key\n    3)Press any others to Quit\n");
        getInt(&a);
        switch(a)
        {
            case 1: 
                printf("[ Enter the key: ]\n"); 
                getInt( &(key) ); 
                find_element(root, key, 0); 
                break;
            case 2:
                printf("[ Enter the key: ]\n"); 
                getInt( &(key) ); 
                find_differ(root, key);
                break;
            default: printf("[ Leaving... ]\n");
        }
    }else
        printf("[ The tree is empty! ]\n");
}

int find_element(Node* root, int key, short int t){
    Node* scan = root;
    Node** keys_set = NULL;
    int ind = 0;
    while(scan != NULL){
        if(key <= scan->key){
            if(key == scan->key){
                keys_set = (Node**)realloc( keys_set, (1 + ind)*sizeof(Node*) );
                keys_set[ind] = scan;
                ind++;
            }
            scan = scan->left;
        }else{
            scan = scan->right;
        }
    }
    if(keys_set != NULL){
        if(t == 0){
            printf("The -finding- key and his info:\n");
            for(int i = 0; i < ind; i++){
                printf("[ key: %d | info: %s | offset: %d ]\n", keys_set[i]->key, keys_set[i]->info, keys_set[i]->offset);
            }
        }
    return 1;
    }else{
        if(t == 0)
            printf("This key doesn't exist\n");
        return 0;
    }
    free(keys_set);
    keys_set = NULL;
}

void find_differ(Node* root, int key){
    Node* scan = root;
    Node** keys_set = NULL;
    int ind = 0, diffkey = 0;
    order_trav_differ(root, key, &diffkey, 0);
    while(scan != NULL){
        if(diffkey <= scan->key){
            if(diffkey == scan->key){
                keys_set = (Node**)realloc( keys_set, (1 + ind)*sizeof(Node*) );
                keys_set[ind] = scan;
                ind++;
            }
            scan = scan->left;
        }else
            scan = scan->right;
    }
    if(keys_set != NULL){
        printf("The -finding- key and his info:\n");
        for(int i = 0; i < ind; i++){
            printf("[ key: %d | info: %s ]\n", keys_set[i]->key, keys_set[i]->info);
        }
    }else
        printf("The -finding- key is doesn't exist!\n");
}

void order_trav_differ(Node* root, int key, int* diffkey, int difference){
    if(root){
        if( (difference) < abs(root->key - key) ){
            difference = abs(root->key - key);
            *diffkey = root->key;
        }
        order_trav_differ(root->left, key, diffkey, difference);
        order_trav_differ(root->right, key, diffkey, difference);
    }
}
//--------------------------------------------------------------------------- УДАЛЕНИЕ

void Delete(Node** root)
{
    if(*root){
        int a, key;
        printf("    1) -Delete- element by key\n    2) Press any others to Quit\n");
        getInt(&a);
        switch(a)
        {
            case 1: 
                printf("[ Enter the key: ]\n"); 
                getInt( &(key) ); 
                delete_element(root, key); 
                break;
            default: printf("[ Leaving... ]\n");
        }
    }else
        printf("[ The tree is empty! ]\n");
}
int delete_element(Node** root, int key){
    Node* scan = *root;
    Node* prev = NULL;
    int detect = 0;
    while(scan != NULL){
        if(key <= scan->key){
            if(key == scan->key){ // нашли ключ
                prev = scan;
                if( (prev->left == NULL) && (prev->right == NULL) ){ // Лепесток
                    detect++;
                    if( prev->parent ){
                        if(prev->parent->left == prev)
                            prev->parent->left = NULL;
                        if(prev->parent->right == prev)
                            prev->parent->right = NULL;
                        if(prev->info)
                            free(prev->info);
                        prev->info = NULL;
                        find_unbalance_node(root, prev->parent);
                        prev->parent = NULL;
                        scan = scan->left;
                        free(prev);
                        prev = NULL;
                        continue;
                    }else{
                        scan = scan->left;
                        if(prev->info)
                            free(prev->info);
                        prev->info = NULL;
                        free(prev);
                        prev = NULL;
                        *root = NULL;
                        continue;
                    }
                }
                if( ( (prev->left != NULL) && (prev->right == NULL) ) || ( (prev->left == NULL) && (prev->right != NULL) ) ){ // Узел
                    detect++;
                    if( prev->parent ){
                        if(prev->left != NULL){
                            prev->left->parent = prev->parent;
                            if(prev->parent->left == prev)
                                prev->parent->left = prev->left;
                            if(prev->parent->right == prev)
                                prev->parent->right = prev->left;
                            scan = scan->left;
                            prev->parent = NULL;
                            if(prev->info)
                                free(prev->info);
                            prev->info = NULL;
                            find_unbalance_node(root, prev->left);
                            prev->left = NULL;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                        if(prev->right != NULL){
                            prev->right->parent = prev->parent;
                            if(prev->parent->left == prev)
                                prev->parent->left = prev->right;
                            if(prev->parent->right == prev)
                                prev->parent->right = prev->right;
                            scan = scan->left;
                            prev->parent = NULL;
                            if(prev->info)
                                free(prev->info);
                            prev->info = NULL;
                            find_unbalance_node(root, prev->right);
                            prev->right = NULL;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                    }else{
                        if(prev->left != NULL){
                            prev->left->parent = NULL;
                            scan = scan->left;
                            if(prev->info)
                                free(prev->info);
                            prev->info = NULL;
                            find_unbalance_node(root, prev->left);
                            prev->left = NULL;
                            *root = scan;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                        if(prev->right != NULL){
                            prev->right->parent = NULL;
                            scan = scan->right;
                            if(prev->info)
                                free(prev->info);
                            prev->info = NULL;
                            find_unbalance_node(root, prev->right);
                            prev->right = NULL;
                            *root = scan;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                    }
                }
                if( (prev->left != NULL) && (prev->right != NULL)){ // Корень
                    detect++;
                    if( prev->parent ){
                        Node* tmp = prev->left;
                        tmp->parent = prev->parent;
                        if( tmp->parent->left == prev){
                            tmp->parent->left = tmp;
                        }else
                            tmp->parent->right = tmp;
                        tmp->hleft = prev->hleft;
                        tmp->hright = prev->hright;
                        tmp->right = prev->right;
                        tmp->right->parent = tmp;
                        prev->parent = NULL;
                        prev->left = NULL;
                        prev->right = NULL;
                        if(prev->info)
                            free(prev->info);
                        prev->info = NULL;
                        free(prev);
                        prev = NULL;
                        find_unbalance_node(root, tmp);
                        tmp = NULL;
                    }else{
                        Node* tmp = prev->left;
                        tmp->parent = prev->parent;
                        tmp->right = prev->right;
                        tmp->right->parent = tmp;
                        tmp->hleft = prev->hleft;
                        tmp->hright = prev->hright;
                        prev->parent = NULL;
                        prev->left = NULL;
                        prev->right = NULL;
                        if(prev->info)
                            free(prev->info);
                        prev->info = NULL;
                        free(prev);
                        prev = NULL;
                        (*root) = tmp;
                        find_unbalance_node(root, tmp);
                        tmp = NULL;
                    }
                }
            }
            scan = scan->left;
        }else{
            scan = scan->right;
        }
    }
    //корректировка
    if(detect){
        return 1;
    }else
        return 0;
}

//--------------------------------------------------------------------------- ОБХОД
void Show(Node* root)
{
    if(root){
        int level_digit = 0;
        printf("Enter the level of digit:\n");
        getInt(&level_digit);
        show_traversall(root, level_digit);
    }else{
        printf("Tree is empty!\n");
    }
}

void show_traversall(Node* root, unsigned int level_digit){
    if(root){
        if( level_digit == lvdigcheck(root->key) )
                printf("key: %d | info: %s\n", root->key, root->info);
            show_traversall(root->left, level_digit);
            show_traversall(root->right, level_digit);
    }
}

int lvdigcheck(unsigned int digit)
{
    int level_digit = 0;
    if( digit/10 == 0 ){
        return 1;
    }else{
        while(digit > 0){
            digit = digit / 10;
            level_digit++;
        }
        return level_digit;
    }
}

//--------------------------------------------------------------------------- СПЕЦИАЛЬНОЕ
void Special_Operations(Node** root)
{
    int a;
    printf("    1) Formatted tree output\n    2) Loading a tree from a text file\n    3) Press any others to Quit\n");
    getInt(&a);
    switch(a)
    {
        case 1: formatted_show(*root); getchar(); break;
        case 2: loadtree_fromtxt(root); getchar(); break;
        default: printf("[Leaving...]\n");
    }
}

//--------------------------------------------------------------------
void loadtree_fromtxt(Node** root){
    printf("Enter the name of file:\n");
    while(getchar() != '\n');
    char* fname = readline();
    FILE* file = fopen(fname, "r");
    free(fname);
    int detect = 0;
    if(file != NULL){
        int key = 0;
        while(!feof(file)){
            char* info = malloc(sizeof(char*));
            fscanf(file, "%d", &key);
            fscanf(file, "%s", info);
            insert(root, key, info, 0);
            info = NULL;
            detect++;
        }
        if(detect){
            printf("  [ Success ]\n");
        }else
            printf("  [ File is empty! ]\n");
        fclose(file);
    }else{
        printf("[ This file was't exist! ]\n");
    }
}

//--------------------------------------------------------------------
void formatted_show(Node* root)
{
    if(root){
        int indent = 0;
        order_traversall(root, indent);
    }else
        printf(" [Tree is empty!] \n");
}

void order_traversall(Node* ptr, int indent){
    if(ptr->right)
        order_traversall(ptr->right, indent + 1);
    for(int i = 0; i < indent; i++)
        printf("    ");
    printf("%-d hl:%d hr:%d\n", ptr->key, ptr->hleft, ptr->hright);
    if(ptr->left)
        order_traversall(ptr->left, indent + 1);

}
//---------------------------------------------------------------------------
// Дополнительные задачи
//--------------------------------------------------------------------------- ГРАФИЗ *
void graphiz_option(Node* root){
    if(root){
        printf("Enter the name of file: \n");
        while(getchar() != '\n');
        char* fname = readline();
        FILE* file = fopen(fname, "w");
        if(file){
            fprintf(file, "digraph Tree{\n");
            fprintf(file, "label = \"Полученное дерево:\";\n");
            order_traversall_graphiz(root, file);
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
        printf(" The tree is empty!\n");
}

void order_traversall_graphiz(Node* ptr, FILE* file){
    if(ptr != NULL){
        if(ptr->left)
            fprintf(file,"    \"%d.%d\" -> \"%d.%d\" [color = blue] [label=\"L\", fontsize=9];\n", ptr->key, ptr->version, ptr->left->key, ptr->left->version);
        if(ptr->right)
            fprintf(file,"    \"%d.%d\" -> \"%d.%d\" [color = crimson] [label=\"R\", fontsize=9];\n", ptr->key, ptr->version, ptr->right->key, ptr->right->version);
        if(ptr->parent)
            fprintf(file,"    \"%d.%d\" -> \"%d.%d\" [label=\"P\", fontsize=9];\n", ptr->key, ptr->version, ptr->parent->key, ptr->parent->version);
        order_traversall_graphiz(ptr->left, file);
        order_traversall_graphiz(ptr->right, file);
    }
}
//--------------------------------------------------------------------------- БИНАРНИК **
void savetree_tobin(Node* root){
    char* fname = NULL;
    FILE* file = NULL;
    printf("Enter the name of BIN file\n");
    while(getchar() != '\n');
    fname = readline();
    file = fopen(fname, "wb");
    if(file){
        fseek(file, 0, SEEK_SET);
        savebin_traversall(root, file);
        printf("[ Success ]\n");
    }else{
        printf("Something goes wrong ;( \n");
    }
    free(fname);
    fclose(file);
}

void savebin_traversall(Node* root, FILE* file){
    if(root){
        fwrite(&(root->key), sizeof(unsigned int), 1, file);
        savebin_traversall(root->left, file);
        savebin_traversall(root->right, file);
    }
}

void readbin_fromfile(Node** root){
    char* fname = NULL;
    FILE* file = NULL;
    printf("Enter the name of BIN file\n");
    while(getchar() != '\n');
    fname = readline();
    file = fopen(fname, "rb");
    if(file){
        fseek(file, 0, SEEK_SET);
        unsigned int key;
        while( fread(&(key), sizeof(unsigned int), 1, file) ){
            insert(root, key, NULL, ( ftell(file) - sizeof(unsigned int) ) );
        }
        printf("[ Success ]\n");
    }else{
        printf("File with this name doesn't exist!\n");
    }
    free(fname);
    fclose(file);
}

void BIN_menu(Node** root){
    int a;
    printf("    1) Save tree to BIN file\n    2) Loading a tree from a BIN file\n    3) Press any others to Quit\n");
    getInt(&a);
    switch(a)
    {
        case 1: savetree_tobin(*root); getchar(); break;
        case 2: readbin_fromfile(root); getchar(); break;
        default: printf("[Leaving...]\n");
    }
}
//---------------------------------------------------------------------------ОБЩЕЕ

int dialog(){
	int rc = 0;
	int n = 0;
	do{
		if(rc < 0 || rc >= 9)
            printf("You are wrong. Repeate, please!\n");
        printf("0. Quit\n1. Add\n2. Find\n3. Delete\n4. Show\n5. Special Operations\n6. Timing\n-------------->\n7. Save to PNG\n8. BIN task\n");
		printf("Make your choice: -->\n");
		n = getInt(&rc); // ввод номера пункта
		if(n == 0) // конец ввода
		rc = 0;
	} while(rc < 0 || rc >= 9);
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

void clear_tree(Node* root){
    if(root != NULL){
        clear_tree(root->left);
        clear_tree(root->right);
        if(root->info)
            free(root->info);
        root->info = NULL;
        free(root);
    }
}

void Timing(){
    Node* root = NULL;
    int n = 10, key[10000], k, cnt = 1000000, m;
    clock_t first, last;
    srand(time(NULL));
    printf(" [ время -ПОИСКА- элементов ] \n");
    while (n-- > 0){ // поиск
        for (int i = 0; i < 10000; ++i)
            key[i] = abs( rand() * rand() );
        for (int i = 0; i < cnt; ++i){
            k = rand() * rand();
            insert(&root, k, NULL, 0);
        }

        m = 0;
        first = clock();
        for (int i = 0; i < 10000; ++i)
            if(find_element(root, key[i], 1))
                ++m;
        last = clock();
        printf("%d Элементов найдено:\n", m);
        printf("Тест #%d, Общее элементов = %d, Время = %.4lf сек.\n", 10 - n, (10 - n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
        printf("------------------------------поиск--------------------------------\n");
    }
    clear_tree(root);
    n = 10;
    printf(" [ время -УДАЛЕНИЯ- элементов ] \n");
    while (n-- > 0){ // удаление
        for (int i = 0; i < 10000; ++i)
            key[i] = rand() * rand();
        for (int i = 0; i < cnt; ++i){
            k = rand() * rand();
            k = abs(k);
            insert(&root, k, NULL, 0);
        }
        m = 0;
        first = clock();
        for (int i = 0; i < 10000; ++i)
            if(delete_element(&root, key[i]))
                ++m;
        last = clock();
        printf("%d Элементов удалено:\n", m);
        printf("Тест #%d, Общее элементов = %d, Время = %.4lf сек.\n", 10 - n, (10 - n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
        printf("-----------------------------удаление-----------------------------\n");
    }
    clear_tree(root);
    n = 10;
    printf(" [ время -ВСТАВКИ- 1000 элементов ] \n");
    while (n-- > 0){ // удаление
        for (int i = 0; i < cnt; ++i){
            k = rand() * rand();
            k = abs(k);
            insert(&root, k, "test", 0);
        }
        first = clock();
        for (int i = 0; i < 1000; ++i){
            k = rand() * rand();
            k = abs(k);
            insert(&root, k, NULL, 0);
        }
        last = clock();
        printf("%d Элементов вставлено:\n", 1000);
        printf("Тест #%d, Общее элементов = %d, Время = %.4lf сек.\n", 10 - n, (10 - n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
        printf("-----------------------------вставка-----------------------------\n");
    }
    clear_tree(root);
    root = NULL;
}