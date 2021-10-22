//
// Created by k.sazhenov on 16.10.2021.
//

#include "floyd_warshall.h"

TGraph::AdjMatrix FloydWarshall(const TGraph& graph) noexcept(false){
    auto d = graph.adjMatrix;
    for(std::size_t i(0); i < d.size(); ++i){
        for(std::size_t u(0); u < d.size(); ++u){
            for(std::size_t v(0); v < d.size(); ++v){
                if(d[u][i] < WEIGHT_MAX && d[i][v] < WEIGHT_MAX){
                    d[u][v] = std::min(d[u][v], d[u][i] + d[i][v]);
                }
            }
        }
    }

    for(std::size_t i(0); i < d.size(); ++i){
        if(d[i][i] < 0){
            throw TNegativeCycle();
        }
    }

    return d;
}
