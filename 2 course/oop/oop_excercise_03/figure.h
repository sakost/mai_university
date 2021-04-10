//
// Created by sakost on 09.11.2020.
//

#ifndef OOP_EXCERCISE_03_FIGURE_H
#define OOP_EXCERCISE_03_FIGURE_H

#include "customvector.h"

class Figure {
public:
    virtual ~Figure() = default;

    [[nodiscard]] virtual Point center() const = 0;
    [[nodiscard]] virtual coord_t area() const = 0;


    friend std::ostream& operator<< (std::ostream& out, const Figure& fig);
    friend std::istream& operator>> (std::istream& in, Figure& fig);

protected:
    virtual std::ostream& print(std::ostream& out) const = 0;
    virtual std::istream& scan(std::istream& in) = 0;
};



#endif //OOP_EXCERCISE_03_FIGURE_H
