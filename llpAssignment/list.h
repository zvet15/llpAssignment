#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h> //for random
#include "echo.h"
typedef struct Car
{
    char name[MAX_STR];
    struct Car *next;
} car;

/* Head of the list. NULL means empty list */
car *list_head = NULL;

/* Retrieves the list head record or NULL if list is empty */
car *get_list_head()
{
    return list_head;
}

car *find_last()
{
    car *lastNode = get_list_head();
    while (lastNode->next != NULL)
    { //if the head
        lastNode = lastNode->next;
    }
    return lastNode;
}

/* Add to last */
void add_car_at_end(car *new_car)
{
    if (get_list_head() == NULL)
    {
        list_head = new_car;
    }
    else
    {
        car *lastCar = find_last();
        lastCar->next = new_car;
    }
}

/* Count list */
int count_list_size()
{
    int total = 0;
    car *currentCar = list_head;
    while (currentCar != NULL)
    {
        total += 1;
        currentCar = currentCar->next;
    }
    return total;
}

/* Clear*/
void free_list()
{
    car *tempCar = NULL;
    if (list_head != NULL)
    {
        while (list_head != NULL)
        {
            tempCar = list_head->next;
            free(list_head);
            list_head = tempCar;
        }
    }
}

/* Random*/
car *get_car_at_index(int index)
{
    int counter = 0;
    car *currentCar = list_head;
    while ((currentCar != NULL) && (index != counter))
    {
        currentCar = currentCar->next;
        counter += 1;
    }
    return currentCar;
}

/* Search the list for a particular name, return NULL if not found */
car *search_by_name(const char *name)
{
    car *currentCar = list_head;

    while (currentCar != NULL)
    {
        if (strstr(currentCar->name, name))
        {
            return currentCar;
        }
        currentCar = currentCar->next;
    }
    return NULL;
}

//get the node and add a car
void generate_car(char *str)
{
    car *c = (car *)malloc(sizeof(car));
    if (c != NULL)
    {
        strcpy(c->name, str);
        add_car_at_end(c);
    }
}
