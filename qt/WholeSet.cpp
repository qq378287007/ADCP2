#include <QEvent>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

#include "data2image.h"

#include "WholeSet.h"

WholeSet::WholeSet(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(min_width, min_height);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);

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

void WholeSet::setFileData(const QSharedPointer<FileData> &FileData)
{
    m_FileData = FileData;
    str = "V1";
    data_width = m_FileData->Ping_Num;

    m_StartCol = 0;
    m_LeftCol = 0;
    m_RightCol = width() - margin_left - margin_right - 1;
    update();
}

void WholeSet::contextMenuEvent(QContextMenuEvent *event)
{
    menu->move(event->globalX(), event->globalY());
    menu->show();
}

bool WholeSet::eventFilter(QObject *watched, QEvent *event)
{
    if (m_FileData.isNull())
        return true;

    switch (event->type())
    {
    case QEvent::Paint:
    {
        center_width = width() - margin_left - margin_right;
        center_height = height() - margin_up - margin_down;
        center_rect.setSize(QSize(center_width, center_height));

        if (m_StartCol < 0)
            m_StartCol = 0;
        else if (m_StartCol > data_width - center_width)
            m_StartCol = data_width - center_width;

        if(m_LeftCol<0){
            m_RightCol -= m_LeftCol;
            m_LeftCol = 0;
        }else if(m_RightCol>center_width-1){
            m_LeftCol -= (m_RightCol-(center_width-1));
            m_RightCol = center_width-1;
        }

        if(m_LeftCol<0)
            m_LeftCol = 0;
        else if(m_RightCol>center_width-1)
            m_RightCol = center_width-1;

        QPainter painter(this);
        drawCenter(&painter);
        drawUp(&painter);
        drawDown(&painter);
        drawLeft(&painter);

        emit updateStartCol(m_StartCol);
        emit updateStartEndCol(m_StartCol+m_LeftCol, m_StartCol+m_RightCol);
        break;
    }
    case QEvent::KeyPress:
    {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);
        switch (e->key())
        {
        case Qt::Key_Left:
            m_StartCol++;
            update();
            break;
        case Qt::Key_Right:
            m_StartCol--;
            update();
            break;
        }
        break;
    }
    case QEvent::MouseButtonPress:
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if (center_rect.contains(e->pos()))
        {
            press_x = e->x();
            press_flag = true;

            int center_col = (m_LeftCol + m_RightCol) / 2;
            int move_x = press_x - margin_left - center_col;
            m_LeftCol += move_x;
            m_RightCol += move_x;
            update();
        }

        break;
    }
    case QEvent::MouseButtonRelease:
    {
        press_flag = false;
        break;
    }
    case QEvent::MouseMove:
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if (press_flag &&
                center_rect.contains(e->pos()) &&
                (e->buttons() & Qt::LeftButton))
        {
            m_LeftCol += e->x() - press_x;
            m_RightCol += e->x() - press_x;
            update();
            press_x = e->x();
        }
        break;
    }
    case QEvent::Wheel:
    {
        QWheelEvent *e = static_cast<QWheelEvent *>(event);
        int x = e->x() - margin_left;
        if(x>=m_LeftCol && x<=m_RightCol)
        {
            if (e->delta() > 0) // 滚轮远离，放大
            {
                m_LeftCol = x - 1/1.1 * (x - m_LeftCol);
                m_RightCol = x + 1/1.1 * (m_RightCol - x);

            }
            else // 滚轮靠近，缩小
            {
                m_LeftCol = x - 1.1 * (x - m_LeftCol);
                m_RightCol = x + 1.1 * (m_RightCol - x);

                if(m_LeftCol == m_RightCol){
                    m_LeftCol--;
                    m_RightCol++;
                }
            }
            update();
        }
        break;
    }
    default:
        break;
    }

    return QWidget::eventFilter(watched, event);
}

void WholeSet::drawCenter(QPainter *painter)
{
    double *data = m_FileData->str2Data(str);
    if (data == nullptr)
        return;

    int m_StartRow = 0;
    int m_EndRow = m_FileData->cellNum - 1;

    double min_value;
    double max_value;
    m_FileData->getMinMax(str, min_value, max_value);

    QImage image = getContour(data, data_width,
                              m_StartRow, m_EndRow,
                              m_StartCol, m_StartCol + center_width - 1,
                              min_value, max_value,
                              center_width, center_height);

    for (int y = 0; y < center_height; y++)
    {
        for (int x = m_LeftCol; x <= m_RightCol; x++)
        {
            QColor color = image.pixelColor(x, y);
            color.setAlpha(100);
            image.setPixelColor(x, y, color);
        }
    }
    painter->drawImage(margin_left, margin_up, image);
}

void WholeSet::drawUp(QPainter *painter)
{
    painter->save();

    QRect textRect(margin_left, 0, center_width, margin_up);
    painter->drawText(textRect, Qt::AlignCenter, "WholeSet");

    painter->restore();
}

void WholeSet::drawDown(QPainter *painter)
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

void WholeSet::drawLeft(QPainter *painter)
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

void WholeSet::drawRight(QPainter *)
{

}

void WholeSet::updateStartColSlot(int StartCol)
{
    m_StartCol = StartCol;
    update();
}

