//
// Created by sakost on 09.04.2020.
//

#include <stdio.h>

#include "lab.h"

void procedure(List *list){
    list_element *el = NULL;

    for (list_element *cur = list_get(list, 0); cur != list_end_el(list); cur = list_next_element(cur)) {
        if(cur == list_start_el(list)) continue;
        if(list_get_element_data(cur) < list_get_element_data(list_prev_element(cur))){
            el = cur;
            break;
        }
    }
    if(el == NULL) return;
    while(el != list_start_el(list) && list_get_element_data(el) <= list_get_element_data(list_prev_element(el))){
        list_swap_elements(el, list_prev_element(el));
        el = list_prev_element(el);
    }
}

void sort(List *list){
    for (int i = 0; i < list_size(list); ++i) {
        procedure(list);
    }
}

int main(){
    List* l = list_init();
    list_append(l, 10);
    list_append(l, 10);
    list_append(l, 7);
    list_append(l, 5);
    list_append(l, 81);
    sort(l);
    list_printf(l);

    list_free(l);
}
