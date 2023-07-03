#pragma once

#include <QWidget>
#include <QSharedPointer>

#include <QMenu>
#include <QAction>

#include "FileData.h"

class Series : public QWidget
{
    Q_OBJECT
public:
    explicit Series(QWidget *parent = nullptr);
    void setFileData(const QSharedPointer<FileData> &FileData);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    bool eventFilter(QObject *, QEvent *) override;

private:
    void drawCenter(QPainter *);
    void drawUp(QPainter *);
    void drawDown(QPainter *);
    void drawLeft(QPainter *);
    void drawRight(QPainter *);

public slots:
    void updateStartCol(int StartCol);

private:
    QMenu *menu;

private:
    QSharedPointer<FileData> m_FileData;
    QString str;

    const int margin_up = 20;
    const int margin_down = 20;
    const int margin_left = 2;
    const int margin_right = 2;
    const int min_width = margin_left + 400 + margin_right;
    const int min_height = margin_up + 12 * 9 + margin_down;

    int center_width;
    int center_height;

    int m_StartCol;
};
