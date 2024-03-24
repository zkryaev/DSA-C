#include "lab4bio.h"


int main()
{
    Node* root = NULL;
    int a = 0;
    while( a = dialog() )
    {
        switch(a){
        case 1: Add(&root); break;
        case 2: Find(root); break;
        case 3: Delete(&root); break;
        case 4: Show(root); getchar(); break;
        case 5: Special_Operations(&root); break;
        case 6: Timing(); break;
        case 7: graphiz_option(root); break;
        case 8: BIN_menu(&root); break;
        default: printf("[ Leaving... ]\n");
        }
    }
    clear_tree(root);
    root = NULL;
    return 0;
}