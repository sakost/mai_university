//
// Created by sakost on 20.12.2020.
//

#ifndef OOP_EXCERCISE_07_CONCRETEFACTORIES_H
#define OOP_EXCERCISE_07_CONCRETEFACTORIES_H

#include "figurefactory.h"
#include "concretefigures.h"

class TriangleFactory: public FigureFactory{
public:
    Figure *create_object(const std::string &raw_data) override {
        auto t = new Triangle;
        t->set_raw_data(raw_data);
        return t;
    }

    Figure *create_object() override {
        return new Triangle;
    }
};


class RectangleFactory: public FigureFactory{
public:
    Figure *create_object(const std::string &raw_data) override {
        auto r = new Rectangle;
        r->set_raw_data(raw_data);
        return r;
    }

    Figure *create_object() override{
        return new Rectangle;
    }
};

class SquareFactory: public FigureFactory{
public:
    Figure *create_object(const std::string &raw_data) override {
        auto s = new Square;
        s->set_raw_data(raw_data);
        return s;
    }

    Figure *create_object() override{
        return new Square;
    }
};

#endif //OOP_EXCERCISE_07_CONCRETEFACTORIES_H
