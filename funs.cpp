#include "AlgorithmDisplay.h"

AlgorithmDisplay::AlgorithmDisplay(QWidget *parent)
    : QMainWindow(parent)
{
    this->initialization();
    ui.setupUi(this);
}

AlgorithmDisplay::~AlgorithmDisplay()
{}


void AlgorithmDisplay::visit(int begin_number, int end_number,QColor line_color,QColor table_color)
{

    QString edge_name = QString("v%1->v%2").arg(begin_number).arg(end_number);

    //此边存在就染色
    if (this->Edges.find(edge_name) != this->Edges.end())
        this->Edges[edge_name]->color = line_color;
    //两节点染色
    this->Nodes["v" + QString::number(begin_number)] = line_color;
    this->Nodes["v" + QString::number(end_number)] = line_color;
    //表格染色
    this->Table[begin_number - 1][end_number - 1] = table_color;

    update();
}

void AlgorithmDisplay::initialization()
{
    //清空所有边
    for (auto& [edge_name, edge_info] : this->Edges)
        edge_info->~EdgeInfo();
    this->Edges.clear();
    //重置节点颜色
    for (auto& [node_name, node_color] : this->Nodes)
        node_color = Qt::yellow;
    
    //重置邻接矩阵和表格
    for (int i = 0; i < this->AdjacencyMatrix.size(); i++)
        for (int j = 0; j < this->AdjacencyMatrix[i].size(); j++)
        {
            this->Table[i][j] = Qt::white;
            this->AdjacencyMatrix[i][j] = (i == j) ? 0 : AlgorithmDisplay::INF;
        }
    //重置邻接表
    for (auto& nodes : this->AdjacencyList)
        nodes.clear();
    update();
}

void AlgorithmDisplay::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //边
    for (const auto& [edge_name, edge_info] : this->Edges)
    {
        QColor EdgeColor = edge_info->color;
        QLineF EdgeLine = edge_info->line;
        int EdgeValue = edge_info->value;


        double midx = (EdgeLine.p1().x() + EdgeLine.p2().x()) / 2.0;
        double midy = (EdgeLine.p1().y() + EdgeLine.p2().y()) / 2.0;

        //在边的中点 显示边长度
        painter.setPen(QPen(Qt::black));
        painter.setFont(QFont("Microsoft YaHei UI", 20));
        painter.drawText(QPointF(midx, midy), QString::number(EdgeValue));
        //画边
        painter.setPen(QPen(EdgeColor, 3));
        painter.drawLine(EdgeLine);

        //const auto [p1, p2] = this->ComputeArrowHead(EdgeLine);
        //painter.drawLine(EdgeLine.p2(), p1);
        //painter.drawLine(EdgeLine.p2(), p2);
    }

    //节点
    for (const auto& node : this->findChildren<QLabel*>())
    {
        //该label不是节点就跳过
        if (this->Nodes.find(node->objectName()) == this->Nodes.end())
            continue;

        QString NodeName = node->objectName();
        QColor NodeColor = this->Nodes[NodeName];
        QString rgb = QString("rgb(%1,%2,%3)")
            .arg(NodeColor.red())
            .arg(NodeColor.green())
            .arg(NodeColor.blue());

        node->setAutoFillBackground(true);
        node->setStyleSheet(QString(
            "color:white;"
            "border-radius:25px;"
            "background-color: %1;").arg(rgb));//节点颜色
    }

    //表格
    const auto& table_color = this->Table;
    for (int i = 0; i < table_color.size(); i++)
    {
        for (int j = 0; j < table_color[i].size(); j++)
        {
            ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);//设置文字居中
            ui.tableWidget->item(i, j)->setBackground(table_color[i][j]);//设置单元格颜色
            QString value = this->AdjacencyMatrix[i][j] == AlgorithmDisplay::INF ? QString(QChar(0x221E)) : QString::number(this->AdjacencyMatrix[i][j]);
            ui.tableWidget->item(i, j)->setText(value);
        }
    }
}

//计算箭头坐标
QPair<QPointF, QPointF> AlgorithmDisplay::ComputeArrowHead(const QLineF line)
{
    QPointF begin = line.p1();
    QPointF end = line.p2();

    //画箭头 
    int arrowSize = 10;//箭头大小
    double arrowAngle = M_PI / 6;//箭头夹角

    double angle = std::atan2(line.dy(), line.dx());
    QPointF arrowP1 = end - QPointF(cos(angle + arrowAngle) * arrowSize, sin(angle + arrowAngle) * arrowSize);
    QPointF arrowP2 = end - QPointF(cos(angle - arrowAngle) * arrowSize, sin(angle - arrowAngle) * arrowSize);


    return qMakePair(arrowP1, arrowP2);
}
