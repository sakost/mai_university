//
// Created by sakost on 25.04.2021.
//

#include "johnson.h"

#include <functional>
#include <queue>


TGraph::AdjMatrix JohnsonAlgorithm(const TGraph &graph) noexcept(false) {
    TGraph modified_graph;
    modified_graph.count_vertexes = graph.count_vertexes + 1;
    modified_graph.count_edges = graph.count_edges + graph.count_vertexes;
    modified_graph.edges = graph.edges;
    for (std::size_t i = 0; i < graph.count_vertexes; ++i) {
        modified_graph.edges.push_back(TEdge{0, graph.count_vertexes, i});
    }
    TGraph::AdjMatrix modified_weights;
    modified_weights = BellmanFord(modified_graph, graph.count_vertexes);

    TGraph::AdjMatrix modified_graph_matrix(graph.count_vertexes,
                                            std::vector<TWeight>(graph.count_vertexes, WEIGHT_MAX));
    for (std::size_t i(0); i < graph.count_vertexes; ++i) {
        modified_graph_matrix[i][i] = 0;
    }
    for (const auto &edge: graph.edges) {
        modified_graph_matrix[edge.src][edge.dest] =
                edge.weight + modified_weights[0][edge.src] - modified_weights[0][edge.dest];
    }

    TGraph::AdjMatrix result(graph.count_vertexes, std::vector<TWeight>(graph.count_vertexes, WEIGHT_MAX));

    for (std::size_t i(0); i < graph.count_vertexes; ++i) {
        Dijkstra(modified_graph_matrix, i, result);
    }

    for (std::size_t i(0); i < graph.count_vertexes; ++i) {
        for (std::size_t j(0); j < graph.count_vertexes; ++j) {
            if (result[i][j] != WEIGHT_MAX) {
                result[i][j] = result[i][j] + modified_weights[0][j] - modified_weights[0][i];
            }
        }
    }

    return result;
}

TGraph::AdjMatrix BellmanFord(const TGraph &graph, std::size_t src) noexcept(false) {
    TGraph::AdjMatrix dist(1, std::vector<TWeight>(graph.count_vertexes, WEIGHT_MAX));
    dist[0][src] = 0;

    for (std::size_t k(0); k < graph.count_vertexes - 1; ++k) {
//        bool flag = false;
        for (auto &edge : graph.edges) {
            if (dist[0][edge.src] != WEIGHT_MAX && dist[0][edge.dest] > dist[0][edge.src] + edge.weight) {
                dist[0][edge.dest] = dist[0][edge.src] + edge.weight;
//                flag = true;
            }
        }
//        if (!flag) {
//            break;
//        }
    }


    for (auto &edge : graph.edges) {
        if (dist[0][edge.src] != WEIGHT_MAX && dist[0][edge.dest] > dist[0][edge.src] + edge.weight) {
            throw TNegativeCycle();
        }
    }

    return dist;
}


void
Dijkstra(const TGraph::AdjMatrix &graph, const std::size_t &start_vertex, TGraph::AdjMatrix &dist) noexcept {
    dist[start_vertex][start_vertex] = 0;


    std::priority_queue<std::pair<TWeight, std::size_t>, std::vector<std::pair<TWeight, std::size_t>>, std::greater<>> Q;
    Q.push({0, start_vertex});

    while (!Q.empty()) {
        auto node = Q.top();
        Q.pop();

        for (std::size_t i(0); i < graph.size(); ++i) {
            if (dist[start_vertex][i] - dist[start_vertex][node.second] > graph[node.second][i]) {
                dist[start_vertex][i] = dist[start_vertex][node.second] + graph[node.second][i];
                Q.push({dist[start_vertex][i], i});
            }
        }
    }
}

