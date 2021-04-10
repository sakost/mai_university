/**
 * Саженов К.С.
 * М8О-208Б-19
 * https://github.com/sakost/oop_excercise_04
 * Вариант 26:
 * * Квадрат
 * * Прямоугольник
 * * Трапеция
 **/

#include <iostream>
#include <type_traits>
#include <cmath>
#include <tuple>

#include "square.h"
#include "rectangle.h"
#include "trapezoid.h"

using coord_t = double;

template<typename T, class R>
typename std::enable_if<std::is_same<T, Rectangle<R>&>::value, bool>::type
readFigure(T figure) {
    size_t n = sizeof(figure.points) / sizeof(figure.points[0]);
    for (int i = 0; i < n; ++i) {
        std::cin >> figure.points[i].first >> figure.points[i].second;
    }
    coord_t ab = (figure.points[1].first - figure.points[0].first) * (figure.points[1].first - figure.points[0].first) +
            (figure.points[1].second - figure.points[0].second) * (figure.points[1].second - figure.points[0].second);
    coord_t bc = (figure.points[2].first - figure.points[1].first) * (figure.points[2].first - figure.points[1].first) +
            (figure.points[2].second - figure.points[1].second) * (figure.points[2].second - figure.points[1].second);
    coord_t ac = (figure.points[2].first - figure.points[0].first) * (figure.points[2].first - figure.points[0].first) +
                 (figure.points[2].second - figure.points[0].second) * (figure.points[2].second - figure.points[0].second);
    coord_t cd = (figure.points[3].first - figure.points[2].first) * (figure.points[3].first - figure.points[2].first) +
                 (figure.points[3].second - figure.points[2].second) * (figure.points[3].second - figure.points[2].second);
    coord_t da = (figure.points[0].first - figure.points[3].first) * (figure.points[0].first - figure.points[3].first) +
                 (figure.points[0].second - figure.points[3].second) * (figure.points[0].second - figure.points[3].second);

    bool error = false;
    if(da != bc || ab != cd){
        error = true;
    }
    if(ac != ab + bc){
        error = true;
    }
    if(error){
        for (int i = 0; i < n; ++i) {
            figure.points[i].first = figure.points[i].second = 0;
        }
        std::cout << "Invalid coordinates" << std::endl;
        return false;
    }
    return true;
}

template<typename T, class R>
typename std::enable_if<std::is_same<T, Square<R>&>::value, bool>::type
readFigure(T figure) {
    size_t n = sizeof(figure.points) / sizeof(figure.points[0]);
    for (int i = 0; i < n; ++i) {
        std::cin >> figure.points[i].first >> figure.points[i].second;
    }
    coord_t ab = (figure.points[1].first - figure.points[0].first) * (figure.points[1].first - figure.points[0].first) +
                 (figure.points[1].second - figure.points[0].second) * (figure.points[1].second - figure.points[0].second);
    coord_t bc = (figure.points[2].first - figure.points[1].first) * (figure.points[2].first - figure.points[1].first) +
                 (figure.points[2].second - figure.points[1].second) * (figure.points[2].second - figure.points[1].second);
    coord_t ac = (figure.points[2].first - figure.points[0].first) * (figure.points[2].first - figure.points[0].first) +
                 (figure.points[2].second - figure.points[0].second) * (figure.points[2].second - figure.points[0].second);
    coord_t cd = (figure.points[3].first - figure.points[2].first) * (figure.points[3].first - figure.points[2].first) +
                 (figure.points[3].second - figure.points[2].second) * (figure.points[3].second - figure.points[2].second);
    coord_t da = (figure.points[0].first - figure.points[3].first) * (figure.points[0].first - figure.points[3].first) +
                 (figure.points[0].second - figure.points[3].second) * (figure.points[0].second - figure.points[3].second);

    bool error = false;
    if(da != bc || ab != cd || ab != bc){
        error = true;
    }
    if(ac != ab + bc){
        error = true;
    }
    if(error){
        for (int i = 0; i < n; ++i) {
            figure.points[i].first = figure.points[i].second = 0;
        }
        std::cout << "Invalid coordinates" << std::endl;
        return false;
    }
    return true;
}

template <typename T>
T dot(const std::pair<T, T>& p1, const std::pair<T, T>& p2){
    return p1.second * p2.first - p1.first * p2.second;
}

template<typename T, class R>
typename std::enable_if<std::is_same<T, Trapezoid<R>&>::value, bool>::type
readFigure(T figure) {
    size_t n = sizeof(figure.points) / sizeof(figure.points[0]);
    for (int i = 0; i < n; ++i) {
        std::cin >> figure.points[i].first >> figure.points[i].second;
    }
    R dot_prod1 = dot(figure.points[0], figure.points[2]);
    R dot_prod2 = dot(figure.points[1], figure.points[3]);

    bool error = false;

    if(!(dot_prod1 < std::numeric_limits<R>::epsilon() || dot_prod2 < std::numeric_limits<R>::epsilon())){
        error = true;
    }

    if(error){
        for (int i = 0; i < n; ++i) {
            figure.points[i].first = figure.points[i].second = 0;
        }
        std::cout << "Invalid coordinates" << std::endl;
        return false;
    }
    return true;
}

template<typename T, size_t index>
typename std::enable_if<index == std::tuple_size<T>::value, void>::type
printTuple(T &) {
}

template<typename T, size_t index>
typename std::enable_if<index < std::tuple_size<T>::value, void>::type
printTuple(T &t) {
    auto figure = std::get<index>(t);
    std::cout << "Vertices of " << figure.name << ":\t";
    printVertices(figure);
    printTuple<T, index + 1>(t);
}

template<typename T>
typename std::enable_if<(sizeof(T::points) / sizeof(T::points[0]) > 0), void>::type
printVertices(T &figure) {
    for (auto v : figure.points)
        std::cout << " ( " << v.first << ", " << v.second << " ) ";
    std::cout << std::endl;
}


template<class T, size_t index>
coord_t totalTupleArea(T &tuple) {
    auto figure = std::get<index>(tuple);
    coord_t value = figureArea(figure);
    if constexpr ((index + 1) < std::tuple_size<T>::value) {
        return value + totalTupleArea<T, index + 1>(tuple);
    }
    return value;
}

template<class T>
auto figureArea(T &figure) {
    coord_t area = 0;
    int n = sizeof(T::points) / sizeof(T::points[0]);
    for (size_t i = 1; i < n; ++i)
        area += figure.points[i - 1].first * figure.points[i].second -
                figure.points[i].first * figure.points[i - 1].second;
    area += figure.points[n - 1].first * figure.points[0].second - figure.points[0].first * figure.points[n - 1].second;
    return std::fabs(area) / 2.0;
}

void help() {
    std::cout << "1 \t--> Add Square\n";
    std::cout << "2 \t--> Add Rectangle\n";
    std::cout << "3 \t--> Add Trapezoid\n";
    std::cout << "4 \t--> Display all the vertices of tuple's figures\n";
    std::cout << "5 \t--> Display total area of tuple's figures\n";
    std::cout << "6 \t--> Help(this message)\n";
    std::cout << "7 \t--> Exit\n";
}


int main() {
    help();

    std::tuple<Square<double>, Rectangle<double>, Trapezoid<double>> tp;

    int cmd;
    bool run(true);
    while (run) {
        std::cout << ">>> ";
        if (!(std::cin >> cmd)) {
            break;
        }
        switch (cmd) {
            case 1: // add square
            {
                auto &fig = std::get<0>(tp);
                if(readFigure<decltype(fig), coord_t>(fig))
                    std::cout << std::get<0>(tp).name << " successfully added" << std::endl;
                break;
            }
            case 2: // add rectangle
            {
                auto &fig = std::get<1>(tp);
                if(readFigure<decltype(fig), coord_t>(fig))
                    std::cout << std::get<1>(tp).name << " successfully added" << std::endl;
                break;
            }
            case 3: // add trapezoid
            {
                auto &fig = std::get<2>(tp);
                if(readFigure<decltype(fig), coord_t>(fig))
                    std::cout << std::get<2>(tp).name << " successfully added" << std::endl;
                break;
            }
            case 4: { // all vertices
                printTuple<decltype(tp), 0>(tp);
                break;
            }
            case 5: { // total area
                std::cout << "Total area of tuple's figures: " << totalTupleArea<decltype(tp), 0>(tp) << std::endl;
                break;
            }
            case 6: { // print help message
                help();
                break;
            }
            case 7: { // exit
                run = false;
                break;
            }
            default: {
                std::cout << "Unknown command. Print `help` to get list of commands" << std::endl;
            }
        }
    }

    std::cout << "Bye" << std::endl;
    return 0;
}
