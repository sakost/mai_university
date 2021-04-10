//
// Created by sakost on 09.11.2020.
//

#ifndef OOP_EXCERCISE_03_RECTANGLE_H
#define OOP_EXCERCISE_03_RECTANGLE_H

#include "figure.h"

class Rectangle : public Figure{
public:
    Rectangle();
    Rectangle(const Point& a_, const Point& b_, const Point& c_, const Point& d_);

    [[nodiscard]] Point center() const override;
    [[nodiscard]] coord_t area() const override;
protected:
    std::ostream &print(std::ostream &out) const override;
    std::istream &scan(std::istream &in) override;

    Point a, b, c, d;
};


#endif //OOP_EXCERCISE_03_RECTANGLE_H
