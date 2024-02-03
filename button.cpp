#include "AlgorithmDisplay.h"


void AlgorithmDisplay::on_Floyd_clicked()
{
    auto& dis = this->AdjacencyMatrix;
    int n = dis.size();
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                //if (dis[i][j] < dis[i][k] + dis[k][j])
                //    continue;
                //dis[i][j] = dis[i][k] + dis[k][j];

                dis[i][j] = std::min(dis[i][j], dis[i][k] + dis[k][j]);
                //染色操作
                this->visit(i + 1, j + 1, Qt::red, Qt::red);
                this->visit(i + 1, k + 1, Qt::red, Qt::red);
                this->visit(k + 1, j + 1, Qt::red, Qt::red);
                
                //还原操作
                int interval = 1000; // 1秒延迟
                QTimer::singleShot(interval, [&]() {
                    this->visit(i + 1, j + 1, Qt::blue, Qt::white);
                    this->visit(i + 1, k + 1, Qt::blue, Qt::white);
                    this->visit(k + 1, j + 1, Qt::blue, Qt::white);
                    });
                QEventLoop loop;
                QTimer::singleShot(interval, &loop, &QEventLoop::quit);
                loop.exec();
            }
        }
    }

    QMessageBox::information(this, "注意", "运行完成", QMessageBox::Ok);

}

void AlgorithmDisplay::on_Dijkstra_clicked()
{
    //重置输出路径
    ui.plainTextEdit->clear();


    int dis[10];
    bool vis[10];
    int path[10];


    auto dij = [&](int begin)->void
        {
            memset(path, 0, sizeof path);
            memset(dis, AlgorithmDisplay::INF, sizeof dis);
            memset(vis, false, sizeof vis);

            std::priority_queue<QPair<int, int>,
                QVector<QPair<int, int>>,
                std::greater<QPair<int, int>>>q;

            dis[begin] = 0;
            q.push(qMakePair(dis[begin], begin));
            
            while (!q.empty())
            {
                int now = q.top().second;
                q.pop();
                if (vis[now])
                    continue;
                vis[now] = true;

                for (auto [to, w] : this->AdjacencyList[now])
                {
                    if (dis[to] > dis[now] + w)
                    {
                        path[to] = now;
                        dis[to] = dis[now] + w;
                    }
                    if (!vis[to])
                        q.push(qMakePair(dis[to], to));



                    //染色
                    this->visit(now, to, Qt::red, Qt::red);
                    int interval = 1000; // 1秒延迟
                    QTimer::singleShot(interval, [&]() {
                        this->visit(now, to, Qt::blue, Qt::white);
                        });
                    QEventLoop loop;
                    QTimer::singleShot(interval, &loop, &QEventLoop::quit);
                    loop.exec();

                }
            }
        };

    dij(1);
    QString tmp1(""),tmp2("");
    for (auto i = 1; i <= 6; i++)
    {
        tmp2.append(std::to_string(i)).append(" \n"[i == 6]);
        tmp1.append(std::to_string(dis[i])).append(" \n"[i == 6]);
    }
    ui.plainTextEdit->appendPlainText(tmp2);
    ui.plainTextEdit->appendPlainText(tmp1);
    for (int i = 2; i <= 6; i++)
    {
        QString buf("");
        for (int now = i; now; now = path[now])
            buf.append(std::to_string(now).append((now == 1) ? "" : "<-"));
        //qDebug().noquote() << buf;
        ui.plainTextEdit->appendPlainText(buf);
    }


}

void AlgorithmDisplay::on_AddEdge_clicked()
{
    QString begin_node = ui.Begin->currentText();//起点节点名称
    QString end_node = ui.End->currentText();//终点节点名称
    QString value = ui.EdgeValue->text();//边权

    //值为空 || 不是全数字 || 终点和起点相同
    if (value == "" || !value.contains(QRegularExpression("^\\d+$")) || begin_node == end_node)
    {
        QMessageBox::critical(this, "错误", "非法输入");
        ui.EdgeValue->setText("");
        return;
    }
    //清空输入框
    ui.EdgeValue->setText("");

    int begin_number = begin_node.mid(1).toInt() - 1;//起点节点编号
    int end_number = end_node.mid(1).toInt() - 1;//终点节点编号
    int edge_value = value.toInt();//边权

    int radius = 30 ;

    QPointF begin_pos = this->findChild<QLabel*>(begin_node)->pos() + QPointF(radius, radius);//起点节点坐标
    QPointF end_pos = this->findChild<QLabel*>(end_node)->pos() + QPointF(radius, radius);//终点节点坐标


    //double dx = begin_pos.x() - end_pos.x();
    //double dy = begin_pos.y() - end_pos.y();
    //double dis = sqrt(dx * dx + dy * dy);
    //double ux = dx / dis;
    //double uy = dy / dis;

    //begin_pos = QPointF(begin_pos.x() - radius * ux, begin_pos.y() - radius * uy);
    //end_pos = QPointF(end_pos.x() + radius * ux, end_pos.y() + radius * uy);

    QLineF line(begin_pos, end_pos);//重新计算边的两点坐标


    /*建立双向边*/
    
    //邻接矩阵 建双向边
    this->AdjacencyMatrix[begin_number][end_number] = edge_value;
    this->AdjacencyMatrix[end_number][begin_number] = edge_value;
    //邻接表 建双向边
    this->AdjacencyList[begin_number + 1].emplaceBack(end_number + 1, edge_value);
    this->AdjacencyList[end_number + 1].emplaceBack(begin_number + 1, edge_value);

    //节点信息修改 染色 有边的节点都是蓝色
	this->Nodes[begin_node] = Qt::blue;
    this->Nodes[end_node] = Qt::blue;
    //边信息修改 染色 边为蓝色
    EdgeInfo* new_edge = new EdgeInfo(line, Qt::blue, edge_value);
    this->Edges[begin_node + "->" + end_node] = new_edge;
    this->Edges[end_node + "->" + begin_node] = new_edge;
    //表格信息修改 
    this->Table[begin_number][end_number] = Qt::blue;
    this->Table[end_number][begin_number] = Qt::blue;

    update();
}


void AlgorithmDisplay::on_Reset_clicked()
{
    this->initialization();
    QMessageBox::information(this, "注意", "数据已重置", QMessageBox::Ok);
}