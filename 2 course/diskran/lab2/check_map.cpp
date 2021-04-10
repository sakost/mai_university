//
// Created by sakost on 01.12.2020.
//

#include <map>
#include <string>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

int main(){
#ifdef TIMEIT
    auto now = high_resolution_clock::now();
#endif

    string cmd;

    map<string, uint64_t> map1;

    while (std::cin >> cmd) {
        if (cmd=="+") {
            cin >> cmd;
            uint64_t val;
            cin >> val;
            transform(cmd.begin(), cmd.end(), cmd.begin(),
                           [](unsigned char c){ return tolower(c); });
            if(map1.count(cmd) != 0){
                cout << "Exist" << endl;
            }
            else{
                map1[cmd] = val;
                cout << "OK" << endl;
            }

        } else if (cmd == "-") {
            std::cin >> cmd;
            transform(cmd.begin(), cmd.end(), cmd.begin(),
                      [](unsigned char c){ return tolower(c); });

            if (map1.count(cmd)) {
                map1.erase(cmd);
                cout << "OK" << endl;
            }
            else
                cout << "NoSuchWord" << endl;

        } else {
            transform(cmd.begin(), cmd.end(), cmd.begin(),
                      [](unsigned char c){ return tolower(c); });

            if (map1.count(cmd))
                std::cout << "OK: " << map1[cmd] << std::endl;
            else
                std::cout << "NoSuchWord" << std::endl;
        }
    }

#ifdef TIMEIT
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - now);

    cerr << "Elapsed time: " << duration.count() << endl;
#endif
    return 0;
}
