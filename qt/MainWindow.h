#pragma once

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include <QSharedPointer>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QWidget>
#include <QSlider>

#include "FileData.h"

#include "ExportWidget.h"
#include "InfoWidget.h"

#include "WholeSet.h"

#include "SubSet.h"

#include "Series.h"
#include "Profile.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // 文件菜单项
    QAction *openFileAction;
    QAction *exportFileAction;
    QAction *infoAction;

    // 各项菜单栏
    QMenu *fileMenu;

private:
    QWidget *widget;
    QVBoxLayout *rightLayout;
    WholeSet *m_WholeSet;
    QSlider *m_Slider;
    SubSet *m_SubSet;

    Profile *m_Profile;
    Series *m_Series;
    QHBoxLayout *upLayout;

    QVBoxLayout *mainLayout;

    ExportWidget *exportWidget;
    InfoWidget *infoWidget;

private:
    QSharedPointer<FileData> m_FileData;
};
