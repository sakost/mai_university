//
// Created by sakost on 09.11.2020.
//

#ifndef OOP_EXCERCISE_04_TRAPEZOID_H
#define OOP_EXCERCISE_04_TRAPEZOID_H

#include <utility>
#include <string>

template <typename T>
struct Trapezoid {
    std::pair<T, T> points[4];

    std::string name = "Trapezoid";
};


#endif //OOP_EXCERCISE_04_TRAPEZOID_H
