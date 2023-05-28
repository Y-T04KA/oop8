//
// Created by kiril on 28.05.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TInterface.h" resolved

#include "tinterface.h"
#include "ui_TInterface.h"


TInterface::TInterface(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::TInterface) {
    ui->setupUi(this);
    graph = new Graph;
    box = ui->changeBox;
}

TInterface::~TInterface() {
    delete ui;
}

void TInterface::pushBt() {
    QString filePath = QFileDialog::getOpenFileName(this, "Открыть файл", "", "TXT (*.txt)");
    box->clear();
    if (!filePath.isEmpty()){
        if (readCheckGraph(filePath)){
            for (int i=1; i<graph->getAdjacencyMatrix().size()+1; i++){
                box->addItem(QString::number(i));
            }
            used = true;
            drawGraph(graph->getAdjacencyMatrix());
        } else {
            QMessageBox::critical(this, "Ошибка", "неверные данные в файле");
        }
    }
}

void TInterface::resetBt() {
    used = false;
    graph->setActiveNode(firstValue);
    box->setCurrentIndex(graph->getActiveNode()-1);
    drawGraph(graph->getAdjacencyMatrix());
    used = true;
}

void TInterface::doBt() {
    if (used){
        int prevNode = graph->getActiveNode();
        int curNode = box->currentIndex()+1;
        if (check(prevNode - 1, curNode - 1) or !used){
            graph->setActiveNode(curNode);
            drawGraph(graph->getAdjacencyMatrix());
        } else {
            box->setCurrentIndex(prevNode-1);
            QMessageBox::critical(this, "Ошибка", "Переход невозможен");
        }
    }
}

void TInterface::drawGraph(QVector<QVector<qint16>> graphData) {
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    auto startX = 250, startY = 250, radius = 200;
    auto numNodes = graphData.size();
    int activeNode = graph->getActiveNode();
    double nodeSize = 30;
    for (int i=0; i<numNodes; i++){
        double angle = 2*M_PI*i/numNodes;
        auto x = startX + radius*cos(angle), y = startY+radius*sin(angle);
        auto *node = new QGraphicsEllipseItem(0, 0, nodeSize, nodeSize);
        node->setPos(x - nodeSize / 2, y - nodeSize / 2);
        if(i + 1 == activeNode){
            QBrush brush(Qt::red);
            QColor color = brush.color();
            color.setAlpha(64);
            brush.setColor(color);
            node->setBrush(brush);
        }
        auto *label = new QGraphicsTextItem(QString::number(i + 1));
        label->setPos(node->pos().x() + nodeSize / 2 - label->boundingRect().width() / 2, node->pos().y() + nodeSize / 2 - label->boundingRect().height() / 2);
        scene->addItem(label);
        scene->addItem(node);
        for (int j = 0; j < numNodes; j++){
            if (graphData[i][j] == 1){
                double angle2 = 2 * M_PI * j / numNodes;
                auto x2 = startX + radius * cos(angle2), y2 = startY + radius * sin(angle2);
                double dx = x2 - x, dy = y2 - y, length = sqrt(dx * dx + dy * dy);
                dx /= length;
                dy /= length;
                auto *line = new QGraphicsLineItem();
                line->setPen(QPen(Qt::black));
                line->setLine(QLineF(x + nodeSize / 2 + dx * nodeSize / 2 - (nodeSize / 2), y + nodeSize / 2 + dy * nodeSize / 2 - (nodeSize / 2), x2 + nodeSize / 2 - dx * nodeSize / 2 - (nodeSize / 2), y2 + nodeSize / 2 - dy * nodeSize / 2 - (nodeSize / 2)));
                QPolygonF arrow;
                arrow << QPointF(-5, -5) << QPointF(0, 0) << QPointF(-5, 5);
                auto *arrowItem = new QGraphicsPolygonItem(arrow);
                arrowItem->setPen(QPen(Qt::black));
                arrowItem->setBrush(QBrush(Qt::black));
                arrowItem->setPos(x2 + nodeSize / 2 - dx * nodeSize / 2 - (nodeSize / 2), y2 + nodeSize / 2 - dy * nodeSize / 2 - (nodeSize / 2));
                angle = atan2(dy, dx);
                angle = angle * 180 / M_PI;
                arrowItem->setRotation(angle);
                scene->addItem(line);
                scene->addItem(arrowItem);
            }
        }
    }
}

bool TInterface::readCheckGraph(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "Ошибка", "Невозможно открыть файл.");
        return false;
    }
    QTextStream in(&file);
    QVector<QVector<qint16>> adjacencyMatrix;
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList values = line.split(' ');
        if (values.size() > 1){
            QVector<qint16> row;
            for (const QString &value : values){
                bool ok;
                int intValue = value.toInt(&ok);
                if (!ok){
                    QMessageBox::critical(this, "Ошибка", "Неверные данные в файле.");
                    return false;
                }
                row.append(intValue);
            }
            adjacencyMatrix.append(row);
        }
        else{
            bool ok;
            int intValue = values[0].toInt(&ok);
            if (!ok or intValue < 1 or intValue > adjacencyMatrix.size()){
                QMessageBox::critical(this, "Ошибка", "Неверные данные в файле.");
                return false;
            }
            graph->setActiveNode(intValue);
            firstValue = graph->getActiveNode();
        }
    }
    file.close();
    graph->setAdjacencyMatrix(adjacencyMatrix);
    return true;
}

bool TInterface::check(int prev, int cur) {
    if(graph->getAdjacencyMatrix()[prev][cur] == 1) return true;
    else return false;
}







