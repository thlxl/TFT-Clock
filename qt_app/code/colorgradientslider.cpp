#include "colorgradientslider.h"

void ColorGradientSlider::paintEvent(QPaintEvent *event)
{
    QSlider::paintEvent(event);

    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QPainter painter(this);
    QLinearGradient gradient(opt.rect.left(), 0, opt.rect.right(), 0);

    for (int i = 0; i <= 255; ++i) {
        QColor color;
        color.setHsv(i, 255, 255);
        gradient.setColorAt(i / 255.0, color);
    }

    QRect groove = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    groove.adjust(0, groove.height() / 3, 0, -groove.height() / 3);

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(groove);

    QRect handle = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    QColor handleColor;
    handleColor.setHsv(value(), 255, 255);
    painter.setBrush(handleColor);
    painter.drawRect(handle);
}
