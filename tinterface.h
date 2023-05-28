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
#include <QVector>
#include "Graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TInterface; }
QT_END_NAMESPACE

class TInterface : public QMainWindow {
Q_OBJECT

public:
    explicit TInterface(QWidget *parent = nullptr);
    ~TInterface() override;
    bool check(int, int);
public slots:
    void pushBt();
    void resetBt();
    void doBt();
private:
    Ui::TInterface *ui;
    bool readCheckGraph(const QString &filePath);
    void drawGraph(QVector<QVector<qint16>> graphData);
    QGraphicsScene *scene;
    Graph *graph;
    QComboBox *box;
    bool used = false;
    int firstValue;
};


#endif //OOP8_TINTERFACE_H
