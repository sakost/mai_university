//
// Created by sakost on 08.04.2020.
//
#ifndef LAB_H
#define LAB_H

typedef struct list_element{
    int field;
    struct list_element *prev, *next;
} list_element;

typedef struct{
    list_element *start_el, *end_el;
    int size;
} List;

List* list_init();
list_element *list_get(List *list, int index);
list_element *list_prev_element(list_element *el);
list_element *list_next_element(list_element *el);
int list_get_element_data(list_element *el);
void list_free(List *list);
void list_insert(List *list, int index, int element);
void list_pop_element(List *list, int index);
int list_size(List *list);
list_element* list_end_el(List *list);
list_element* list_start_el(List *list);
void list_swap_elements(list_element* first, list_element* second);
void list_printf(List *list);
void list_append(List *list, int element);

#endif // LAB_H