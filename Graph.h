//
// Created by kiril on 28.05.2023.
//

#ifndef OOP8_GRAPH_H
#define OOP8_GRAPH_H
#include <QVector>

class Graph {
    QVector<QVector<qint16>> Matrix;
    int activeNode;
public:
    Graph() = default;
    void setAdjacencyMatrix(const QVector<QVector<qint16>> & adjacencyMatrix){Matrix = adjacencyMatrix;};
    QVector<QVector<qint16>> getAdjacencyMatrix() const{return Matrix;}
    void setActiveNode(const int value){ activeNode = value;};
    int getActiveNode() const {return activeNode;};
};


#endif //OOP8_GRAPH_H
