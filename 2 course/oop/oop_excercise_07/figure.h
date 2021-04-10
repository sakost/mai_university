//
// Created by sakost on 17.12.2020.
//

#ifndef OOP_EXCERCISE_07_FIGURE_H
#define OOP_EXCERCISE_07_FIGURE_H

#include <string>
#include <any>
#include <set>
#include <vector>

using coord = std::pair<std::int64_t, std::int64_t>;

class invalid_property_name : std::exception{};

class Figure {
public:
    using coord_type = long double;

    static const std::string name;

    virtual ~Figure() = default;

    [[nodiscard]] virtual const std::set<std::string>& getPropsNames() const{
        return m_props;
    };
    [[nodiscard]] virtual std::any getProp(const std::string&) const {
        throw invalid_property_name();
    };

    [[nodiscard]] virtual std::string get_raw_data() const = 0;
    virtual void set_raw_data(const std::string&) = 0;

protected:
    std::set<std::string> m_props;
};

const std::string Figure::name{""};


#endif //OOP_EXCERCISE_07_FIGURE_H
