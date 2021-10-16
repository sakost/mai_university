//
// Created by sakost on 25.04.2021.
//

#ifndef LAB9_JOHNSON_H
#define LAB9_JOHNSON_H

#include "graph.h"

#include <vector>
#include <limits>

TGraph::AdjMatrix JohnsonAlgorithm(const TGraph& graph) noexcept(false);
TGraph::AdjMatrix BellmanFord(const TGraph& graph, std::size_t src) noexcept(false);
void
Dijkstra(const TGraph::AdjMatrix &graph, const std::size_t &start_vertex, TGraph::AdjMatrix &dist) noexcept;

#endif //LAB9_JOHNSON_H
