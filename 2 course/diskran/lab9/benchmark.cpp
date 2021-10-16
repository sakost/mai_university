//
// Created by k.sazhenov on 16.10.2021.
//

#include <chrono>
#include <iostream>

#include "floyd_warshall.h"
#include "johnson.h"

int main(){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    std::size_t n, m;
    std::cin >> n >> m;
    std::vector<TEdge> edges(m);

    TGraph graph(edges, n);

    TGraph::AdjMatrix ans_matrix;

    for(auto& edge: edges){
        std::cin >> edge.src;edge.src--;
        std::cin >> edge.dest;edge.dest--;
        std::cin >> edge.weight;
    }

    {
        auto t1 = high_resolution_clock::now();

        try {
            JohnsonAlgorithm(graph);
        }catch (TNegativeCycle&){}

        auto t2 = high_resolution_clock::now();

        duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "Johnson algorithm: " << ms_double.count() << std::endl;
    }

    {
        auto t1 = high_resolution_clock::now();

        try {
            FloydWarshall(graph);
        } catch (TNegativeCycle&) {}

        auto t2 = high_resolution_clock::now();

        duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "Naive algorithm: " << ms_double.count() << std::endl;
    }


    return 0;
}

