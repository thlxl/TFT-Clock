#ifndef COLORGRADIENTSLIDER_H
#define COLORGRADIENTSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QPainter>
#include <QStyleOptionSlider>
#include <QLinearGradient>

class ColorGradientSlider : public QSlider {
public:
    ColorGradientSlider(QWidget *parent = nullptr) : QSlider(Qt::Horizontal, parent) {
        setRange(0, 255);
    }

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // COLORGRADIENTSLIDER_H
