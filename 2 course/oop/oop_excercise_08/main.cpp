/**
 * Саженов К.С.
 * М8О-208Б-19
 * https://github.com/sakost/oop_excercise_08
 *
 * Вариант 25
 * Треугольник, квадрат, прямоугольник
 **/

#include "concretefigures.h"
#include "handler.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>


std::ostream& help(std::ostream& out){
    out << "triangle\t-\tto add a triangle" << std::endl;
    out << "square\t-\tto add a square" << std::endl;
    out << "rect\t-\tto add a rectangle" << std::endl;
    return out;
}

int main(int argc, char **argv) {
    using namespace std;
    using coord_type = Figure::coord_type;

    size_t buf_size = 10;

    if(argc < 2) {
        cout << "default buf size will be used(" << buf_size << ")" << endl;
    }
    else{
        stringstream ss(argv[1]);
        if(!(ss >> buf_size)){
            cout << "Second argument must be an integer" << endl;
            return EXIT_FAILURE;
        }
    }

    Handler<Figure*> handler(buf_size);
    handler.register_handler([](const vector<Figure*> &figs){
        for(const auto& fig: figs) {
            cout << "name: " << any_cast<string>(fig->getProp("name")) << " area: " <<
                 any_cast<coord_type>(fig->getProp("area")) << " raw_data: " << fig->get_raw_data() << endl;
        }
    });
    handler.register_handler([](const vector<Figure*> &figs){
        auto ms = chrono::duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch()).count();
        string filename = to_string(ms);
        ofstream out(filename + ".txt");
        for(const auto& fig: figs){
            out << "name: " << any_cast<string>(fig->getProp("name")) << " area: " <<
                any_cast<coord_type>(fig->getProp("area")) << " raw_data: " << fig->get_raw_data() << endl;
        }
        out.close();
    });

    handler.start();

    string cmd;

    while((cout << "> " << flush) && getline(cin, cmd)){
        if(cmd.empty()) continue;
        if(cmd != "triangle" && cmd != "rect" && cmd != "square"){
            cout << "Unknown figure\n";
            cout << "All valid figures: triangle, rect and square" << endl;
            continue;
        }

        if(cmd == "rect")
        {
            cout << "Input size of first side: " << flush;
            coord_type first_side;
            if(!(cin >> first_side)){
                cerr << "Invalid input" << endl;
                break;
            }
            cout << "Input size of second side: " << flush;
            coord_type second_side;
            if(!(cin >> second_side)){
                cerr << "Invalid input" << endl;
                break;
            }

            handler.push(new Rectangle(first_side, second_side));

            cout << "Rectangle successfully added" << endl;
        }
        else if(cmd == "square")
        {
            cout << "Input size of side: " << flush;
            coord_type side;
            if(!(cin >> side)){
                cerr << "Invalid input" << endl;
                break;
            }
            handler.push(new Square(side));
            cout << "Square successfully added" << endl;
        }
        else if(cmd == "triangle")
        {
            cout << "Input coordinates of three vertexes:" << flush;
            coord a, b, c;
            cout << " first: " << flush;
            cin >> a.first >> a.second;
            cout << " second: " << flush;
            cin >> b.first >> b.second;
            cout << " third: " << flush;
            cin >> c.first >> c.second;

            handler.push(new Triangle(a, b, c));
            cout << "Triangle successfully added" << endl;
        }
        else
        {
            throw runtime_error("something went wrong...");
        }
    }
    cout << endl;

    handler.stop();

    return 0;
}
