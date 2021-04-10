//
// Created by sakost on 09.11.2020.
//

#include "rectangle.h"


Point Rectangle::center() const {
    return (a+b+c+d)/4;
}

coord_t Rectangle::area() const {
    CustomVector ab(a, b), bc(b, c);
    return ab.Length() * bc.Length();
}

std::ostream &Rectangle::print(std::ostream &out) const {
    out << "Rectangle {" << a << "; " << b << "; " << c << "; " << d << "}";
    return out;
}

std::istream &Rectangle::scan(std::istream &in) {
    in >> a >> b >> c >> d;
    *this = Rectangle(a, b, c, d);
    return in;
}

Rectangle::Rectangle(const Point &a_, const Point &b_, const Point &c_, const Point &d_): a(a_), b(b_), c(c_), d(d_) {
    CustomVector ab(a, b), bc(b, c), cd(c, d), da(d, a);

    if (!isParallel(da, bc)) {
        std::swap(a, b);
        ab = { a, b };
        bc = { b, c };
        da = { d, a };
    }
    if (!isParallel(ab, cd)) {
        std::swap(b, c);
        ab = { a, b };
        bc = { b, c };
        da = { d, a };
    }

    if (ab * bc || bc * cd || cd * da || da * ab) {
        throw std::logic_error("The sides of the figure should be perpendicular");
    }
    if (ab.Length() != cd.Length() || bc.Length() != da.Length()){
        throw std::logic_error("The sides of the figure must be pairwise equal");
    }
    if (!ab.Length() || !bc.Length()) {
        throw std::logic_error("The sides of the figure must be greater than zero");
    }
}

Rectangle::Rectangle() : a(0, 0), b(0, 1), c(1, 0), d(1, 1) {}

