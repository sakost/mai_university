//
// Created by sakost on 08.11.2020.
//

#include "point.h"

#include <limits>

Point::Point() : x(0), y(0){}
Point::Point(coord_t a, coord_t b) : x(a), y(b){}
Point::Point(const Point &other) : x(other.x), y(other.y){}

coord_t Point::X() const{
    return x;
}

coord_t Point::Y() const{
    return y;
}

Point Point::operator+ (const Point& a) const {
    return { this->x + a.x, this->y + a.y };
}

Point Point::operator- (const Point& a) const {
    return { this->x - a.x, this->y - a.y };
}

Point Point::operator* (coord_t a) const {
    return { this->x * a, this->y * a };
}

Point Point::operator/ (coord_t a) const {
    if(a == 0){
        throw std::logic_error("the divider must be non-zero");
    }
    return { this->x / a, this->y / a };
}

std::ostream& operator<< (std::ostream& out, const Point& a) {
    out << "Point( " << a.x << ", " << a.y << " )";
    return out;
}

std::istream& operator>> (std::istream& in, Point& a) {
    in >> a.x >> a.y;
    return in;
}

bool Point::operator==(const Point &other) const {
    return (this->x - other.x <= std::numeric_limits<coord_t>::epsilon()) &&
        (this->y - other.y <= std::numeric_limits<coord_t>::epsilon());
}
