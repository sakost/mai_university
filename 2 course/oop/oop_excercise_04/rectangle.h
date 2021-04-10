//
// Created by sakost on 09.11.2020.
//

#ifndef OOP_EXCERCISE_04_RECTANGLE_H
#define OOP_EXCERCISE_04_RECTANGLE_H

#include <utility>
#include <string>


template <typename T>
struct Rectangle {
    std::pair<T, T> points[4];

    std::string name = "Rectangle";
};


#endif //OOP_EXCERCISE_04_RECTANGLE_H
