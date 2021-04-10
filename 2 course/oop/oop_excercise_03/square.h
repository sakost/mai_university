//
// Created by sakost on 08.11.2020.
//

#ifndef OOP_EXCERCISE_03_SQUARE_H
#define OOP_EXCERCISE_03_SQUARE_H

#include "rectangle.h"

class Square : public Rectangle {
public:
    Square() : Rectangle(){}
    Square(const Point& a_, const Point& b_, const Point& c_, const Point& d_);

    std::ostream &print(std::ostream &out) const override;
};


#endif //OOP_EXCERCISE_03_SQUARE_H
