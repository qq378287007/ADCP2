#pragma once

#include <QWidget>
#include <QSharedPointer>

#include <QMenu>
#include <QAction>

#include "FileData.h"

class WholeSet : public QWidget
{
    Q_OBJECT
public:
    explicit WholeSet(QWidget *parent = nullptr);
    void setFileData(const QSharedPointer<FileData> &FileData);

    int centerWidth() const {
        return width() - margin_left - margin_right;
    }

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    bool eventFilter(QObject *, QEvent *) override;

private:
    void drawCenter(QPainter *);
    void drawUp(QPainter *);
    void drawDown(QPainter *);
    void drawLeft(QPainter *);
    void drawRight(QPainter *);

signals:
    void updateStartCol(int);
    void updateStartEndCol(int, int);
public slots:
    void updateStartColSlot(int StartCol);


private:
    QMenu *menu;

private:
    const int margin_up = 20;
    const int margin_down = 20;
    const int margin_left = 20;
    const int margin_right = 2;
    const int min_width = margin_left + 400 + margin_right;
    const int min_height = margin_up + 12 * 9 + margin_down;

    int center_width;
    int center_height;
    QRect center_rect{margin_left, margin_up, 0, 0};

    QSharedPointer<FileData> m_FileData;
    QString str;
    int data_width;

    int m_StartCol;

    int m_LeftCol;
    int m_RightCol;

    int press_x;
    bool press_flag;
};
