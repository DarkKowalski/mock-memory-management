#ifndef USER_INPUT_H
#define USER_INPUT_H
#include <stdbool.h>
#include <stdio.h>
#define ALLOC 'a'
#define FREE  'f'
typedef struct input_task
{
    int  id;
    int  size;
    bool alloc; /* set to alloc, clear to free */
} input_task;

#define MAX_INPUT (64)
typedef struct input_tasks
{
    int        total;
    input_task space[MAX_INPUT];
} input_tasks;

input_tasks *init_tasks_cli(input_tasks *tasks, int limit)
{
    if (tasks == NULL)
        return NULL;

    int total = 0;
    scanf("%d", &total);
    getchar();
    if (total > limit)
        return NULL;
    tasks->total = total;

    for (int i = 0; i < total; i++)
    {
        char command = getchar();

        if (command == ALLOC)
        {
            int id = 0;
            int size = 0;
            scanf("%d %d", &id, &size);
            getchar();
            tasks->space[i].id = id;
            tasks->space[i].size = size;
            tasks->space[i].alloc = true;
        }
        else if (command == FREE)
        {
            int id = 0;
            scanf("%d", &id);
            getchar();
            tasks->space[i].id = id;
            tasks->space[i].alloc = false;
        }
    }

    return tasks;
}

#endif