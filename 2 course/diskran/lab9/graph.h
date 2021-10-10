//
// Created by sakost on 20.06.2021.
//

#ifndef LAB9_GRAPH_H
#define LAB9_GRAPH_H

#include <cctype>
#include <vector>
#include <limits>


using TWeight = long long;
const TWeight WEIGHT_MAX = std::numeric_limits<TWeight>::max();

struct TEdge{
    TWeight weight{};
    std::size_t src{}, dest{};
};

class TGraph{
public:
//    using AdjList = std::vector< std::vector< std::pair<std::size_t, TWeight> > >;
    using AdjMatrix = std::vector<std::vector<TWeight>>;

    AdjMatrix adjMatrix;

    std::vector<TEdge> edges;
    TGraph() = default;
    TGraph(std::vector<TEdge> edges, std::size_t n);
    std::size_t count_vertexes{}, count_edges{};
};


#endif //LAB9_GRAPH_H
