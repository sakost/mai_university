//
// Created by sakost on 17.12.2020.
//

#ifndef OOP_EXCERCISE_07_FIGUREFACTORY_H
#define OOP_EXCERCISE_07_FIGUREFACTORY_H

#include "factory.h"
#include "figure.h"

#include <type_traits>

class FigureFactory : Factory<const std::string&>, Factory<> {};


#endif //OOP_EXCERCISE_07_FIGUREFACTORY_H
