//
// Created by sakost on 17.12.2020.
//

#ifndef OOP_EXCERCISE_07_CONCRETEFIGURES_H
#define OOP_EXCERCISE_07_CONCRETEFIGURES_H

#include "figure.h"

#include <vector>
#include <set>
#include <sstream>
#include <cmath>
#include <cassert>


Figure::coord_type distance(const coord& lhs, const coord& rhs){
    return std::sqrt(std::pow(lhs.first - rhs.first, 2) + std::pow(lhs.second - rhs.second, 2));
}

class Triangle : public Figure{
public:
    static const std::string name;

    Triangle(const coord& a, const coord& b, const coord& c){
        Initialize(a, b, c);
    }

    Triangle() : Triangle({0,0}, {0, 1}, {1, 0}){}

    void Initialize(const coord& a, const coord& b, const coord& c){
        coords[0] = a;
        coords[1] = b;
        coords[2] = c;
    }

    [[nodiscard]] std::string get_raw_data() const override {
        std::stringstream ss;
        ss << coords.size() << ' ';
        for (const auto &item : coords) {
            ss << item.first << ' ' << item.second << ' ';
        }
        return ss.str();
    }


    [[nodiscard]] const std::set<std::string> &getPropsNames() const override {
        return m_props;
    }

    [[nodiscard]] std::any getProp(const std::string &prop) const override {
        if(prop == "area"){
            return area();
        }
        if(prop == "name"){
            return name;
        }

        return Figure::getProp(prop);
    }

    [[nodiscard]] coord_type area() const{
        coord_type total(0);
        for (std::size_t i = 1; i < coords.size(); ++i) {
            total += coords[i-1].first * coords[i].second - coords[i].first * coords[i-1].second;
        }
        total += coords.back().first * coords.front().second - coords.front().first * coords.back().second;
        return std::abs(total) / 2.0;
    }

protected:
    void set_raw_data(const std::string &data) override {
        std::stringstream ss(data);
        std::size_t len;
        ss >> len;
        assert(len == coords.size());
        for (auto &item : coords) {
            ss >> item.first >> item.second;
        }
    }

    std::set<std::string> m_props = {"area"};
    std::vector<coord> coords{3};
};

const std::string Triangle::name = "triangle";


class Rectangle : public Figure {
public:
    static const std::string name;

    Rectangle(const coord &a, const coord &b, const coord &c, const coord &d) {
        Initialize(a, b, c, d);
    }

    Rectangle(const coord_type &first_side_len, const coord_type &second_side_len, const coord left_bottom = {0, 0}){
        Initialize(first_side_len, second_side_len, left_bottom);
    }

    Rectangle() : Rectangle(1, 1) {};

    void Initialize(const coord_type &first_side_len, const coord_type &second_side_len, const coord left_bottom = {0, 0}) {
        coords[0].first = left_bottom.first;
        coords[0].second = left_bottom.second;

        coords[1].first = left_bottom.first;
        coords[1].second = left_bottom.second + second_side_len;

        coords[2].first = left_bottom.first + first_side_len;
        coords[2].second = left_bottom.second + second_side_len;

        coords[3].first = left_bottom.first + first_side_len;
        coords[3].second = left_bottom.second;
    }

    void Initialize(const coord &a, const coord &b, const coord &c, const coord &d) {
        coords[0] = a;
        coords[1] = b;
        coords[2] = c;
        coords[3] = d;
    }

    [[nodiscard]] const std::set<std::string> &getPropsNames() const override {
        return m_props;
    }

    [[nodiscard]] std::any getProp(const std::string &prop) const override {
        if (prop == "area") {
            return area();
        }
        if (prop == "first_side") {
            return first_side();
        }
        if (prop == "second_side") {
            return second_side();
        }
        if(prop == "name"){
            return name;
        }

        return Figure::getProp(prop);
    }

    [[nodiscard]] std::string get_raw_data() const override {
        std::stringstream ss;
        ss << coords.size() << ' ';
        for (const auto &item : coords) {
            ss << item.first << ' ' << item.second << ' ';
        }
        return ss.str();
    }


    [[nodiscard]] coord_type area() const {
        coord_type dx1 = coords[3].first - coords[0].first;
        coord_type dy1 = coords[3].second - coords[0].second;
        coord_type dx2 = coords[1].first - coords[0].first;
        coord_type dy2 = coords[1].second - coords[0].second;
        return std::abs(dx1 * dy2 - dy1 * dx2);
    }

    [[nodiscard]] coord_type first_side() const {
        return std::sqrt(std::pow(coords[3].first - coords[0].first, 2) +
                         std::pow(coords[3].second - coords[0].second, 2));
    }

    [[nodiscard]] coord_type second_side() const {
        return std::sqrt(std::pow(coords[1].first - coords[0].first, 2) +
                         std::pow(coords[1].second - coords[0].second, 2));
    }

    bool isTrueRectangle(){
        return std::abs(distance(coords[0], coords[1]) - distance(coords[2], coords[3])) <
               std::numeric_limits<coord_type>::epsilon() &&
               std::abs(distance(coords[1], coords[2])) - distance(coords[0], coords[3]) <
               std::numeric_limits<coord_type>::epsilon() &&
               std::abs(area() - first_side() * second_side()) < std::numeric_limits<coord_type>::epsilon();
    }

protected:
    void set_raw_data(const std::string &data) override {
        std::stringstream ss(data);
        std::size_t len;
        ss >> len;
        assert(len == coords.size());
        for (auto &item : coords) {
            ss >> item.first >> item.second;
        }
    }

    std::set<std::string> m_props = {"area", "first_side", "second_side", "name"};
    std::vector<coord> coords{4};
};

const std::string Rectangle::name = "rectangle";


class Square : public Rectangle {
public:
    static const std::string name;

    explicit Square(const coord_type &side_len = 1, const coord left_bottom = {0, 0}) :
            Rectangle(side_len, side_len, left_bottom) {}

    void Initialize(const coord_type &side_len = 1, const coord left_bottom = {0, 0}){
        this->Rectangle::Initialize(side_len, side_len, left_bottom);
    }

    [[nodiscard]] coord_type side() const {
        return Rectangle::first_side();
    }

    [[nodiscard]] std::any getProp(const std::string &prop) const override {
        if (prop == "side") {
            return side();
        }
        if(prop == "name"){
            return name;
        }

        return Rectangle::getProp(prop);
    }

    bool isTrueSquare(){
        return this->isTrueRectangle() && std::abs(distance(coords[1], coords[2])) - distance(coords[2], coords[3]) <
                                          std::numeric_limits<coord_type>::epsilon();;
    }


protected:
    std::set<std::string> m_props = {"area", "side", "name"};
};

const std::string Square::name = "square";


#endif //OOP_EXCERCISE_07_CONCRETEFIGURES_H
