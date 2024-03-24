#include "lab4aio.h"
#include "lab4bin.h"

void save_to_bin(Node* root, int* count_keys){
    if(root){
        Node* tmp = root;
        printf("Enter the name of BIN file:\n");
        while(getchar() != '\n');
        char* fname = readline();
        FILE* file = fopen(fname, "wb");
        free(fname);
        fseek(file, 0, SEEK_SET);
        fwrite(count_keys, sizeof(int), 1, file); // записал количество элементов
        order_trav_bin(root, file);
        printf("[ The table saved in file! ]\n");
        fclose(file);
    }else
        printf(" [Tree is empty!]\n");
}

void order_trav_bin(Node* ptr, FILE* file){
    if(ptr != NULL){
    fwrite(&(ptr->key), sizeof(int), 1, file);
    order_trav_bin(ptr->left, file);
    order_trav_bin(ptr->right, file);
    }
}

//---------------------------------------------------->
void read_from_bin(Node** root, int* count_keys){
    if( (*count_keys) == 0 ){
        printf("Enter the name of BIN file:\n");
        while(getchar() != '\n');
        char* fname = readline();
        FILE* file = fopen(fname, "rb");
        if(file){
            fseek(file, 0, SEEK_SET);
            fread(count_keys, sizeof(int), 1, file);
            int* arrkeys = (int*)calloc((*count_keys), sizeof(int));
            int key = 0;
            for(int i = 0; i < (*count_keys); i++){
                fread(&key, sizeof(int), 1, file);
                arrkeys[i] = key;
                insert(root, key, 1);
            }
            save_txt_binopt((*root), count_keys, arrkeys);
            printf("[ The table read from file and set to TXT counts of keys! ]\n");
            free(fname);            
            free(arrkeys);
            fclose(file);
        }else
            printf(" [ File is not exist! ]\n");
    }else
        printf(" [ Delete from tree all elements ]\n");
}
//---------------------------------------------------->
void save_txt_binopt(Node* root, int* count_keys, int* arrkeys){
    if(root != NULL){
        printf("Enter the name of TXT file:\n");
        char* txtfname = readline();
        FILE* txtfile = fopen(txtfname, "wr");
        int prevkey = -1;
        for(int i = 0; i < (*count_keys); i++){
            if( arrkeys[i] != prevkey ){
                fprintf(txtfile, "key: %d | count: %d\n", arrkeys[i], count_repeatkey(root, arrkeys[i]));
                prevkey = arrkeys[i];
            }
        }
        free(txtfname);
        fclose(txtfile);
    }else
        printf("[ Tree is empty! ]\n");
}

int count_repeatkey(Node* root, int key){
    Node* scan = root;
    int ind = 0;
    while(scan != NULL){
        if(key <= scan->key){
            if(key == scan->key){
                ind++;
            }
            scan = scan->left;
        }else{
            scan = scan->right;
        }
    }
    return ind;
}