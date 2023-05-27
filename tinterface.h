//
// Created by kiril on 28.05.2023.
//

#ifndef OOP8_TINTERFACE_H
#define OOP8_TINTERFACE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QComboBox>
#include "Graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TInterface; }
QT_END_NAMESPACE

class TInterface : public QMainWindow {
Q_OBJECT

public:
    explicit TInterface(QWidget *parent = nullptr);
    ~TInterface() override;
    bool checkPossibility(int,int);
public slots:
    void pushBt();
    void resetBt();
    void doBt();
private:
    Ui::TInterface *ui;
    std::vector<std::vector<int>> convertToStdVector(const QVector<QVector<int>>& qvector);
    bool readAndValidateGraphFile(const QString &filePath);
    void drawGraph(std::vector<std::vector<int>> graphData);
    QGraphicsScene *scene;
    Graph *graph;
    QComboBox *box;
    bool dirty = false;
    int firstValue;
};


#endif //OOP8_TINTERFACE_H
