#include <QEvent>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

#include "ColorWidget.h"

ColorWidget::ColorWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(min_width, min_height);
    setAutoFillBackground(true);

    QPalette pal(palette());
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);

    installEventFilter(this);
}

bool ColorWidget::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Paint:
    case QEvent::Resize:
    {
        center_width = width() - margin_left - margin_right;
        center_height = height() - margin_up - margin_down;
        center_rect.setSize(QSize(center_width, center_height));

        //QPainter painter(this);
        drawCenter();
        drawUp();
        drawDown();
        //drawLeft(&painter);
        //drawRight(&painter);

        //QPixmap pixmap =  this->grab();
        //pixmap.save("out.png");

        return true;
        break;
    }
    case QEvent::KeyPress:
    {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);
        switch (e->key())
        {
        case Qt::Key_Left:
            QPixmap pixmap =  this->grab();
            pixmap.save("out.png");

            break;


        }

        break;
    }
    default:
        break;
    }

    return QWidget::eventFilter(watched, event);
}


QImage getColorBar(int image_width, int image_height,
                   const QVector<QColor> &color_bar = {QColor(0, 0, 143),
        QColor(0, 0, 159),
        QColor(0, 0, 175),
        QColor(0, 0, 191),
        QColor(0, 0, 207),
        QColor(0, 0, 223),
        QColor(0, 0, 239),
        QColor(0, 0, 255),
        QColor(0, 16, 255),
        QColor(0, 32, 255),
        QColor(0, 48, 255),
        QColor(0, 64, 255),
        QColor(0, 80, 255),
        QColor(0, 96, 255),
        QColor(0, 112, 255),
        QColor(0, 128, 255),
        QColor(0, 143, 255),
        QColor(0, 159, 255),
        QColor(0, 175, 255),
        QColor(0, 191, 255),
        QColor(0, 207, 255),
        QColor(0, 223, 255),
        QColor(0, 239, 255),
        QColor(0, 255, 255),
        QColor(16, 255, 239),
        QColor(32, 255, 223),
        QColor(48, 255, 207),
        QColor(64, 255, 191),
        QColor(80, 255, 175),
        QColor(96, 255, 159),
        QColor(112, 255, 143),
        QColor(128, 255, 128),
        QColor(143, 255, 112),
        QColor(159, 255, 96),
        QColor(175, 255, 80),
        QColor(191, 255, 64),
        QColor(207, 255, 48),
        QColor(223, 255, 32),
        QColor(239, 255, 16),
        QColor(255, 255, 0),
        QColor(255, 239, 0),
        QColor(255, 223, 0),
        QColor(255, 207, 0),
        QColor(255, 191, 0),
        QColor(255, 175, 0),
        QColor(255, 159, 0),
        QColor(255, 143, 0),
        QColor(255, 128, 0),
        QColor(255, 112, 0),
        QColor(255, 96, 0),
        QColor(255, 80, 0),
        QColor(255, 64, 0),
        QColor(255, 48, 0),
        QColor(255, 32, 0),
        QColor(255, 16, 0),
        QColor(255, 0, 0),
        QColor(239, 0, 0),
        QColor(223, 0, 0),
        QColor(207, 0, 0),
        QColor(191, 0, 0),
        QColor(175, 0, 0),
        QColor(159, 0, 0),
        QColor(143, 0, 0),
        QColor(128, 0, 0)}){
    QImage image(image_width, image_height, QImage::Format_RGBA8888);
    int color_number = color_bar.size();

    for (int x = 0; x < image_width; x++)
    {
        double ind = x / (image_width - 1.0) *   (color_number-1);
        int ind1 = qRound(ind);
        if(ind1 == color_number-1)
            ind1 = color_number-2;
        int ind2 = ind1+1;
        QColor color1 = color_bar.at(ind1);
        QColor color2 = color_bar.at(ind2);
        double factor1 = ind2 - ind;
        double factor2 = ind - ind1;

        QColor color;
        color.setRed(qRound(color1.red() * factor1 + color2.red() * factor2));
        color.setGreen(qRound(color1.green() * factor1 + color2.green() * factor2));
        color.setBlue(qRound(color1.blue() * factor1 + color2.blue() * factor2));

        for (int y = 0; y < image_height; y++)
            image.setPixelColor(x, y, color);
    }
    return image;
}

QImage getColorBar2(int image_width, int image_height){
    QVector<QVector<double>> color_bar{ {         0,         0,    0.5625},
                                        {         0,         0,    0.6250},
                                        {         0,         0,    0.6875},
                                        {         0,         0,    0.7500},
                                        {         0,         0,    0.8125},
                                        {         0,         0,    0.8750},
                                        {         0,         0,    0.9375},
                                        {         0,         0,    1.0000},
                                        {         0,    0.0625,    1.0000},
                                        {         0,    0.1250,    1.0000},
                                        {         0,    0.1875,    1.0000},
                                        {         0,    0.2500,    1.0000},
                                        {         0,    0.3125,    1.0000},
                                        {         0,    0.3750,    1.0000},
                                        {         0,    0.4375,    1.0000},
                                        {         0,    0.5000,    1.0000},
                                        {         0,    0.5625,    1.0000},
                                        {         0,    0.6250,    1.0000},
                                        {         0,    0.6875,    1.0000},
                                        {         0,    0.7500,    1.0000},
                                        {         0,    0.8125,    1.0000},
                                        {         0,    0.8750,    1.0000},
                                        {         0,    0.9375,    1.0000},
                                        {         0,    1.0000,    1.0000},
                                        {    0.0625,    1.0000,    0.9375},
                                        {    0.1250,    1.0000,    0.8750},
                                        {    0.1875,    1.0000,    0.8125},
                                        {    0.2500,    1.0000,    0.7500},
                                        {    0.3125,    1.0000,    0.6875},
                                        {    0.3750,    1.0000,    0.6250},
                                        {    0.4375,    1.0000,    0.5625},
                                        {    0.5000,    1.0000,    0.5000},
                                        {    0.5625,    1.0000,    0.4375},
                                        {    0.6250,    1.0000,    0.3750},
                                        {    0.6875,    1.0000,    0.3125},
                                        {    0.7500,    1.0000,    0.2500},
                                        {    0.8125,    1.0000,    0.1875},
                                        {    0.8750,    1.0000,    0.1250},
                                        {    0.9375,    1.0000,    0.0625},
                                        {    1.0000,    1.0000,         0},
                                        {    1.0000,    0.9375,         0},
                                        {    1.0000,    0.8750,         0},
                                        {    1.0000,    0.8125,         0},
                                        {    1.0000,    0.7500,         0},
                                        {    1.0000,    0.6875,         0},
                                        {    1.0000,    0.6250,         0},
                                        {    1.0000,    0.5625,         0},
                                        {    1.0000,    0.5000,         0},
                                        {    1.0000,    0.4375,         0},
                                        {    1.0000,    0.3750,         0},
                                        {    1.0000,    0.3125,         0},
                                        {    1.0000,    0.2500,         0},
                                        {    1.0000,    0.1875,         0},
                                        {    1.0000,    0.1250,         0},
                                        {    1.0000,    0.0625,         0},
                                        {    1.0000,         0,         0},
                                        {    0.9375,         0,         0},
                                        {    0.8750,         0,         0},
                                        {    0.8125,         0,         0},
                                        {    0.7500,         0,         0},
                                        {    0.6875,         0,         0},
                                        {    0.6250,         0,         0},
                                        {    0.5625,         0,         0},
                                        {    0.5000,         0,         0}};

    QImage image(image_width, image_height, QImage::Format_RGBA8888);
    int color_number = color_bar.size();

    for (int x = 0; x < image_width; x++)
    {
        double ind = x / (image_width - 1.0) *   (color_number-1);
        int ind1 = ind;
        if(ind1 == color_number-1)
            ind1 = color_number-2;
        int ind2 = ind1+1;
        QVector<double> color1 = color_bar.at(ind1);
        QVector<double> color2 = color_bar.at(ind2);
        double factor1 = ind2 - ind;
        double factor2 = ind - ind1;

        QColor color;
        color.setRed(qRound(255*color1.at(0) * factor1 + 255*color2.at(0) * factor2));
        color.setGreen(qRound(255*color1.at(1) * factor1 + 255*color2.at(1) * factor2));
        color.setBlue(qRound(255*color1.at(2) * factor1 + 255*color2.at(2) * factor2));

        for (int y = 0; y < image_height; y++)
            image.setPixelColor(x, y, color);
    }
    return image;
}
void ColorWidget::drawCenter()
{
    QPainter painter(this);
    QImage image = getColorBar2(center_width, center_height);
    painter.drawImage(margin_left, margin_up, image);
}

void ColorWidget::drawUp()
{
    QPainter painter(this);
    painter.save();

    QFont font;
    font.setPointSize(20);
    painter.setFont(font);
    QRect textRect(margin_left, 0, center_width, margin_up);
    painter.drawText(textRect, Qt::AlignCenter, "ColorWidget");

    painter.restore();
}

void ColorWidget::drawDown()
{
    QPainter painter(this);
    painter.save();

    double min = 1.0;
    double max = 20.0;
    QVector<QPair<double, QString>> pair;
    pair<<qMakePair(1.0, QString("1.0"))
       <<qMakePair(2.0, QString("2.0"))
      <<qMakePair(5.0, QString("5.0"))
     <<qMakePair(10.0, QString("10.0"))
    <<qMakePair(20.0, QString("20.0"));

    QRectF textRect(0, 0, 500, margin_down);

    for(int i=0;i<pair.size();i++){
        double value = pair.at(i).first;
        QString label =pair.at(i).second;
        int col = (value-min) / (max-min) * center_width;

        textRect.moveCenter(QPoint(margin_left + col, height() - margin_down / 2.0));
        painter.drawText(textRect, Qt::AlignCenter, label);
    }

    painter.restore();
}

void ColorWidget::drawLeft(QPainter *painter)
{
    painter->save();

    double space = center_height / (12-1.0);
    QRectF textRect(0, 0, margin_left, space);

    for (int i = 0; i < 12; i++)
    {
        textRect.moveCenter(QPoint(margin_left / 2.0, margin_up + space * i));
        painter->drawText(textRect, Qt::AlignCenter, QString::number(i + 1));
    }

    painter->restore();
}

void ColorWidget::drawRight(QPainter *)
{

}
