#pragma once

#include <QScrollArea>
#include <QVBoxLayout>

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

};
