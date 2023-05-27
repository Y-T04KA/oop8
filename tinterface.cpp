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
    connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeBoxIndexChanged(int)));
}

TInterface::~TInterface() {
    delete ui;
}

void TInterface::pushBt() {
    QString filePath = QFileDialog::getOpenFileName(this, "Выбрать файл", "", "TXT (*.txt)");
    ui->label->setText(filePath);
    box->clear();
    if (!filePath.isEmpty()){
        if (readAndValidateGraphFile(filePath)){
            for (int i=1; i<graph->getAdjacencyMatrix().size()+1; i++){
                box->addItem("Перейти в вершину " + QString::number(i));
            }
            dirty = true;
            drawGraph(graph->getAdjacencyMatrix());
        } else {
            QMessageBox::critical(this, "Error", "wrong data");
        }
    }
}

void TInterface::resetBt() {
    dirty = false;
    graph->setActiveNode(firstValue);
    box->setCurrentIndex(graph->getActiveNode()-1);
    drawGraph(graph->getAdjacencyMatrix());
    dirty = true;
}

void TInterface::doBt() {
    if (dirty){
        int prevNode = graph->getActiveNode();
        int curNode = box->currentIndex()+1;
        if (checkPossibility(prevNode-1,curNode-1) or !dirty){
            graph->setActiveNode(curNode);
            drawGraph(graph->getAdjacencyMatrix());
        } else {
            box->setCurrentIndex(prevNode-1);
            QMessageBox::critical(this, "Ошибка", "Невозможно перейти из нынешного состояния в выбранное.");
        }
    }
}

void TInterface::drawGraph(std::vector<std::vector<int>> graphData) {
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    int numNodes = graphData.size(), nodeSize = 30, startX = 400, startY = 400, radius = 200;
    int activeNode = graph->getActiveNode();
    for (int i=0; i<numNodes; i++){
        double angle = 2*M_PI*i/numNodes, arrowSize = 10;
        int x = startX + radius*cos(angle), y = startY+radius*sin(angle);
        QGraphicsEllipseItem *node = new QGraphicsEllipseItem(0, 0, nodeSize, nodeSize);
        node->setPos(x - nodeSize / 2, y - nodeSize / 2);
        if(i + 1 == activeNode)
        {
            QBrush brush(Qt::red);
            QColor color = brush.color();
            color.setAlpha(64);
            brush.setColor(color);
            node->setBrush(brush);
        }
        QGraphicsTextItem *label = new QGraphicsTextItem(QString::number(i + 1));
        label->setPos(node->pos().x() + nodeSize / 2 - label->boundingRect().width() / 2, node->pos().y() + nodeSize / 2 - label->boundingRect().height() / 2);
        scene->addItem(label);
        scene->addItem(node);
        for (int j = 0; j < numNodes; j++)
        {
            if (graphData[i][j] == 1)
            {
                double angle2 = 2 * M_PI * j / numNodes;
                int x2 = startX + radius * cos(angle2), y2 = startY + radius * sin(angle2);
                double dx = x2 - x, dy = y2 - y, length = sqrt(dx * dx + dy * dy);
                dx /= length;
                dy /= length;
                QGraphicsLineItem *line = new QGraphicsLineItem();
                line->setPen(QPen(Qt::black));
                line->setLine(QLineF(x + nodeSize / 2 + dx * nodeSize / 2 - (nodeSize / 2), y + nodeSize / 2 + dy * nodeSize / 2 - (nodeSize / 2), x2 + nodeSize / 2 - dx * nodeSize / 2 - (nodeSize / 2), y2 + nodeSize / 2 - dy * nodeSize / 2 - (nodeSize / 2)));
                QPolygonF arrow;
                arrow << QPointF(-5, -5) << QPointF(0, 0) << QPointF(-5, 5);
                QGraphicsPolygonItem *arrowItem = new QGraphicsPolygonItem(arrow);
                arrowItem->setPen(QPen(Qt::black));
                arrowItem->setBrush(QBrush(Qt::black));
                arrowItem->setPos(x2 + nodeSize / 2 - dx * nodeSize / 2 - (nodeSize / 2), y2 + nodeSize / 2 - dy * nodeSize / 2 - (nodeSize / 2));
                double angle = atan2(dy, dx);
                angle = angle * 180 / M_PI;
                arrowItem->setRotation(angle);
                scene->addItem(line);
                scene->addItem(arrowItem);
            }
        }
    }
}

bool TInterface::readAndValidateGraphFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "Ошибка", "Невозможно открыть файл.");
        return false;
    }
    QTextStream in(&file);
    QVector<QVector<int>> adjacencyMatrix;
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList values = line.split(' ');
        if (values.size() > 1){
            QVector<int> row;
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
    std::vector<std::vector<int>> stdAdjacencyMatrix = convertToStdVector(adjacencyMatrix);
    graph->setAdjacencyMatrix(stdAdjacencyMatrix);
    return true;
}

std::vector<std::vector<int>> TInterface::convertToStdVector(const QVector<QVector<int>> &qvector) {
    std::vector<std::vector<int>> stdvector;
    for (const QVector<int>& innerVector : qvector){
        std::vector<int> stdInnerVector;
        stdInnerVector.reserve(innerVector.size());
        for (const int& value : innerVector){
            stdInnerVector.push_back(value);
        }
        stdvector.push_back(stdInnerVector);
    }
    return stdvector;
}

bool TInterface::checkPossibility(int prevNode, int curNode) {
    std::vector<std::vector<int>> adjacencyMatrix = graph->getAdjacencyMatrix();
    if(adjacencyMatrix[prevNode][curNode] == 1) return true;
    else return false;
}







