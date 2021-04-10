//
// Created by sakost on 08.11.2020.
//

#include "customvector.h"

#include <cmath>

CustomVector::CustomVector(coord_t x_, coord_t y_) : x(x_), y(y_) {}
CustomVector::CustomVector(const Point &from, const Point &to) : x(to.X() - from.X()), y(to.Y() - from.Y()) {}

coord_t CustomVector::operator*(const CustomVector &rhs) const {
    return this->x * rhs.x + this->y * rhs.y;
}

CustomVector CustomVector::operator+(const CustomVector &other) const {
    return {this->x + other.x, this->y + other.y};
}

CustomVector CustomVector::operator-() const {
    return {-this->x, -this->y};
}

CustomVector CustomVector::operator-(const CustomVector &other) const {
    return *this + (-other);
}

CustomVector &CustomVector::operator=(const CustomVector &rhs) = default;

coord_t CustomVector::X() const {
    return this->x;
}

coord_t CustomVector::Y() const {
    return this->y;
}

coord_t CustomVector::Length() const {
    return std::sqrt((*this) * (*this));
}

coord_t dot(const CustomVector& lhs, const CustomVector& rhs) {
    return lhs.X() * rhs.Y() - lhs.Y() * rhs.X();
}

bool isParallel(const CustomVector& lhs, const CustomVector& rhs){
    return dot(lhs, rhs) == 0;
}

coord_t distance(const Point& l1, const Point& l2, const Point& p){
    coord_t u = (p.X() - l1.X()) * (l2.X() - l1.X()) + (p.Y() - l1.Y()) * (l2.Y() - l1.Y());
    const CustomVector v(l1, l2);
    u /= v*v;
    Point pp(l1.X() + u*(l2.X() - l1.X()), l1.Y() + u * (l2.Y() - l1.Y()));
    CustomVector vec(p, pp);
    return vec.Length();
}

