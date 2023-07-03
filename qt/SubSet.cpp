#include <QPainter>
#include <QDebug>
#include <QContextMenuEvent>

#include "data2image.h"
#include "SubSet.h"

SubSet::SubSet(QWidget *parent) : QWidget(parent)
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
    strList << "V1"
            << "V2"
            << "V3"
            << "V4";
    QMenu *m_VMenu = new QMenu("V", this);
    foreach (auto str, strList)
        m_VMenu->addAction(new QAction(str, this));
    menu->addMenu(m_VMenu);

    strList.clear();
    strList << "E1"
            << "E2"
            << "E3"
            << "E4";
    QMenu *m_EMenu = new QMenu("E", this);
    foreach (auto str, strList)
        m_EMenu->addAction(new QAction(str, this));
    menu->addMenu(m_EMenu);

    strList.clear();
    strList << "Cor1"
            << "Cor2"
            << "Cor3"
            << "Cor4";
    QMenu *m_CorMenu = new QMenu("Cor", this);
    foreach (auto str, strList)
        m_CorMenu->addAction(new QAction(str, this));
    menu->addMenu(m_CorMenu);

    connect(menu, &QMenu::triggered, [&](QAction *action)
            {
        str = action->text();
        update(); });
}

void SubSet::setFileData(const QSharedPointer<FileData> &FileData)
{
    m_FileData = FileData;
    str = "V1";
    data_width = m_FileData->Ping_Num;
    updateStartEndCol(0, width()-margin_left-margin_right-1);
}

void SubSet::contextMenuEvent(QContextMenuEvent *event)
{
    menu->move(event->globalX(), event->globalY());
    menu->show();
}

bool SubSet::eventFilter(QObject *watched, QEvent *event)
{
    if (m_FileData.isNull())
        return true;

    if (event->type() == QEvent::Paint)
    {
        center_width = width() - margin_left - margin_right;
        center_height = height() - margin_up - margin_down;

        if (m_StartCol < 0)
            m_StartCol = 0;
        else if (m_StartCol > m_FileData->Ping_Num - 1)
            m_StartCol = m_FileData->Ping_Num - 1;

        QPainter painter(this);
        drawCenter(&painter);
        drawUp(&painter);
        drawDown(&painter);
        drawLeft(&painter);
    }
    return QWidget::eventFilter(watched, event);
}

void SubSet::drawCenter(QPainter *painter)
{
    painter->save();

    double *data = m_FileData->str2Data(str);

    int start_row = 0;
    int end_row = m_FileData->cellNum - 1;

    double min_value;
    double max_value;
    m_FileData->getMinMax(str, min_value, max_value);

    QImage image = getContour(data, data_width,
                              start_row, end_row,
                              m_StartCol, m_EndCol,
                              min_value, max_value,
                              center_width, center_height);
    painter->drawImage(margin_left, margin_up, image);

    painter->restore();
}

void SubSet::drawUp(QPainter *painter)
{
    painter->save();

    QRect textRect(margin_left, 0, center_width, margin_up);
    painter->drawText(textRect, Qt::AlignCenter, "SubSet");

    painter->restore();
}

void SubSet::drawDown(QPainter *painter)
{
    painter->save();

    double space = 50;
    QRectF textRect(0, 0, space, margin_down);

    for (int i = 0; i < center_width; i += space)
    {
        textRect.moveCenter(QPoint(margin_left + i, height() - margin_down / 2.0));
        int t = qRound(i/(center_width-1.0) * (m_EndCol-m_StartCol) + m_StartCol);
        painter->drawText(textRect, Qt::AlignCenter, QString::number(t));
    }

    painter->restore();
}

void SubSet::drawLeft(QPainter *painter)
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

void SubSet::drawRight(QPainter *)
{
}

void SubSet::updateStartEndCol(int StartCol, int EndCol)
{
    m_StartCol = StartCol;
    m_EndCol = EndCol;
    update();
}
