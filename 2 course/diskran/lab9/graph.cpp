//
// Created by sakost on 20.06.2021.
//

#include "graph.h"

#include <utility>


TGraph::TGraph(std::vector<TEdge> edges, std::size_t n) : edges(std::move(edges)), count_vertexes(n) {
    count_edges = this->edges.size();

    adjMatrix.resize(count_vertexes, std::vector<TWeight>(count_vertexes, WEIGHT_MAX));

    for (const auto &edge: this->edges) {
        adjMatrix[edge.src][edge.dest] = edge.weight;
    }


    for (std::size_t i = 0; i < count_vertexes; i++) {
        for (std::size_t j = 0; j < count_vertexes; j++) {
            if (i == j) {
                adjMatrix[i][j] = 0;
            }
        }
    }
}
