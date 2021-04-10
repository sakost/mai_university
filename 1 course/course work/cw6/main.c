//
// Created by sakost on 09.04.2020.
//
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char surname[30+1];
    char initials[2+1];
    char sex[6+1];
    int schoolNumber;
    char haveMedal;
    int math, phys, lit;
    char passedWriting;
} Student;

int main(int argc, char* argv[]){
    char *filename;
    if(argc < 2){
        filename = "test.txt";
    } else{
        filename = argv[1];
    }

    const int count = 15;
    FILE *file = fopen(filename, "r");

    Student *students = (Student*)calloc(count, sizeof(Student));
    Student *result = (Student*)calloc(count, sizeof(Student));
    int i = 0, ires = 0;
    while(!feof(file)){
        Student s;
        fscanf(file, "%s %2s %s %d %c %d %d %d %c", s.surname, s.initials, s.sex, &s.schoolNumber,
               &s.haveMedal, &s.math, &s.phys, &s.lit, &s.passedWriting);
        students[i++] = s;
    }

    int p;
    printf("What's a p?\n>>> ");
    scanf("%d", &p);

    puts("--------------------------------------------------------");
    puts("|Surname     |In|Sex|School|Medal|Math|Phys|Lit|Writing|");
    puts("--------------------------------------------------------");

    for (int j = 0; j < count; ++j) {
        Student s = students[j];
        if(s.haveMedal == '+' && (s.math < p || s.phys < p || s.lit < p)){
            printf("|%12s|%2s|%6s|%6d|%2c|%4d|%4d|%3d|%7c|\n", s.surname, s.initials, s.sex,
                   s.schoolNumber, s.haveMedal, s.math, s.phys, s.lit, s.passedWriting);
            result[ires++] = s;
        }
    }

    if(ires == 0){
        puts("|                       NO DATA                        |");
    }

    puts("--------------------------------------------------------");


    fclose(file);
}
