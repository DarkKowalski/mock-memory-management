#include "freelist.h"
#include "user_intput.h"

#ifdef FIRST_MATCH
#define match_alloc(list, id, size) first_match_alloc((list), (id), (size))
#elif defined BEST_MATCH
#define match_alloc(list, id, size) best_match_alloc((list), (id), (size))
#endif

int main(void)
{

    freelist list;
    init_freelist(&list, MEMORY_SIZE);
    input_tasks tasks;
    init_tasks_cli(&tasks, MAX_INPUT);

    for (int i = 0; i < tasks.total; i++)
    {
        int id = tasks.space[i].id;
        if (tasks.space[i].alloc)
        {
            int size = tasks.space[i].size;
            printf("[INFO]: Alloc %dKB for Task %d\n", size, id);
            match_alloc(&list, id, size);
        }
        else
        {
            printf("[INFO]: Free memory for Task %d\n", id);
            free_node(&list, id);
        }
        print_list(&list);
    }

    destory_list(&list);
    return 0;
}