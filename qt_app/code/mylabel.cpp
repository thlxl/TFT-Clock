#include "mylabel.h"
#include <QPainter>

void MyLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    // 获取 QLabel 的大小和中心点坐标
    int labelWidth = width();
    int labelHeight = height();
    QPoint center(labelWidth / 2, labelHeight / 2);

    // 创建 QPainter 对象，并在其上绘制小黑点
    QPainter painter(this);
    painter.setPen(Qt::black); // 设置画笔颜色为黑色
    painter.setBrush(Qt::black);  // 设置刷子颜色为黑色

    // 绘制上半部分的黑点
    painter.drawEllipse(center.x() - x1, center.y() - y1, point_size, point_size);

    // 绘制下半部分的黑点
    painter.drawEllipse(center.x() - x2, center.y() + y2, point_size, point_size);
}
