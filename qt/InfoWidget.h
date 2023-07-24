#pragma once

#include <QScrollArea>
#include <QVBoxLayout>

#include <QHash>
#include <QString>
#include <QVector>
#include <QPair>

#include "FileData.h"

class InfoWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = nullptr);

public:
    void setFileData(const QSharedPointer<FileData> &FileData)
    {
        m_FileData = FileData;
    }
private:
    QSharedPointer<FileData> m_FileData;

    QWidget *widget;
    QVBoxLayout *mainLayout;

    QHash<QString, QVector<QPair<QString, QString>>> infos;

    QFont titleFont;
    QFont contentFont;

    QPalette titlePe;
    QPalette contentPe;
};
