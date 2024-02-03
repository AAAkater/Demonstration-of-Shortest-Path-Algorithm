#pragma once

#include <QtWidgets/QMainWindow>

#include<qpainter.h>
#include<qtimer.h>
#include<qmessagebox.h>

#include <queue>
#include <map>
#include "ui_AlgorithmDisplay.h"


class EdgeInfo
{
public:
    QLineF line;//位置
    QColor color;//颜色
    int value;//权值
    EdgeInfo() {}
    EdgeInfo(QLineF line, QColor color, int w)
    {
        this->line = line;
        this->color = color;
        this->value = w;
    }
    ~EdgeInfo() {};
};


class NodeInfo
{
    int w;
    int from;
    int to;
    NodeInfo(){}
    NodeInfo(int w,int from,int to)
    {
        this->w = w;
        this->from = from;
        this->to = to;
    }
    bool operator <(const NodeInfo& it) { return it.w < this->w; }

};

class AlgorithmDisplay : public QMainWindow
{
    Q_OBJECT

public:
    AlgorithmDisplay(QWidget *parent = nullptr);
    ~AlgorithmDisplay();
private:
    Ui::AlgorithmDisplayClass ui;

    static const int INF = 0x3f3f3f3f;
    //边信息
    std::map<QString, EdgeInfo*>Edges;
    //节点信息
    std::map<QString, QColor>Nodes;
    //表格信息
    QVector<QVector<QColor>>Table{ 6,QVector<QColor>(6) };

    //邻接矩阵
    QVector<QVector<int>>AdjacencyMatrix{ 6,QVector<int>(6) };
    //邻接表
    QVector<QPair<int, int>>AdjacencyList[10];
    
    //计算箭头坐标
    QPair<QPointF, QPointF>ComputeArrowHead(const QLineF line);
    //初始化
    void initialization();
    //访问节点和边 并改变颜色
    void visit(int begin_number, int end_number, QColor line_color, QColor table_color);
    void paintEvent(QPaintEvent* event);
private slots:
    void on_Dijkstra_clicked();
    void on_Floyd_clicked();
    void on_AddEdge_clicked();
    void on_Reset_clicked();
};



