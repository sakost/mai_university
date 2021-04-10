//
// Created by sakost on 08.11.2020.
//

#include "square.h"

Square::Square(const Point &a_, const Point &b_, const Point &c_, const Point &d_) : Rectangle(a_, b_, c_, d_) {
    CustomVector ab(a, b), da(d, a);

    if (ab.Length() != da.Length()){
        throw std::logic_error("The sides of the figure must be equal each other");
    }
}

std::ostream &Square::print(std::ostream &out) const {
    out << "Square {" << a << "; " << b << "; " << c << "; " << d << "}";
    return out;
}
