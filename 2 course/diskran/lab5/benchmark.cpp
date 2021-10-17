//
// Created by k.sazhenov on 17.10.2021.
//
#include "suffixtree.h"
#include "naive.h"

#include <iostream>
#include <string>
#include <chrono>


int main(){
    std::string s;
    std::cin >> s;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    std::string ans1, ans2;

    {
        auto t1 = high_resolution_clock::now();

        ans1 = minimum_cycle_string(s);

        auto t2 = high_resolution_clock::now();

        duration<double> ms_double = t2 - t1;
        std::cout << "Naive algorithm: " << ms_double.count() << std::endl;
    }

    {
        auto t1 = high_resolution_clock::now();

        const TSuffixTree suffixTree(s+s);
        ans2 = suffixTree.FindMinimumString(s.size()+1).substr(0, s.size());

        auto t2 = high_resolution_clock::now();

        duration<double> ms_double = t2 - t1;
        std::cout << "Suffix tree: " << ms_double.count() << std::endl;
    }

    assert(ans1 == ans2);
}
