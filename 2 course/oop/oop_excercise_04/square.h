//
// Created by sakost on 09.11.2020.
//

#ifndef OOP_EXCERCISE_04_SQUARE_H
#define OOP_EXCERCISE_04_SQUARE_H


template <typename T>
struct Square {
    std::pair<T, T> points[4];
    const std::string name = "Square";
};


#endif //OOP_EXCERCISE_04_SQUARE_H
