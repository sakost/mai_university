//
// Created by sakost on 27.11.2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


typedef enum {
    FIRST,
    SECOND,
} CONTEXT;

CONTEXT r = FIRST;

const char* libName1 = "libfirst.so";
const char* libName2 = "libsecond.so";

float (*sinInt)(float, float, float) = NULL;
char* (*translation)(long x) = NULL;
char *err;

void *libHandle = NULL;

void loadDLibs(CONTEXT context){
    const char *name;
    if(context == FIRST){
        name = libName1;
    } else{
        name = libName2;
    }

    libHandle = dlopen(name, RTLD_LAZY);
    if(!libHandle){
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
}

void unloadDLibs(){
    dlclose(libHandle);
}

void loadContext(){
    loadDLibs(r);
    sinInt = dlsym(libHandle, "SinIntegral");
    translation = dlsym(libHandle, "translation");
    if((err = dlerror())) {
        fprintf(stderr, "%s\n", err);
        exit(EXIT_FAILURE);
    }
}

void changeContext(){
    unloadDLibs();
    if(r == FIRST){
        r = SECOND;
    } else {
        r = FIRST;
    }

    loadContext();
}

int main(){
    r = FIRST;
    loadContext();

    int cmd = 0;

    while (scanf("%d", &cmd) != EOF){

        if(cmd == 0){
            changeContext();
            puts("Ok. Contract was changed");
            if(r == FIRST){
                puts("Now context is first");
            } else{
                puts("Now context is second");
            }
            continue;
        }
        if(cmd == 1) {
            float a, b, c;
            if (scanf("%f %f %f", &a, &b, &c) == EOF) {
                break;
            }
            printf("%f\n", sinInt(a, b, c));
        }
        else{
            long x;
            if(scanf("%ld", &x)== EOF){
                break;
            }

            char *string;
            printf("Translate integer(%ld) from 10 base to ", x);
            if(r == FIRST) {
                printf("2");
            }else{
                printf("3");
            }
            puts(" base");
            string = translation(x);
            printf("%s\n", string);
            free(string);
        }
    }
    unloadDLibs();
}