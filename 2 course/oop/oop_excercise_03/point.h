//
// Created by sakost on 08.11.2020.
//

#ifndef OOP_EXCERCISE_03_POINT_H
#define OOP_EXCERCISE_03_POINT_H

#include <iostream>

using coord_t = double;

class Point {
public:
    Point();
    Point(coord_t a, coord_t b);
    Point(const Point& other);

    [[nodiscard]] coord_t X() const;
    [[nodiscard]] coord_t Y() const;

    Point operator+ (const Point& a) const;
    Point operator- (const Point& a) const;
    Point operator* (coord_t a) const;
    Point operator/ (coord_t a) const;

    friend std::ostream& operator<< (std::ostream& out, const Point& a);
    friend std::istream& operator>> (std::istream& in, Point& a);

    bool operator==(const Point& other) const;


private:
    coord_t x, y;
};


#endif //OOP_EXCERCISE_03_POINT_H
