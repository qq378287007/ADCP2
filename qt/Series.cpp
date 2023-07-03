#include <QEvent>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

#include "data2image.h"

#include "Series.h"

Series::Series(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(min_width, min_height);
    setAutoFillBackground(true);

    QPalette pal(palette());
    pal.setColor(QPalette::Window, Qt::gray);
    setPalette(pal);

    installEventFilter(this);

    menu = new QMenu(this);

    QStringList strList;

    strList.clear();
    strList << "BTV1"
            << "BTV2"
            << "BTV3"
            << "BTV4";
    QMenu *m_BTVMenu = new QMenu("BTV", this);
    foreach (auto str, strList)
        m_BTVMenu->addAction(new QAction(str, this));
    menu->addMenu(m_BTVMenu);

    strList.clear();
    strList << "BTDepth1"
            << "BTDepth2"
            << "BTDepth3"
            << "BTDepth4";
    QMenu *m_BTDepthMenu = new QMenu("BTDepth", this);
    foreach (auto str, strList)
        m_BTDepthMenu->addAction(new QAction(str, this));
    menu->addMenu(m_BTDepthMenu);

    strList.clear();
    strList << "BTE1"
            << "BTE2"
            << "BTE3"
            << "BTE4";
    QMenu *m_BTEMenu = new QMenu("BTE", this);
    foreach (auto str, strList)
        m_BTEMenu->addAction(new QAction(str, this));
    menu->addMenu(m_BTEMenu);

    strList.clear();
    strList << "BTCor1"
            << "BTCor2"
            << "BTCor3"
            << "BTCor4";
    QMenu *m_BTCorMenu = new QMenu("BTCor", this);
    foreach (auto str, strList)
        m_BTCorMenu->addAction(new QAction(str, this));
    menu->addMenu(m_BTCorMenu);

    strList.clear();
    strList << "BTP1"
            << "BTP2"
            << "BTP3"
            << "BTP4";
    QMenu *m_BTPMenu = new QMenu("BTP", this);
    foreach (auto str, strList)
        m_BTPMenu->addAction(new QAction(str, this));
    menu->addMenu(m_BTPMenu);

    strList.clear();
    strList << "Pressure"
            << "Temp"
            << "Roll"
            << "Pitch"
            << "Heading";
    QMenu *m_Otherenu = new QMenu("Other", this);
    foreach (auto str, strList)
        m_Otherenu->addAction(new QAction(str, this));
    menu->addMenu(m_Otherenu);

    connect(menu, &QMenu::triggered, [&](QAction *action)
    {
        str = action->text();
        update(); });
}

void Series::setFileData(const QSharedPointer<FileData> &FileData)
{
    m_FileData = FileData;
    str = "BTV1";
    updateStartCol(0);
}

void Series::contextMenuEvent(QContextMenuEvent *event)
{
    menu->move(event->globalX(), event->globalY());
    menu->show();
}

bool Series::eventFilter(QObject *watched, QEvent *event)
{
    if (m_FileData.isNull())
        return true;

    if (event->type() == QEvent::Paint)
    {
        center_width = width() - margin_left - margin_right;
        center_height = height() - margin_up - margin_down;

        if (m_StartCol < 0)
            m_StartCol = 0;
        else if (m_StartCol > m_FileData->Ping_Num - center_width)
            m_StartCol = m_FileData->Ping_Num - center_width;

        QPainter painter(this);
        drawCenter(&painter);
        drawUp(&painter);
        drawDown(&painter);
        drawLeft(&painter);
        drawRight(&painter);
    }
    return QWidget::eventFilter(watched, event);
}

void Series::drawCenter(QPainter *painter)
{
    painter->save();

    double *data = m_FileData->str2Data(str);

    double min_value;
    double max_value;
    m_FileData->getMinMax(str, min_value, max_value);

    QImage image = getSeries1(data,
                              m_StartCol, m_StartCol + center_width - 1,
                              min_value, max_value,
                              center_width, center_height);
    painter->drawImage(margin_left, margin_up, image);

    painter->restore();
}

void Series::drawUp(QPainter *painter)
{
    painter->save();

    QRect textRect(margin_left, 0, center_width, margin_up);
    painter->drawText(textRect, Qt::AlignCenter, "Series");

    painter->restore();
}

void Series::drawDown(QPainter *painter)
{
    painter->save();

    double space = 50;
    QRectF textRect(0, 0, space, margin_down);

    for (int i = 0; i < center_width; i += space)
    {
        textRect.moveCenter(QPoint(margin_left + i, height() - margin_down / 2.0));
        painter->drawText(textRect, Qt::AlignCenter, QString::number(m_StartCol + i + 1));
    }

    painter->restore();
}

void Series::drawLeft(QPainter *)
{
}

void Series::drawRight(QPainter *)
{
}

void Series::updateStartCol(int StartCol)
{
    m_StartCol = StartCol;
    update();
}
