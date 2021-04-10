#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <wchar.h>
#include <locale.h>

// 5


typedef unsigned set_data_elem;
enum{
    bits_per_char=16u,
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

const wchar_t sgood_vowels[] = L"аоуэыи";
const wchar_t svowels[] = L"аеиоуыэюя";

bool in_string(const wchar_t str[], wchar_t c){
    size_t n = wcslen(str);
    for(int i = 0; i < n; i++){
        if(str[i] == c)return true;
    }
    return false;
}


int main(){
    setlocale(LC_ALL, "");
    freopen("in.txt", "r", stdin);

    set good_vowels;
    set vowels;
    set_clear(&vowels);
    set_clear(&good_vowels);

    for (wchar_t i = L'а'; i != L'я'; ++i) {
        if (in_string(sgood_vowels, i))
            set_insert(&good_vowels, i);
        if(in_string(svowels, i)) set_insert(&vowels, i);
    }

    wchar_t buf;
    set s={}, empty_set = {};
    set_clear(&s);
    set_clear(&empty_set);
    while(wscanf(L"%lc", &buf) > 0){
        if(buf == L' '){
            if(set_includes(&good_vowels, &s)){
                printf("yes");
                return 0;
            }
            set_clear(&s);
        }
        else if(!(buf <= L'я' && buf >= L'а')){
            set_clear(&s);
            continue;
        }
        else if(set_in(&vowels, buf))
            set_insert(&s, buf);
    }
    printf("no");
}
