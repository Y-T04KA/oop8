//
// Created by kiril on 28.05.2023.
//

#ifndef OOP8_GRAPH_H
#define OOP8_GRAPH_H
#include <vector>

class Graph {
    std::vector<std::vector<int>> adjacencyMatrix_;
    int activeNode_;
    int amountNodes_;
public:
    Graph() = default;
    void setAdjacencyMatrix(const std::vector<std::vector<int>>&);
    std::vector<std::vector<int>> getAdjacencyMatrix() const{return adjacencyMatrix_;}
    void setActiveNode(const int value){activeNode_ = value;};
    int getActiveNode() const {return activeNode_;};
    int getAmountNodes() const {return amountNodes_;};
};


#endif //OOP8_GRAPH_H
