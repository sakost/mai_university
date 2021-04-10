//
// Created by sakost on 09.11.2020.
//

#include "trapezoid.h"

Point Trapezoid::center() const {
    return (a + b + c + d) / 4;
}

coord_t Trapezoid::area() const {
    CustomVector ab(a, b), bc(b, c), cd(c, d), da(d, a);

    if(isParallel(ab, cd)){ // bc || da
        std::swap(ab, bc);
        std::swap(cd, da);
    }
    if(da.Length() > bc.Length()){ // bc greater than da
        std::swap(da, bc);
    }

    coord_t h = distance(b, c, d);
    return h * (bc.Length() + da.Length()) / 2;
}

std::ostream &Trapezoid::print(std::ostream &out) const {
    out << "Trapezoid {" << a << "; " << b << "; " << c << "; " << d << "}";
    return out;
}

std::istream &Trapezoid::scan(std::istream &in) {
    in >> a >> b >> c >> d;
    return in;
}

Trapezoid::Trapezoid() : a(0, 0), b(0, 1), c(1, 1), d(1, 0){}

Trapezoid::Trapezoid(const Point &a_, const Point &b_, const Point &c_, const Point &d_) {
    CustomVector ab(a, b), bc(b, c), cd(c, d), da(d, a);

    if (!isParallel(ab, cd) && !isParallel(bc, da)) {
        std::swap(a, c);
        ab = CustomVector(a, b);
        cd = CustomVector(c, d);
        da = CustomVector(d, a);
    }

    if (!ab.Length() || !bc.Length() || !cd.Length() || !da.Length()) {
        throw std::logic_error("The sides of the figure must be greater than zero");
    }

    if (!isParallel(ab, cd) && !isParallel(bc, da)) {
        throw std::logic_error("At least two sides of figure must be parallel");
    }
}
