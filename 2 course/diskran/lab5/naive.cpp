//
// Created by k.sazhenov on 17.10.2021.
//

#include "naive.h"


std::string minimum_cycle_string(const std::string & str){
    auto cur_min_str = str;
    const auto n = str.size();
    for(int i(0); i < str.size(); ++i){
        std::string cur_str = str.substr(i, n-i) + str.substr(0, i);
        if(std::lexicographical_compare(cur_str.begin(),  cur_str.end(), cur_min_str.begin(),  cur_min_str.end())) cur_min_str = cur_str;
    }

    return cur_min_str;
}
