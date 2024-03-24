#include "lab4aio.h"
#include "lab4bin.h"

//--------------------------------------------------------------------------- ДОБАВЛЕНИЕ
void Add(Node** root, int* count_keys)
{
    (*count_keys) = (*count_keys) + 1;
    Node* new = createNode();
    if((*root) == NULL){
        *root = new;
        new = NULL;
    }else{
        Node* scan = (*root);
        Node* prev = NULL;
        while(scan != NULL){
            prev = scan;
            if(new->key <= scan->key){
                scan = scan->left;
                if(new->key == prev->key){
                    new->parent = prev;
                    new->left = prev->left;
                    if( ((prev->parent != NULL) && (new->left == NULL)) && (prev->parent->right == prev) ){ // случай когда добавляем правому родителю левый элемент.
                        new->ropeL = prev->parent;
                        prev->parent->ropeR = new;
                    }else{
                        if( ((new->key == prev->key) && (new->left == NULL)) && (prev->ropeL != NULL) ){ 
                            new->ropeL = prev->ropeL;
                            prev->ropeL->ropeR = new;
                            prev->ropeL = NULL;
                        }
                    }
                    if(prev->left != NULL)
                        prev->left->parent = new;
                    prev->left = new;
                    scan = NULL;
                    new = NULL;
                }
            }else
                scan = scan->right;
        }
        if(new != NULL){
            if(new->key < prev->key){
                prev->left = new;
                new->parent = prev;
                if( (prev->parent != NULL) && (prev->parent->right == prev) ){ // случай когда добавляем правому родителю левый элемент.
                    new->ropeL = prev->parent;
                    prev->parent->ropeR = new;
                }else{ 
                    if(prev->ropeL != NULL){        // если родитель не правый то проверяем на наличие указателя rope у prev
                        new->ropeL = prev->ropeL;
                        prev->ropeL->ropeR = new;
                        prev->ropeL = NULL;
                    }
                }
                new = NULL;
            }else{
                prev->right = new;
                new->parent = prev;
                if( (prev->parent != NULL) && (prev->parent->left == prev) ){ // случай когда добавляем левому родителю правый элемент.
                    new->ropeR = prev->parent;
                    prev->parent->ropeL = new;
                }else{
                    if(prev->ropeR != NULL){ // если родитель не левый то проверяем на наличие указателя rope у prev
                        new->ropeR = prev->ropeR;
                        prev->ropeR->ropeL = new;
                        prev->ropeR = NULL;
                    }
                }
                new = NULL;
            }
        }
        prev = NULL;
    }
    printf("[ Element was insert to tree! ]\n");
}

struct Node* createNode()
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    printf("[ Enter the key: ]\n");
    getInt(&(newNode->key));
    printf("[ Enter the info: ]\n");
    getInt(&(newNode->info));
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->ropeL = NULL;
    newNode->ropeR = NULL;
    return newNode;
}

//--------------------------------------------------------------------------- ПОИСК
void Find(Node* root)
{
    int a;
    printf("1) Finding element by key\n2) Finding the element with the smallest key value\n3) Press any others to Quit\n");
    getInt(&a);
    switch(a)
    {
        case 1: keysearch(root); getchar(); break;
        case 2: minkeysearch(root); getchar(); break;
        default: printf("[ Leaving... ]\n");
    }
}

void keysearch(Node* root) 
{
    if(root){
        int key;
        printf("[ Enter the key: ]\n"); 
        getInt(&key);
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
            printf("The -finding- key and his info:\n");
            for(int i = 0; i < ind; i++){
                printf("[ key: %d | info: %d ]\n", keys_set[i]->key, keys_set[i]->info);
            }
        }else
            printf("The -finding- key is doesn't exist!\n");
    }else
        printf("[ The tree is empty! ]\n");
}

void minkeysearch(Node* root)
{
    if(root){
        Node* scan = root;
        int ind = 0;
        while(scan->left != NULL){
            scan = scan->left;
        }
        Node** keys_set = (Node**)malloc(sizeof(Node*));
        keys_set[ind] = scan;
        while(scan->parent->key == scan->key){
            ind++;
            keys_set = (Node**)realloc( keys_set, (ind)*(sizeof(Node*)) );
            scan = scan->parent;
            keys_set[ind] = scan;
        }
        scan = NULL;
        printf("The -smallest- key and his info:\n");
        for(int i = 0; i <= ind; i++){
            printf("[ key: %d | info: %d ]\n", keys_set[i]->key, keys_set[i]->info);
        }
    }else
        printf("[ The tree is empty! ]\n");
}
//--------------------------------------------------------------------------- УДАЛЕНИЕ

void Delete(Node** root, int* count_keys)
{
    if(*root){
        int a;
        printf("1) Delete element by key\n2) Press any others to Quit\n");
        getInt(&a);
        switch(a)
        {
            case 1: delete_element(root, count_keys); break;
            default: printf("[ Leaving... ]\n");
        }   
    }else
        printf("[ The tree is empty! ]\n");
}

void delete_element(Node** root, int* count_keys){
    (*count_keys) = (*count_keys) - 1;
    int key;
    printf("[ Enter the element that you want delete ]\n");
    getInt(&key);
    Node* scan = *root;
    Node* prev = NULL;
    while(scan != NULL){
        if(key <= scan->key){
            prev = scan; 
            if(key == scan->key){ // нашли ключ
                if( (prev->left == NULL) && (prev->right == NULL) ){ // Лепесток
                    if(prev->parent != NULL){
                        // операции с нитями
                        if(prev->ropeL != NULL){
                            if(prev->ropeL->right == prev->parent){
                                prev->ropeL->ropeR = NULL;
                                prev->ropeL = NULL;
                            }else{
                                prev->parent->ropeL = prev->ropeL;
                                prev->ropeL->ropeR = prev->parent;
                                prev->ropeL = NULL;
                            }
                        }
                        if(prev->ropeR != NULL){
                            if(prev->ropeR->left == prev->parent){
                                prev->ropeR->ropeL = NULL;
                                prev->ropeR = NULL;
                            }else{
                                prev->parent->ropeR = prev->ropeR;
                                prev->ropeR->ropeL = prev->parent;
                                prev->ropeR = NULL;
                            }
                        }
                        //
                        if(prev->parent->left == prev)
                            prev->parent->left = NULL;
                        if(prev->parent->right == prev)
                            prev->parent->right = NULL;
                        prev->parent = NULL;
                        scan = scan->left;
                        free(prev);
                        continue;
                    }else{
                        scan = scan->left;
                        free(prev);
                        *root = NULL;
                        continue;
                    }
                }
                if( ( (prev->left != NULL) && (prev->right == NULL) ) || ( (prev->left == NULL) && (prev->right != NULL) ) ){ // Узел
                    if(prev->parent != NULL){
                        // нити
                        if( (prev->ropeL == NULL) && (prev->ropeR != NULL) ){ // Ропер
                                if( prev->parent->right == prev ){
                                    prev->ropeR->ropeL = prev->left;
                                    if(prev->left){
                                        prev->left->ropeR = prev->ropeR;
                                        if(prev->left->ropeL)
                                            prev->left->ropeL->ropeR = NULL;
                                        prev->left->ropeL = NULL;
                                    }
                                    prev->ropeR = NULL;
                                }
                                if(prev->parent->left == prev){
                                    if(prev->right->ropeR != NULL){
                                        prev->right->ropeR->ropeL = NULL;
                                        prev->right->ropeR = NULL;
                                    }
                                    prev->ropeR->ropeL = NULL;
                                    prev->ropeR = NULL;
                                }
                            }
                            if( (prev->ropeL != NULL) && (prev->ropeR == NULL) ){ // Ропель
                                if(prev->parent->left == prev){
                                    prev->ropeL->ropeR = prev->right;
                                    if(prev->right){
                                        prev->right->ropeL = prev->ropeL;
                                        if(prev->right->ropeR)
                                            prev->right->ropeR->ropeL = NULL;
                                        prev->right->ropeR = NULL;
                                    }
                                    prev->ropeL = NULL;
                                }
                                if(prev->parent->right == prev){
                                    if(prev->left->ropeL != NULL){
                                        prev->right->ropeL->ropeR = NULL;
                                        prev->right->ropeL = NULL;
                                    }
                                    prev->ropeL->ropeR = NULL;
                                    prev->ropeL = NULL;
                                }
                            }
                            if( (prev->ropeL != NULL) && (prev->ropeR != NULL) ){ // Сложный случай: Ропер и Ропель
                                if( prev->parent->right == prev ){
                                    prev->ropeL->ropeR = prev->ropeR;
                                    prev->ropeR->ropeL = prev->ropeL;
                                    prev->ropeR = NULL;
                                    prev->ropeL = NULL;
                                    if(prev->left->ropeL != NULL){
                                        prev->left->ropeL->ropeR = NULL;
                                        prev->left->ropeL = NULL;
                                    }
                                }
                                if(prev->parent->left == prev){
                                    prev->ropeR->ropeL = prev->ropeL;
                                    prev->ropeL->ropeR = prev->ropeR;
                                    prev->ropeL = NULL;
                                    prev->ropeR = NULL;
                                    if(prev->right->ropeL != NULL){
                                        if(prev->right->ropeR)
                                            prev->right->ropeR->ropeL = NULL;
                                        prev->right->ropeR = NULL;
                                    }
                                }
                            }
                        // удаление узла
                        if(prev->left != NULL){
                            prev->left->parent = prev->parent;
                            if(prev->parent->left == prev)
                                prev->parent->left = prev->left;
                            if(prev->parent->right == prev)
                                prev->parent->right = prev->left;
                            scan = scan->left;
                            prev->left = NULL;
                            prev->parent = NULL;
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
                            prev->right = NULL;
                            prev->parent = NULL;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                    }else{
                        if( (prev->ropeL == NULL) && (prev->ropeR != NULL) ){
                            prev->ropeR->ropeL = NULL;
                            prev->ropeR = NULL;
                        }
                        if( (prev->ropeL != NULL) && (prev->ropeR == NULL) ){
                            prev->ropeL->ropeR = NULL;
                            prev->ropeL = NULL;
                        }
                        if(prev->left != NULL){
                            prev->left->parent = NULL;
                            scan = scan->left;
                            prev->left = NULL;
                            *root = scan;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                        if(prev->right != NULL){
                            prev->right->parent = NULL;
                            scan = scan->right;
                            *root = scan;
                            prev->right = NULL;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                    }
                }
                if( (prev->left != NULL) && (prev->right != NULL)){ // Корень
                    if(prev->ropeR != NULL){
                        if(prev->ropeR->right != NULL){
                            if(prev->ropeR->right->ropeR)
                                prev->ropeR->right->ropeR->ropeL = NULL;
                            prev->ropeR->right->ropeR = NULL;
                            if(prev->ropeR->ropeR != NULL){
                                prev->ropeR->ropeR->ropeL = prev;
                                prev->ropeR->ropeR = NULL;
                            }
                            prev->key = prev->ropeR->key;
                            prev->info = prev->ropeR->info;
                            prev->ropeR->right->parent = prev->ropeR->parent;
                            prev->ropeR->parent->left = prev->ropeR->right;
                            prev->ropeR->right->parent = NULL;
                            Node* tmp = NULL;
                            if(prev->ropeR->ropeR == NULL){
                                tmp = prev->ropeR;
                                prev->ropeR->right->ropeL = prev->ropeR->ropeL;
                                prev->ropeR = prev->ropeR->right;
                                tmp->right = NULL;
                                free(tmp);
                                tmp = NULL;
                            }else{
                                tmp = prev->ropeR;
                                prev->ropeR->right = NULL;
                                free(tmp);
                                tmp = NULL;
                            }
                        }else{
                            prev->key = prev->ropeR->key;
                            prev->info = prev->ropeR->info;
                            prev->ropeR->ropeL = NULL;
                            Node* tmp = prev->ropeR;
                            prev->ropeR = NULL;
                            tmp->parent->left = NULL;
                            tmp->parent = NULL;
                            free(tmp);
                        }
                    }else{
                        prev->key = prev->right->key;
                        prev->info = prev->right->info;
                        if(prev->right->right != NULL){
                            Node* tmp = prev->right;
                            if(prev->right->ropeR)
                                prev->right->ropeR->ropeL = prev;
                            prev->ropeR = prev->right->ropeR;
                            prev->right->ropeR = NULL;
                            prev->right->right->parent = prev;
                            prev->right->parent = NULL;
                            prev->right = prev->right->right;
                            free(tmp);
                            tmp = NULL;
                        }else{
                            free(prev->right);
                            prev->right = NULL;
                        }
                    }
                }
            }
            scan = scan->left;
        }else{
            scan = scan->right;
        }
    }
}
//--------------------------------------------------------------------------- ОБХОД
void Show(Node* root)
{
    if(root){
        int a;
        printf("1) Recursion traversall\n2) Iteration traversall\n3) Press any others to Quit\n");
        getInt(&a);
        clock_t first, last;
        srand(time(NULL));
        switch(a)
        {
            case 1: printf("[      -Recursion- traversall      ]\n"); first = clock(); inOrderTraversall_recursion(root); last = clock(); printf("[      ----------------------      ]\n"); printf("-Trav.time- = %lf сек.\n", (double)(last - first)/CLOCKS_PER_SEC ); getchar(); break;
            case 2: printf("[      -Iteration- traversall      ]\n"); first = clock(); inOrderTraversall_iteration(root); last = clock(); printf("[      ----------------------      ]\n"); printf("-Trav.time- = %lf сек.\n", (double)(last - first)/CLOCKS_PER_SEC ); getchar(); break;
            default: printf("[ Leave... ]\n");
        }
    }else
        printf("[ The tree is empty! ]\n");
}

void inOrderTraversall_iteration(Node* root){
    Node* ptr = root;
    int stop = 0;
    while(ptr->left)
        ptr = ptr->left;
    while( ptr && (stop <= ptr->key) ){
        // печать
        if(ptr->parent){
            printf("· key: %4d | info: %4d | parent: %4d ·\n", ptr->key, ptr->info, ptr->parent->key);
        }else{
            printf("· key: %4d | info: %4d | parent: ROOT ·\n", ptr->key, ptr->info);
        }
        //
        stop = ptr->key;
        if(ptr->ropeR){
            ptr = ptr->ropeR;
            continue;
        }
        if(ptr->right){
            ptr = ptr->right;
            continue;
        }
        if(ptr->parent)
            ptr = ptr->parent;
    }
    ptr = NULL;
        

    
}

void inOrderTraversall_recursion(Node* ptr)
{  
    if(ptr != NULL){
        inOrderTraversall_recursion(ptr->left);
        if(ptr->parent){
            printf("· key: %4d | info: %4d | parent: %4d ·\n", ptr->key, ptr->info, ptr->parent->key);
        }else{
            printf("· key: %4d | info: %4d | parent: ROOT ·\n", ptr->key, ptr->info);
        }
        inOrderTraversall_recursion(ptr->right);
    }
}
//--------------------------------------------------------------------------- СПЕЦИАЛЬНОЕ
void Special_Operations(Node** root)
{
    int a;
    printf("1) Formatted tree output\n2) Loading a tree from a text file\n3) Press any others to Quit\n");
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
        int key = 0, info = 0;
        while(!feof(file)){
            fscanf(file, "%d", &key);
            fscanf(file, "%d", &info);
            insert(root, key, info);
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

void insert(Node** root, int key, int info){
    Node* new = (Node*)malloc(sizeof(Node));
    new->key = key;
    new->info = info;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    new->ropeL = NULL;
    new->ropeR = NULL;
    if((*root) == NULL){
        *root = new;
        new = NULL;
    }else{
        Node* scan = (*root);
        Node* prev = NULL;
        while(scan != NULL){
            prev = scan;
            if(new->key <= scan->key){
                scan = scan->left;
                if(new->key == prev->key){
                    new->parent = prev;
                    new->left = prev->left;
                    if( ((prev->parent != NULL) && (new->left == NULL)) && (prev->parent->right == prev) ){ // случай когда добавляем правому родителю левый элемент.
                        new->ropeL = prev->parent;
                        prev->parent->ropeR = new;
                    }else{
                        if( ((new->key == prev->key) && (new->left == NULL)) && (prev->ropeL != NULL) ){ 
                            new->ropeL = prev->ropeL;
                            prev->ropeL->ropeR = new;
                            prev->ropeL = NULL;
                        }
                    }
                    if(prev->left != NULL)
                        prev->left->parent = new;
                    prev->left = new;
                    scan = NULL;
                    new = NULL;
                }
            }else
                scan = scan->right;
        }
        if(new != NULL){
            if(new->key < prev->key){
                prev->left = new;
                new->parent = prev;
                if( (prev->parent != NULL) && (prev->parent->right == prev) ){ // случай когда добавляем правому родителю левый элемент.
                    new->ropeL = prev->parent;
                    prev->parent->ropeR = new;
                }else{ 
                    if(prev->ropeL != NULL){        // если родитель не правый то проверяем на наличие указателя rope у prev
                        new->ropeL = prev->ropeL;
                        prev->ropeL->ropeR = new;
                        prev->ropeL = NULL;
                    }
                }
                new = NULL;
            }else{
                prev->right = new;
                new->parent = prev;
                if( (prev->parent != NULL) && (prev->parent->left == prev) ){ // случай когда добавляем левому родителю правый элемент.
                    new->ropeR = prev->parent;
                    prev->parent->ropeL = new;
                }else{
                    if(prev->ropeR != NULL){ // если родитель не левый то проверяем на наличие указателя rope у prev
                        new->ropeR = prev->ropeR;
                        prev->ropeR->ropeL = new;
                        prev->ropeR = NULL;
                    }
                }
                new = NULL;
            }
        }
        prev = NULL;
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
    printf("%-d\n", ptr->key);
    if(ptr->left)
        order_traversall(ptr->left, indent + 1);

}
//---------------------------------------------------------------------------
// Дополнительные задачи
//---------------------------------------------------------------------------
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
        if(ptr->parent)
            fprintf(file, "    %d -> %d [label=\"P\", fontsize=9];\n", ptr->key, ptr->parent->key);
        if(ptr->left)
            fprintf(file, "    %d -> %d [color = blue] [label=\"L\", fontsize=9];\n", ptr->key, ptr->left->key);
        if(ptr->right)
            fprintf(file, "    %d -> %d [color = crimson] [label=\"R\", fontsize=9];\n", ptr->key, ptr->right->key);
        if(ptr->ropeL)
            fprintf(file, "    %d -> %d [label=\"rL\", fontsize=9];\n", ptr->key, ptr->ropeL->key);
        if(ptr->ropeR)
            fprintf(file, "    %d -> %d [label=\"rP\", fontsize=9];\n", ptr->key, ptr->ropeR->key);
        order_traversall_graphiz(ptr->left, file);
        order_traversall_graphiz(ptr->right, file);
    }
}
//---------------------------------------------------------------------------
void binary_option(Node** root, int* count_keys){
    int a;
    printf("1) Save to bin file\n2) Loading from bin file\n3) Press any others to Quit\n");
    getInt(&a);
    switch(a)
    {
        case 1: save_to_bin((*root), count_keys); getchar(); break;
        case 2: read_from_bin(root, count_keys); getchar(); break;
        default: printf("[Leaving...]\n");
    }
}

//---------------------------------------------------------------------------
void Timing(){
    Node* root = NULL;
    int n = 10, key[10000], k, cnt = 1000000, m;
    clock_t first, last;
    srand(time(NULL));
    printf(" [ время -ПОИСКА- элементов ] \n");
    while (n-- > 0){ // поиск
        for (int i = 0; i < 10000; ++i)
            key[i] = rand() * rand();
        for (int i = 0; i < cnt; ++i){
            k = rand() * rand();
            insert(&root, k, 1);
        }
        m = 0;
        first = clock();
        for (int i = 0; i < 10000; ++i)
            if(find(root, key[i]))
                ++m;
        last = clock();
        printf("%d Элементов найдено:\n", m);
        printf("Тест #%d, Общее элементов = %d, Время = %.4lf сек.\n", 10 - n, (10 - n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
        printf("------------------------------поиск--------------------------------\n");
    }
    n = 10;
    printf(" [ время -УДАЛЕНИЯ- элементов ] \n");
    while (n-- > 0){ // удаление
        for (int i = 0; i < 10000; ++i)
            key[i] = rand() * rand();
        for (int i = 0; i < cnt; ++i){
            k = rand() * rand();
            k = abs(k);
            insert(&root, k, 1);
        }
        m = 0;
        first = clock();
        for (int i = 0; i < 10000; ++i)
            if(delete_timing(&root, key[i]))
                ++m;
        last = clock();
        printf("%d Элементов удалено:\n", m);
        printf("Тест #%d, Общее элементов = %d, Время = %.4lf сек.\n", 10 - n, (10 - n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
        printf("-----------------------------удаление-----------------------------\n");
    }
    clear_tree(root);
    root = NULL;
}
int find(Node* root, int key){
    Node* scan = root;
    while(scan != NULL){
        if(key <= scan->key){
            if(key == scan->key){
                return scan->key;
            }
            scan = scan->left;
        }else{
            scan = scan->right;
        }
    }
}

int delete_timing(Node** root, int key){
    Node* scan = *root;
    Node* prev = NULL;
    int a = 0;
    while(scan != NULL){
        if(key <= scan->key){
            prev = scan; 
            if(key == scan->key){ // нашли ключ
                if( (prev->left == NULL) && (prev->right == NULL) ){ // Лепесток
                    a++;
                    if(prev->parent != NULL){
                        // операции с нитями
                        if(prev->ropeL != NULL){
                            if(prev->ropeL->right == prev->parent){
                                prev->ropeL->ropeR = NULL;
                                prev->ropeL = NULL;
                            }else{
                                prev->parent->ropeL = prev->ropeL;
                                prev->ropeL->ropeR = prev->parent;
                                prev->ropeL = NULL;
                            }
                        }
                        if(prev->ropeR != NULL){
                            if(prev->ropeR->left == prev->parent){
                                prev->ropeR->ropeL = NULL;
                                prev->ropeR = NULL;
                            }else{
                                prev->parent->ropeR = prev->ropeR;
                                prev->ropeR->ropeL = prev->parent;
                                prev->ropeR = NULL;
                            }
                        }
                        //
                        if(prev->parent->left == prev)
                            prev->parent->left = NULL;
                        if(prev->parent->right == prev)
                            prev->parent->right = NULL;
                        prev->parent = NULL;
                        scan = scan->left;
                        free(prev);
                        continue;
                    }else{
                        scan = scan->left;
                        free(prev);
                        *root = NULL;
                        continue;
                    }
                }
                if( ( (prev->left != NULL) && (prev->right == NULL) ) || ( (prev->left == NULL) && (prev->right != NULL) ) ){ // Узел
                    a++;
                    if(prev->parent != NULL){
                        // нити
                        if( (prev->ropeL == NULL) && (prev->ropeR != NULL) ){ // Ропер
                                if( prev->parent->right == prev ){
                                    prev->ropeR->ropeL = prev->left;
                                    if(prev->left){
                                        prev->left->ropeR = prev->ropeR;
                                        if(prev->left->ropeL)
                                            prev->left->ropeL->ropeR = NULL;
                                        prev->left->ropeL = NULL;
                                    }
                                    prev->ropeR = NULL;
                                }
                                if(prev->parent->left == prev){
                                    if(prev->right->ropeR != NULL){
                                        prev->right->ropeR->ropeL = NULL;
                                        prev->right->ropeR = NULL;
                                    }
                                    prev->ropeR->ropeL = NULL;
                                    prev->ropeR = NULL;
                                }
                            }
                            if( (prev->ropeL != NULL) && (prev->ropeR == NULL) ){ // Ропель
                                if(prev->parent->left == prev){
                                    prev->ropeL->ropeR = prev->right;
                                    if(prev->right){
                                        prev->right->ropeL = prev->ropeL;
                                        if(prev->right->ropeR)
                                            prev->right->ropeR->ropeL = NULL;
                                        prev->right->ropeR = NULL;
                                    }
                                    prev->ropeL = NULL;
                                }
                                if(prev->parent->right == prev){
                                    if(prev->left->ropeL != NULL){
                                        prev->right->ropeL->ropeR = NULL;
                                        prev->right->ropeL = NULL;
                                    }
                                    prev->ropeL->ropeR = NULL;
                                    prev->ropeL = NULL;
                                }
                            }
                            if( (prev->ropeL != NULL) && (prev->ropeR != NULL) ){ // Сложный случай: Ропер и Ропель
                                if( prev->parent->right == prev ){
                                    prev->ropeL->ropeR = prev->ropeR;
                                    prev->ropeR->ropeL = prev->ropeL;
                                    prev->ropeR = NULL;
                                    prev->ropeL = NULL;
                                    if(prev->left->ropeL != NULL){
                                        prev->left->ropeL->ropeR = NULL;
                                        prev->left->ropeL = NULL;
                                    }
                                }
                                if(prev->parent->left == prev){
                                    prev->ropeR->ropeL = prev->ropeL;
                                    prev->ropeL->ropeR = prev->ropeR;
                                    prev->ropeL = NULL;
                                    prev->ropeR = NULL;
                                    if(prev->right->ropeL != NULL){
                                        if(prev->right->ropeR)
                                            prev->right->ropeR->ropeL = NULL;
                                        prev->right->ropeR = NULL;
                                    }
                                }
                            }
                        // удаление узла
                        if(prev->left != NULL){
                            prev->left->parent = prev->parent;
                            if(prev->parent->left == prev)
                                prev->parent->left = prev->left;
                            if(prev->parent->right == prev)
                                prev->parent->right = prev->left;
                            scan = scan->left;
                            prev->left = NULL;
                            prev->parent = NULL;
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
                            prev->right = NULL;
                            prev->parent = NULL;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                    }else{
                        if( (prev->ropeL == NULL) && (prev->ropeR != NULL) ){
                            prev->ropeR->ropeL = NULL;
                            prev->ropeR = NULL;
                        }
                        if( (prev->ropeL != NULL) && (prev->ropeR == NULL) ){
                            prev->ropeL->ropeR = NULL;
                            prev->ropeL = NULL;
                        }
                        if(prev->left != NULL){
                            prev->left->parent = NULL;
                            scan = scan->left;
                            prev->left = NULL;
                            *root = scan;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                        if(prev->right != NULL){
                            prev->right->parent = NULL;
                            scan = scan->right;
                            *root = scan;
                            prev->right = NULL;
                            free(prev);
                            prev = NULL;
                            continue;
                        }
                    }
                }
                if( (prev->left != NULL) && (prev->right != NULL)){ // Корень
                    a++;
                    if(prev->ropeR != NULL){
                        if(prev->ropeR->right != NULL){
                            if(prev->ropeR->right->ropeR)
                                prev->ropeR->right->ropeR->ropeL = NULL;
                            prev->ropeR->right->ropeR = NULL;
                            if(prev->ropeR->ropeR != NULL){
                                prev->ropeR->ropeR->ropeL = prev;
                                prev->ropeR->ropeR = NULL;
                            }
                            prev->key = prev->ropeR->key;
                            prev->info = prev->ropeR->info;
                            prev->ropeR->right->parent = prev->ropeR->parent;
                            prev->ropeR->parent->left = prev->ropeR->right;
                            prev->ropeR->right->parent = NULL;
                            Node* tmp = NULL;
                            if(prev->ropeR->ropeR == NULL){
                                tmp = prev->ropeR;
                                prev->ropeR->right->ropeL = prev->ropeR->ropeL;
                                prev->ropeR = prev->ropeR->right;
                                tmp->right = NULL;
                                free(tmp);
                                tmp = NULL;
                            }else{
                                tmp = prev->ropeR;
                                prev->ropeR->right = NULL;
                                free(tmp);
                                tmp = NULL;
                            }
                        }else{
                            prev->key = prev->ropeR->key;
                            prev->info = prev->ropeR->info;
                            prev->ropeR->ropeL = NULL;
                            Node* tmp = prev->ropeR;
                            prev->ropeR = NULL;
                            tmp->parent->left = NULL;
                            tmp->parent = NULL;
                            free(tmp);
                        }
                    }else{
                        prev->key = prev->right->key;
                        prev->info = prev->right->info;
                        if(prev->right->right != NULL){
                            Node* tmp = prev->right;
                            if(prev->right->ropeR)
                                prev->right->ropeR->ropeL = prev;
                            prev->ropeR = prev->right->ropeR;
                            prev->right->ropeR = NULL;
                            prev->right->right->parent = prev;
                            prev->right->parent = NULL;
                            prev->right = prev->right->right;
                            free(tmp);
                            tmp = NULL;
                        }else{
                            free(prev->right);
                            prev->right = NULL;
                        }
                    }
                }
            }
            scan = scan->left;
        }else{
            scan = scan->right;
        }
    }
    return a;
}

//---------------------------------------------------------------------------
void clear_tree(Node* root){
    if(root != NULL){
        clear_tree(root->left);
        clear_tree(root->right);
        free(root);
    }
}

int dialog(){
	int rc = 0;
	int n = 0;
	do{
		if(rc < 0 || rc >= 9)
            printf("You are wrong. Repeate, please!\n");
        printf("0. Quit\n1. Add\n2. Find\n3. Delete\n4. Show\n5. Special Operations\n6. Timing\n-------------->\n7. Save to PNG\n8. Load BIN and save to TXT\n");
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