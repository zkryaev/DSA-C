#include "lab3aio.h"

int main(void) {
    int input;
    Table *table = (Table*)malloc(sizeof(Table));
    table->csize1 = 0; // индекс LPT
    printf("Enter the size of table:\n");
    int a = 0;
    while(getInt(&a) == 0){
    printf("You can't allocate memory this way, enter an integer:\n");
    }
    if(a > 0){
        table->msize = a;
        table->ks1 = (Keyspace1*)calloc(table->msize, sizeof(Keyspace1));
        table->ks2 = (Keyspace2*)calloc(table->msize, sizeof(Keyspace2));
        set_busy(table);
        while((input = dialog()))
        {
            switch(input){
            case 1: Add(table); break;
            case 2: Find(table); break;
            case 3: Delete(table); break;
            case 4: Show(table); getchar(); break;
            case 5: Special_Operations(table); break;
            }
        }
    }else{
        printf("You can't give size the above or equal 0\n");
        free(table);
        table = NULL;
        return 0;
    }
    clear_table(table);
    return 0;
}