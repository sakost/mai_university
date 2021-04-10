//
// Created by sakost on 28.09.2020.
//

// Саженов К.С.
// Группа М8О-208Б-19
// На github.com username sakost
// Создать класс TransNumber для работы с трансцендентными числами. Трансцендентное число представлено парой (a, b), где a – действительная часть, b – трансцендентная часть.
// Трансцендентная часть представляет собой действительное число b, умноженное на константу 2 .
// Реализовать арифметические операции (по аналогии с операциями над комплексными числами в алгебраической форме), и операции сравнения по значению (a + b ´ 2 ).
// Арифметические операции, а также сравнения (больше, меньше и равно) необходимо реализовать в виде перегрузки операторов.
// Необходимо реализовать пользовательский литерал для работы с константами типа TransNumber.


#include "TransNumber.h"

#include <iostream>

const TransNumber<long double> zero = TransNumber(0.l, 0.l), one = TransNumber(1.l, 1.l), null = TransNumber(-1.l, -1.l);

/**
 * adds custom const literals to program
 **/
TransNumber<long double> operator ""_tn(const char* s, size_t){
    std::string ss(s);
    if(ss == "zero"){
        return zero;
    }
    else if(ss == "one"){
        return one;
    }
    return null;
}

/**
 * help function prints help message to std::cout
 **/
void help(){
    std::cout << "plus real1 trans1 real2 trans2" << std::endl;
    std::cout << "minus real1 trans1 real2 trans2" << std::endl;
    std::cout << "mul real1 trans1 real2 trans2" << std::endl;
    std::cout << "div real1 trans1 real2 trans2" << std::endl;
    std::cout << "le real1 trans1 real2 trans2" << std::endl;
    std::cout << "lt real1 trans1 real2 trans2" << std::endl;
    std::cout << "ge real1 trans1 real2 trans2" << std::endl;
    std::cout << "gt real1 trans1 real2 trans2" << std::endl;
    std::cout << "ne real1 trans1 real2 trans2" << std::endl;
    std::cout << "eq real1 trans1 real2 trans2" << std::endl;
    std::cout << "exit to quit" << std::endl;
    std::cout << "help to get help" << std::endl;
}

using Number = long double;

int main() {
    help();
    std::string cmd;

    while (true){
        std::cout << ">>> ";
        std::cin >> cmd;
        // exit commands
        if(cmd == "quit" || cmd == "exit") {
            break;
        }
        // all available commands
        else if(cmd != "plus" && cmd != "minus" && cmd != "mul" && cmd != "div" && cmd != "eq"  && cmd != "ne"  && cmd != "le"  && cmd != "lt"  && cmd != "gt"  && cmd != "ge"){
            help();
            continue;
        }

        // create three numbers
        Number real1, trans1, real2, trans2;
        std::cin >> real1 >> trans1 >> real2 >> trans2;
        // create three transcendent numbers
        TransNumber<Number> n1(real1, trans1), n2(real2, trans2), n3;

        // given operations
        if(cmd == "plus"){
            n3 = n1 + n2;
        }
        else if(cmd == "minus"){
            n3 = n1 - n2;
        }
        else if(cmd == "mul"){
            n3 = n1 * n2;
        }
        else if(cmd == "div"){
            n3 = n1 / n2;
        }
        else {
            bool flag = false; // flag indicates comparision operator result
            if (cmd == "eq") {
                flag = n1 == n2;
            } else if (cmd == "ne") {
                flag = n1 != n2;
            } else if (cmd == "le") {
                flag = n1 <= n2;
            } else if (cmd == "lt") {
                flag = n1 < n2;
            } else if (cmd == "ge") {
                flag = n1 >= n2;
            } else if (cmd == "gt") {
                flag = n1 > n2;
            }
            if(flag) {
                std::cout << "true" << std::endl;
            } else{
                std::cout << "false" << std::endl;
            }
            continue;
        }
        // prints result
        std::cout << n3 << std::endl;
    }
}

