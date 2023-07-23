#pragma once

#include <QWidget>

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorWidget(QWidget *parent = nullptr);
protected:
    bool eventFilter(QObject *, QEvent *) override;
private:
    void drawCenter();
    void drawUp();
    void drawDown();
    void drawLeft(QPainter *);
    void drawRight(QPainter *);
private:
    const int margin_up = 50;
    const int margin_down = 50;
    const int margin_left = 20;
    const int margin_right = 20;
    const int min_width = margin_left + 400 + margin_right;
    const int min_height = margin_up + 12 * 9 + margin_down;

    int center_width;
    int center_height;
    QRect center_rect{margin_left, margin_up, 0, 0};
};
