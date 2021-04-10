//
// Created by sakost on 17.12.2020.
//

#ifndef OOP_EXCERCISE_07_FACTORY_H
#define OOP_EXCERCISE_07_FACTORY_H

#include "figure.h"

#include <vector>
#include <memory>

template<typename ...Args>
class Factory {
public:
    virtual Figure* create_object(Args ...args) = 0;
};


#endif //OOP_EXCERCISE_07_FACTORY_H
