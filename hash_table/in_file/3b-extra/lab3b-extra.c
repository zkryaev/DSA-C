#include "lab3bio.h"

int main(void){
    Table* table = (Table*)malloc(sizeof(Table));
    FILE* file = NULL;
    file = file_interaction(table); // ПЕРЕДЕЛАТЬ!!! На манер новой структуры
    if(file != NULL){
        int input = 0;
        while((input = dialog()))
        {
            switch(input){
            case 1: Add(table, file); break;
            case 2: Find(table, file); break;
            case 3: Iterator(table, file); break;
            case 4: Delete(table, file); break;
            case 5: Show(table, file); getchar(); break;
            case 6: Special_Operations(table, file); break;
            }
        }
        save_table_to_file(table, file);
        clear_table(table);
    }else{
        free(table);
        table = NULL;
        return 0;
    }
    free(table);
    table = NULL;
    fclose(file);
    return 0;  
}


