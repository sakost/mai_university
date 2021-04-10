//
// Created by sakost on 28.09.2020.
//

#ifndef OOP_EXCERCISE_02_TRANSNUMBER_H
#define OOP_EXCERCISE_02_TRANSNUMBER_H

#include <iostream>
#include <cmath>

const long double C = 2.l;

/**
 * Class implements transcendent number with type T
 * trans part is `trans` * 2
 **/
template<typename T>
class TransNumber {
public:
    TransNumber(T real_, T trans_); // explicit constructor
    TransNumber(); // default constructor

    T getValue() const;

    // arithmetical operators
    TransNumber operator+(const TransNumber &other);

    TransNumber operator-(const TransNumber &other);

    TransNumber operator*(const TransNumber &other);

    TransNumber operator/(const TransNumber &other);

    // compare operations
    bool operator<(const TransNumber<T> &other);

    bool operator==(const TransNumber<T> &other);

    bool operator<=(const TransNumber<T> &other);

    bool operator>(const TransNumber<T> &other);

    bool operator>=(const TransNumber<T> &other);

    bool operator!=(const TransNumber<T> &other);

    template<typename J>
    friend std::ostream &operator<<(std::ostream &os, const TransNumber<J> &transNumber); // pretty printing

private:
    T real, trans;
};


template<typename T>
TransNumber<T>::TransNumber() : TransNumber(0, 0) {}

template<typename T>
TransNumber<T>::TransNumber(T real_, T trans_) {
    real = real_;
    trans = trans_;
}

template<typename T>
TransNumber<T> TransNumber<T>::operator+(const TransNumber &other) {
    return TransNumber<T>(real + other.real, trans + other.trans);
}

template<typename T>
TransNumber<T> TransNumber<T>::operator-(const TransNumber &other) {
    return TransNumber<T>(real - other.real, trans - other.trans);
}

template<typename T>
TransNumber<T> TransNumber<T>::operator*(const TransNumber &other) {
    return TransNumber<T>(
            real * other.real + trans * other.trans * C,
            real * other.trans + other.real * trans
    );
}

template<typename T>
TransNumber<T> TransNumber<T>::operator/(const TransNumber &other) {
    return TransNumber<T>(
            (real * other.real - C * trans * other.trans) / (other.real * other.real - C * other.trans * other.trans),
            (trans * other.real - real * other.trans)/(other.real * other.real - C * other.trans * other.trans)
    );
}

template<typename T>
T TransNumber<T>::getValue() const {
    return real + trans * std::sqrt(C);
}

template<typename T>
bool TransNumber<T>::operator<(const TransNumber<T> &other) {
    return getValue() < other.getValue();
}

template<typename T>
bool TransNumber<T>::operator==(const TransNumber<T> &other) {
    return getValue() == other.getValue();
}

template<typename T>
bool TransNumber<T>::operator<=(const TransNumber<T> &other) {
    return getValue() <= other.getValue();
}

template<typename T>
bool TransNumber<T>::operator>(const TransNumber<T> &other) {
    return getValue() > other.getValue();
}

template<typename T>
bool TransNumber<T>::operator>=(const TransNumber<T> &other) {
    return getValue() >= other.getValue();
}

template<typename T>
bool TransNumber<T>::operator!=(const TransNumber<T> &other) {
    return getValue() != other.getValue();
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const TransNumber<T> &transNumber) {
    os << transNumber.real << " + " << transNumber.trans << "*sqrt(2)";
    return os;;
}



#endif //OOP_EXCERCISE_02_TRANSNUMBER_H
