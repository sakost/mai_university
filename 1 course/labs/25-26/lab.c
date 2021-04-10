//
// Created by sakost on 08.04.2020.
//
#include "lab.h"

#include <malloc.h>
#include <stdio.h>

List *list_init(){
    List *l = (List*) malloc(sizeof(List));
    l->start_el = NULL;
    l->end_el = NULL;
    return l;
}

list_element *forward_get(List *list, int index){
    int cur_index = 0;
    list_element *cur_el = list->start_el;
    while(cur_index++ != index){
        cur_el = cur_el->next;
    }
    return cur_el;
}

list_element *backward_get(List *list, int index){
    int cur_index = list->size - 1;
    list_element *cur_el = list->end_el;
    while(cur_index-- != index){
        cur_el = cur_el->prev;
    }
    return cur_el;
}

list_element *list_get(List *list, int index){
    if(index >= list->size){
        return 0;
    }
    if(index < list->size / 2){
        return forward_get(list, index);
    }
    return backward_get(list, index);
}

list_element *list_prev_element(list_element *el){
    return el->prev;
}

list_element *list_next_element(list_element *el){
    return el->next;
}

int list_get_element_data(list_element *el){
    return el->field;
}
void list_free(List *list){
    list_element *cur_el = list->start_el, *tmp_el;
    while(cur_el != list->end_el){
        tmp_el = cur_el;
        cur_el = cur_el->next;
        free(tmp_el);
    }
    free(list);
}

void list_insert(List *list, int index, int element){
    if(list->start_el == NULL){
        list->start_el = (list_element*)malloc(sizeof(list_element));
        list->end_el = list->start_el;

        list->start_el->prev = list->end_el;
        list->start_el->next = list->end_el;

        list->end_el->next = list->start_el;
        list->end_el->prev = list->start_el;
        list->size = 1;
        return;
    }
    list_element *cur_el = list_get(list, index), *prev_el;
    prev_el = cur_el->prev;
    prev_el->next = (list_element*)malloc(sizeof(list_element));
    prev_el->next->prev = prev_el;
    prev_el->next->next = cur_el;
    cur_el->prev = prev_el->next;
    prev_el->next->field = element;
    list->size ++;
}

void list_append(List *list, int element){
    list_element *new_el =  (list_element*)malloc(sizeof(list_element));
    new_el->field = element;
    if(list->start_el == NULL){
        list->start_el = new_el;
        list->end_el = new_el;

        new_el->prev = new_el;
        new_el->next = new_el;
        return;
    }

    list->end_el->next = new_el;
    list->start_el->prev = new_el;
    new_el->prev = list->end_el;
    new_el->next = list->start_el;
    list->end_el = new_el;
    list->size++;
}

void list_pop_element(List *list, int index){
    list_element *cur_el = list_get(list, index);
    cur_el->prev->next = cur_el->next;
    cur_el->next->prev = cur_el->prev;
    free(cur_el);
}

int list_size(List *list){
    return list->size;
}

list_element* list_end_el(List *list){
    return list->end_el;
}

list_element* list_start_el(List *list){
    return list->start_el;
}

void list_swap_elements(list_element* first, list_element* second){
    int tmp = first->field;
    first->field = second->field;
    second->field = tmp;
}

void list_printf(List *list){
    list_element *cur = list_get(list, 0);
    for (; cur != list_end_el(list); cur = list_next_element(cur)) {
        printf("%d\n", list_get_element_data(cur));
    }
    printf("%d\n", list_get_element_data(cur));
}
