//
// Created by sakost on 08.11.2020.
//

#ifndef OOP_EXCERCISE_03_CUSTOMVECTOR_H
#define OOP_EXCERCISE_03_CUSTOMVECTOR_H

#include "point.h"

class CustomVector {
public:
    CustomVector(coord_t x_, coord_t y_);
    CustomVector(const Point& from, const Point& to);

    coord_t operator*(const CustomVector& rhs) const;
    CustomVector operator+(const CustomVector& other) const;
    CustomVector operator-() const;
    CustomVector operator-(const CustomVector& other) const;
    CustomVector& operator=(const CustomVector& rhs);

    [[nodiscard]] coord_t X() const;
    [[nodiscard]] coord_t Y() const;

    [[nodiscard]] coord_t Length() const;
private:
    coord_t x, y;
};

[[nodiscard]] coord_t dot(const CustomVector& lhs, const CustomVector& rhs);
[[nodiscard]] bool isParallel(const CustomVector& lhs, const CustomVector& rhs);

[[nodiscard]] coord_t distance(const Point& l1, const Point& l2, const Point& p);


#endif //OOP_EXCERCISE_03_CUSTOMVECTOR_H
