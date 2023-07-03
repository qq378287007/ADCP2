#include "InfoWidget.h"

#include <QLabel>
#include <QHBoxLayout>

InfoWidget::InfoWidget(QWidget *parent) : QScrollArea (parent)
{
    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    widget = new QWidget(this);
    widget->setLayout(mainLayout);


    QHBoxLayout *tmp;
    QLabel *title;
    QLabel *left;
    QLabel *right;

    QFont font;
    font.setPointSize(18);

    QFont font2;
    font2.setPointSize(10);

    QPalette pe;
    pe.setColor(QPalette::Window, Qt::blue);

    QPalette pe2;
    pe2.setColor(QPalette::WindowText,Qt::red);

    for(int i=0; i<55; i++){
        title = new QLabel(QString("中文%1").arg(i), this);
        title->setFont(font);
        title->setAutoFillBackground(true);
        title->setPalette(pe);
        mainLayout->addWidget(title);

        left = new QLabel(QString("  left%1").arg(i), this);
        left->setFont(font2);
        left->setPalette(pe2);
        right = new QLabel(QString("right%1").arg(i), this);
        right->setFont(font2);
        right->setPalette(pe2);
        tmp = new QHBoxLayout();
        tmp->addWidget(left);
        tmp->addStretch();
        tmp->addWidget(right);
        mainLayout->addLayout(tmp);
    }

    setWidget(widget);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignCenter); // 居中对齐
    setWidgetResizable(true); // 自动调整大小
    setMinimumSize(100, 60);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(palette);



}
