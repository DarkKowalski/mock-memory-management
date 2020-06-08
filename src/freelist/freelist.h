#ifndef FREELIST_H
#define FREELIST_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE (640)
#define UNUSED_ID   (-1)

typedef struct node
{
    int          id;
    int          size;
    int          start;
    int          end;
    bool         avaliable;
    struct node *prev;
    struct node *next;
} node;

typedef struct freelist
{
    struct node *head;
} freelist;

freelist *init_freelist(freelist *list, int size)
{
    if (list == NULL)
        return NULL;
    if (size <= 0)
        return list;

    list->head = (node *)malloc(sizeof(node));
    if (list->head == NULL)
        return list;
    list->head->id = UNUSED_ID;
    list->head->avaliable = false;
    list->head->prev = NULL;
    list->head->next = (node *)malloc(sizeof(node));
    if (list->head->next == NULL)
        return list;

    list->head->next->id = UNUSED_ID;
    list->head->next->size = size;
    list->head->next->start = 0;
    list->head->next->end = size - 1;
    list->head->next->prev = list->head;
    list->head->next->next = NULL;
    list->head->next->avaliable = true;

    return list;
}

void free_node(freelist *list, int id)
{
    if (list == NULL || list->head == NULL || list->head->next == NULL ||
        id < 0)
        return;

    node *current = list->head->next;
    while (current != NULL)
    {
        if (current->id == id && !(current->avaliable)) /* Found */
        {
            if (current->next != NULL && current->next->avaliable)
            {
                node *next = current->next;
                current->size += next->size;
                current->end = next->end;
                current->next = next->next;
                free(next);
            }

            if (current->prev != NULL && current->prev->avaliable)
            {
                node *prev = current->prev;
                prev->size += current->size;
                prev->end = current->end;
                prev->next = current->next;
                free(current);
                current = NULL;
            }

            if (current != NULL)
            {
                current->avaliable = true;
            }
        }
        else /* Not found */
        {
            current = current->next;
        }
    }
}
node *first_match_alloc(freelist *list, int id, int size)
{
    if (list == NULL || list->head == NULL || list->head->next == NULL)
        return NULL;

    node *current = list->head->next;
    while (current != NULL)
    {
        if (current->avaliable)
        {
            if (current->size == size)
            {
                current->avaliable = false;
                current->id = id;
                return current;
            }

            if (current->size > size)
            {
                node *new_node = (node *)malloc(sizeof(node));
                if (new_node == NULL)
                    return NULL;
                new_node->next = current;
                new_node->prev = current->prev;

                new_node->id = id;
                new_node->size = size;
                new_node->start = current->start;
                new_node->end = current->start + size - 1;
                new_node->avaliable = false;

                current->start += size;
                current->size -= size;
                current->prev->next = new_node;
                current->prev = new_node;
                return new_node;
            }
            if (current->size < size)
            {
                current = current->next;
            }
        }
        else /* Occupied */
        {
            current = current->next;
        }
    }

    return NULL; /* Failed */
}

node *best_match_alloc(freelist *list, int id, int size)
{
    if (list == NULL || list->head == NULL || list->head->next == NULL)
        return NULL;

    node *current = list->head->next;
    int   best_fragment = __INT_MAX__;
    node *best_match = NULL;

    while (current != NULL)
    {
        if (current->avaliable)
        {
            if (current->size == size)
            {
                current->avaliable = false;
                current->id = id;
                return current;
            }

            if (current->size > size)
            {
                int fragment = current->size - size;
                if (fragment < best_fragment)
                {
                    best_fragment = fragment;
                    best_match = current;
                }
            }
        }

        current = current->next;
    }

    if (best_match == NULL)
        return NULL; /* Failed */
    else
    {
        node *new_node = (node *)malloc(sizeof(node));
        if (new_node == NULL)
            return NULL;
        new_node->next = best_match;
        new_node->prev = best_match->prev;

        new_node->id = id;
        new_node->size = size;
        new_node->start = best_match->start;
        new_node->end = best_match->start + size - 1;
        new_node->avaliable = false;

        best_match->start += size;
        best_match->size -= size;
        best_match->prev->next = new_node;
        best_match->prev = new_node;

        return new_node;
    }
}

void print_list(freelist *list)
{
    if (list == NULL || list->head == NULL)
        return;

    for (int i = 0; i < 80; i++)
        printf("=");
    printf("\n");

    int   index = 0;
    node *current = list->head->next;
    while (current != NULL)
    {
        node *next = current->next;
        if (current->avaliable)
        {
            printf("[INFO]: Index: %d, Avaliable, size %d, from %d to %d\n",
                   index, current->size, current->start, current->end);
            index++;
        }
        else
        {
            printf("[INFO]: Index: %d, ID: %d, size %d, from %d to %d\n", index,
                   current->id, current->size, current->start, current->end);
            index++;
        }

        current = next;
    }
    printf("\n");
}

void destory_list(freelist *list)
{
    if (list == NULL || list->head == NULL)
        return;

    node *current = list->head;
    while (current != NULL)
    {
        node *next = current->next;
        free(current);
        current = next;
    }
}

#endif