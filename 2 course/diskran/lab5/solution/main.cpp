#include "suffixtree.h"

#include <iostream>
#include <string>

int main() {
    std::string s;
    std::cin >> s;

    const TSuffixTree suffixTree(s+s);

    std::cout << suffixTree.FindMinimumString(s.size()+1).substr(0, s.size()) << std::endl;
//    std::cerr << s+s << std::endl;
//    std::cerr << suffixTree;

    return EXIT_SUCCESS;
}
