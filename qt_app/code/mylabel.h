#ifndef MYLABEL_H
#define MYLABEL_H

#include <QObject>
#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    MyLabel(QWidget *parent = nullptr) : QLabel(parent) {}
    int point_size;
    int x1,x2,y1,y2;

protected:
    void paintEvent(QPaintEvent *event)override;
};

#endif // MYLABEL_H
