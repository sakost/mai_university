/**
 * Саженов К.С.
 * М8О-208Б-19
 * https://github.com/sakost/oop_exercise_03
 * Вариант 26:
 * * Квадрат
 * * Прямоугольник
 * * Трапеция
 **/


#include "square.h"
#include "rectangle.h"
#include "trapezoid.h"

#include <iostream>
#include <vector>

void help(std::ostream& out = std::cout){
    out << "`add square`\t=> Create square\n";
    out << "`add rectangle`\t=> Create rectangle\n";
    out << "`add trapezoid`\t=> Create trapezoid\n";
    out << "`get <INDEX>`\t=> Print figure coordinates with index INDEX\n";
    out << "`print area`\t=> Print all areas\n";
    out << "`print all`\t=> Print sum of all areas\n";
    out << "`print center`\t=> Print all centers\n";
    out << "`delete <INDEX>`\t=> Delete the figure by given index INDEX\n";
    out << "`help`\t=> Prints this message\n";
    out << "`exit`\t=> Exit\n";
}

enum class Error{
    PRESENTATION_ERROR,
    INCORRECT_COORDINATES,
    INCORRECT_INDEX,
    UNDEFINED_ERROR
};

void error_occurred(Error errno_){
    switch (errno_) {
        case Error::PRESENTATION_ERROR:
            std::cout << "Presentation error or invalid command" << std::endl;
            break;
        case Error::INCORRECT_COORDINATES:
            std::cout << "Incorrect coordinates of figure" << std::endl;
            break;
        case Error::INCORRECT_INDEX:
            std::cout << "Incorrect index of figure" << std::endl;
            break;
        case Error::UNDEFINED_ERROR:
            std::cout << "Undefined error" << std::endl;
            break;
    }
}

int main() {
    help();

    std::vector<Figure*> figures;

    std::string cmd;

    while(true){
        std::cout << ">>> ";
        if(!(std::cin >> cmd)){
            break;
        }
        if(cmd == "exit"){
            break;
        } else if(cmd == "help"){
            help();
            continue;
        } else if(cmd == "add"){
            if(!(std::cin >> cmd)){
                error_occurred(Error::PRESENTATION_ERROR);
                continue;
            }
            Figure* fig;
            std::cout << "type x1 y1 x2 y2 x3 y3 x4 y4: ";
            if(cmd == "square"){
                fig = new Square;
            } else if(cmd == "rectangle"){
                fig = new Rectangle;
            } else if(cmd == "trapezoid"){
                fig = new Trapezoid;
            } else {
                error_occurred(Error::PRESENTATION_ERROR);
                continue;
            }
            try {
                std::cin >> *fig;
            } catch (std::logic_error &e) {
                std::cout << e.what() << std::endl;
                error_occurred(Error::INCORRECT_COORDINATES);
                continue;
            }
            figures.push_back(fig);
            std::cout << cmd << " successfully added to vector at index " << figures.size() - 1 << std::endl;
        } else if(cmd == "print"){
            if(!(std::cin >> cmd)){
                error_occurred(Error::PRESENTATION_ERROR);
                continue;
            }
            if (cmd == "all") {
                coord_t total_area = 0;
                for (Figure* fig : figures) {
                    total_area += fig->area();
                }
                std::cout << "Total area: " << total_area << std::endl;
            }
            else if (cmd == "area") {
                std::cout << "Areas: \n";
                for (Figure* fig : figures) {
                    std::cout << fig->area() << std::endl;
                }
            }
            else if (cmd == "center") {
                std::cout << "Centers: \n";
                for (Figure* fig : figures) {
                    std::cout << fig->center() << std::endl;
                }
            } else{
                error_occurred(Error::PRESENTATION_ERROR);
                continue;
            }
        } else if(cmd == "get") {
            size_t id;
            std::cin >> id;
            if (id >= figures.size()) {
                error_occurred(Error::INCORRECT_INDEX);
                continue;
            }
            std::cout << *figures[id] << std::endl;
        } else if(cmd == "delete"){
            size_t idx;
            if(!(std::cin >> idx)){
                error_occurred(Error::PRESENTATION_ERROR);
                continue;
            }
            if(idx >= figures.size()){
                error_occurred(Error::INCORRECT_INDEX);
                continue;
            }
            figures.erase(figures.begin() + idx);
            std::cout << "Figure was deleted" << std::endl;
        } else{
            error_occurred(Error::PRESENTATION_ERROR);
        }
    }

    std::cout << "Bye" << std::endl;

    return 0;
}
