//
// Created by sakost on 09.11.2020.
//

#include "figure.h"

std::ostream &operator<<(std::ostream &out, const Figure &fig) {
    fig.print(out);
    return out;
}

std::istream &operator>>(std::istream &in, Figure &fig) {
    fig.scan(in);
    return in;
}
