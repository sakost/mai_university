/**
 * Саженов К.С.
 * М8О-208Б-19
 * https://github.com/sakost/oop_excercise_07
 *
 * Вариант 25
 * Треугольник, квадрат, прямоугольник
 **/

#include "editor.h"
#include "concretefactories.h"
#include "concretefigures.h"

#include <iostream>
#include <fstream>
#include <algorithm>


std::ostream& help(std::ostream& out){
    out << "exit\t-\texit from program\n";
    out << "add\t-\tpush back a figure\n";
    out << "pop\t-\tpop back a figure from document\n";
    out << "undo\t-\tundo last operations\n";
    out << "export\t-\texport document to the file\n";
    out << "import\t-\timport document from the file\n";
    out << "new\t-\tcreate new document\n";
    out << "print\t-\tprint all figures\n";
    out << "area\t-\tprint area of last figure\n";
    return out;
}

int main() {
    using namespace std;
    using coord_type = Figure::coord_type;

    Editor editor;

    SquareFactory s_factory;
    RectangleFactory r_factory;
    TriangleFactory t_factory;

    string cmd;

    while((cout << "> " << flush) && getline(cin, cmd)){
        if(cmd == "exit") break;
        if(cmd == "add"){
            cout << "Input the name of figure(triangle, rect or square): " << flush;
            if(!(cin >> cmd)){
                cerr << "Invalid input" << endl;
                break;
            }
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

                auto r = dynamic_cast<Rectangle*>(r_factory.create_object());
                r->Initialize(first_side, second_side);
                if(!r->isTrueRectangle()){
                    delete r;
                    cout << "Invalid input:\ncoordinates does not form a rectangle" << endl;
                    continue;
                }
                editor.add_figure(r);
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
                auto s = dynamic_cast<Square*>(s_factory.create_object());
                s->Initialize(side);
                if(!s->isTrueSquare()){
                    delete s;
                    cout << "Invalid input:\ncoordinates does not form a square" << endl;
                    continue;
                }
                editor.add_figure(s);
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
                auto t = dynamic_cast<Triangle*>(t_factory.create_object());
                t->Initialize(a, b, c);
                editor.add_figure(t);
                cout << "Triangle successfully added" << endl;
            }
            else
            {
                throw runtime_error("something went wrong...");
            }
        }
        else if(cmd == "pop"){
            if(editor.get_figures().empty()){
                cout << "Document is empty" << endl;
                continue;
            }
            editor.pop_figure();
            cout << "Ok" << endl;
        }
        else if(cmd == "undo"){
            if(editor.undo()){
                cout << "Ok" << endl;
            }else{
                cout << "Oops..Couldn't undo" << endl;
            }
        }
        else if(cmd == "export"){
            cout << "Input the name of file which export to: " << flush;
            string filename;
            if(!(cin >> filename)){
                cout << "Invalid filename" << endl;
                continue;
            }
            ofstream fs(filename);
            if(!fs) {
                cout << "Invalid file or path" << endl;
                continue;
            }
            EditorSerializer::serialize(editor, fs);
            cout << "Document was successfully  added to file " << filename << endl;
        }
        else if(cmd == "import"){
            cout << "Input the name of file which import from: " << flush;
            string filename;
            if(!(cin >> filename)){
                cout << "Invalid filename" << endl;
                continue;
            }
            ifstream fs(filename);
            if(!fs) {
                cout << "Invalid file or path" << endl;
                continue;
            }
            if(!editor.get_figures().empty()){
                cout << "Are you sure? This document will be overwritten[Y/n]: " << flush;
                string ans;
                getline(cin, ans);
                for_each(ans.begin(), ans.end(), [](auto &el){
                    el = ::tolower(el);
                });
                if(!ans.empty() && ans != "y"){
                    cout << "Ok, going to main menu..." << endl;
                    continue;
                }
                cout << "Ok removing..." << endl;
            }
            editor = EditorLoader::load(fs);
        }
        else if(cmd == "new"){
            if(!editor.get_figures().empty()){
                cout << "Are you sure? This document will be overwritten[Y/n]: " << flush;
                string ans;
                getline(cin, ans);
                for_each(ans.begin(), ans.end(), [](auto &el){
                    el = ::tolower(el);
                });
                if(!ans.empty() && ans != "y"){
                    cout << "Ok, going to main menu..." << endl;
                    continue;
                }
                cout << "Ok removing..." << endl;
            }
            editor = Editor();
            cout << "Created new document(and old was deleted if it was existed)" << endl;
        }
        else if(cmd == "print"){
            if(editor.get_figures().empty()){
                cout << "Nothing to show!" << endl;
                continue;
            }
            for(const auto &item: editor.get_figures()){
                cout << any_cast<string>(item->getProp("name")) << ": " << item->get_raw_data() << endl;
            }
        }
        else if(cmd == "help"){
            help(cout);
        }
        else if(cmd == "area"){
            if(editor.get_figures().empty()){
                cout << "Nothing to show!" << endl;
                continue;
            }
            for(const auto &item: editor.get_figures()) {
                cout << "Area of " << any_cast<string>(item->getProp("name")) << " is " <<
                     any_cast<coord_type>(item->getProp("area")) << endl;
            }
        }
    }
    cout << endl;

    return 0;
}
