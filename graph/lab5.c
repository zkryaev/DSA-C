#include "lab5io.h"


int main()
{
    adjencylist* adlist = (adjencylist*)malloc(sizeof(adjencylist));
    adlist->array_vertices = NULL;
    adlist->num_vertices = 0;
    int a = 0;
    int d = 0;
    while( a = dialog() )
    {
        switch(a){
        case 1: add_vertex(adlist); break;
        case 2: add_edge(adlist); break;
        case 3: delete_vertex(adlist); break;
        case 4: delete_edge(adlist); break;
        case 5: show_adjancylist(adlist); break;
        case 6: BFS(adlist); break;
        case 7: blmnford_shortpath(adlist); break;
        case 8: topologic_sort(adlist); break;
        case 9: graphviz(adlist); break;
        case 10: work_withroutes(adlist); break;
        default: printf("[ Лондон гудбай о-о-о-о... ]\n");
        }
    }
    clear_graph(adlist);
    free(adlist);
    adlist = NULL;
    return 0;
}