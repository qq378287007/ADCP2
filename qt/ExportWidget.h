#pragma once

#include <QWidget>

#include <QLabel>
#include <QLineEdit>

#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QVector>

#include <QSharedPointer>
#include <QSet>

#include "FileData.h"

class ExportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExportWidget(QWidget *parent = nullptr);

public:
    void setFileData(const QSharedPointer<FileData> &FileData)
    {
        m_FileData = FileData;
    }
public slots:
    void CheckBox_Clicked();
private:
    QGroupBox *m_FileType;
    QRadioButton *m_txt;
    QRadioButton *m_mat;

    QGroupBox *m_Bins;
    QPushButton *selectAll;
    QPushButton *selectNone;
    QLineEdit *m_MinBin;
    QLineEdit *m_MaxBin;
    QVector<QCheckBox *> cbs;

    QGroupBox *m_Ensemble;
    QLineEdit *m_MinEnsemble;
    QLineEdit *m_MaxEnsemble;

    QGroupBox *m_DataType;

    QGridLayout *upLayout;

    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QHBoxLayout *downLayout;

    QVBoxLayout *mainLayout;

    QSharedPointer<FileData> m_FileData;
    QSet<QString> m_CheckSet;
};
