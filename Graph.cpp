//
// Created by kiril on 28.05.2023.
//

#include "Graph.h"

void Graph::setAdjacencyMatrix(const std::vector<std::vector<int>> & adjacencyMatrix) {
    adjacencyMatrix_ = adjacencyMatrix;
    amountNodes_ = adjacencyMatrix_.size();
}
