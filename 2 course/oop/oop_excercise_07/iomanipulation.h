//
// Created by sakost on 17.12.2020.
//

#ifndef OOP_EXCERCISE_07_IOMANIPULATION_H
#define OOP_EXCERCISE_07_IOMANIPULATION_H

#include "factory.h"

#include <ios>

template <typename T> class Serializer {
public:
  ~Serializer() = delete;
  static void serialize(const T &obj, std::ostream &out) {}
};

template <typename T, typename... Args> class Loader {
public:
  ~Loader() = delete;
  static T load(std::istream &in) { return factory.create_object(); }
};

#endif // OOP_EXCERCISE_07_IOMANIPULATION_H
