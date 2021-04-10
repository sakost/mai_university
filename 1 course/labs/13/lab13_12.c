 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// 12 - есть ли согласная, входящая в состав всех слов?


typedef unsigned set_data_elem;
enum{
    bits_per_char=8u,
    bits_per_elem= sizeof(set_data_elem) * bits_per_char,
    datalen = (1u << bits_per_char) / bits_per_elem
};

typedef struct {
    set_data_elem data[datalen];
} set;

void set_clear(set *s){
    memset(s->data, 0, sizeof(s->data));
}

void set_insert(set *s, int c){
    s->data[c / bits_per_elem] |= 1u << (c % bits_per_elem);
}

void set_generate(set *s, bool indicator(int)){
    set_clear(s);
    for (int i = 0; i != 1u << bits_per_char; ++i) {
        if(indicator(i)) set_insert(s, i);
    }
}

void set_erase(set *s, int c){
    s -> data[c/bits_per_elem] &= ~(1u << (c % bits_per_elem));
}

bool set_in(const set *s, int c){
    return (s->data[c / bits_per_elem] & (1u << (c % bits_per_elem))) != 0;
}

int set_size(const set *s){
    int size = 0;
    for(int i = 0; i != 1u << bits_per_char; i++){
        if(set_in(s, i)) size ++;
    }
    return size;
}

bool set_equal(const set *s1, const set *s2){
    for (int i = 0; i != datalen; ++i) {
        if(s1->data[i] != s2->data[i]) return false;
    }
    return true;
}

bool set_includes(const set *s1, const set *s2){
    for(int i = 0; i != datalen; i++){
        if((s1->data[i] | s2->data[i]) != s1->data[i]) return false;
    }
    return true;
}

set set_union(const set *s1, const set *s2){
    set result = {};
    for (int i = 0; i != datalen; ++i) {
        result.data[i] = s1->data[i] | s2->data[i];
    }
    return result;
}

set set_difference(const set *s1, const set *s2){
    set result = {};
    for (int i = 0; i != datalen; ++i) {
        result.data[i] = s1->data[i] & ~s2->data[i];
    }
    return result;
}

set set_symmetric_difference(const set *s1, const set *s2){
    set result = {};
    for (int i = 0; i != datalen; ++i) {
        result.data[i] = s1->data[i] ^ s2->data[i];
    }
    return result;
}


bool is_alpha(int c){return isalpha(c);}
bool is_digit(int c){return isdigit(c);}



int main(){
    freopen("in.txt", "r", stdin);

    set consonants, vowels; // согласные, гласные

    set_insert(&vowels, 'a');
    set_insert(&vowels, 'e');
    set_insert(&vowels, 'i');
    set_insert(&vowels, 'o');
    set_insert(&vowels, 'u');

    for(char ch = 'a'; ch != 'z'; ch++){
        if(!set_in(&vowels, ch)) set_insert(&consonants, ch);
    }

    set uSet = {}, word_set = {}, empty_set = {};
    char buf;
    bool start = true;

    while(scanf("%c", &buf) > 0){
        if(start){
            if(buf == ' '){
                start = false;
                uSet = set_union(&uSet, &word_set);
                set_clear(&word_set);
            }
            else{
                set_insert(&word_set, tolower(buf));
            }
        } else{
            if(buf == ' '){
                set u = set_union(&uSet, &word_set);
                set sd = set_symmetric_difference(&uSet, &word_set);

                uSet = set_difference(&u, &sd);
                if(set_includes(&empty_set, &uSet)){ // пустое принадлежит uSet
                    printf("No, there isn\'t any consonants that are included in all words\n");
                    return 0;
                }
                set_clear(&word_set);
            } else if(set_in(&consonants, buf)){
                set_insert(&word_set, buf);
            }
        }
    }
    if(set_includes(&empty_set, &uSet)){
        printf("No, there isn\'t any consonants that are included in all words\n");
    } else{
        for(char ch = 'a'; ch != 'z'; ch++){
            if(set_in(&uSet, ch)){
                printf("Yes, this consonant is '%c'\n", ch);
                return 0;
            }
        }
    }

}
