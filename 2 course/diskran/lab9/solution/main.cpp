#include "johnson.h"

#include <iostream>

int main() {
    std::size_t n, m;
    std::cin >> n >> m;
    std::vector<TEdge> edges(m);

    for(auto& edge: edges){
        std::cin >> edge.src;edge.src--;
        std::cin >> edge.dest;edge.dest--;
        std::cin >> edge.weight;
    }

    TGraph graph(edges, n);

    TGraph::AdjMatrix ans_matrix;

    try {
        ans_matrix = JohnsonAlgorithm(graph);
    }catch (TNegativeCycle& err){
        std::cout << err.what() << std::endl;
        return 0;
    }

    for(const auto& row: ans_matrix){
        for(const auto& col: row){
            if(col == WEIGHT_MAX){
                std::cout << "inf";
            } else{
                std::cout << col;
            }
            std::cout << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}
