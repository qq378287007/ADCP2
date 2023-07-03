#include <QFileDialog>
#include <QDebug>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    openFileAction = new QAction(tr("打开"), this);
    exportFileAction = new QAction(tr("导出"), this);

    // 文件菜单
    fileMenu = menuBar()->addMenu(tr("文件"));  //(a)
    fileMenu->addAction(openFileAction);       //(b)
    fileMenu->addAction(exportFileAction);

    m_WholeSet = new WholeSet(this);
    m_Slider = new QSlider(Qt::Horizontal, this);
    m_SubSet = new SubSet(this);
    m_Series = new Series(this);
    rightLayout = new QVBoxLayout();
    rightLayout->addWidget(m_WholeSet);
    rightLayout->addWidget(m_Slider);
    rightLayout->addWidget(m_SubSet);
    rightLayout->addWidget(m_Series);

    m_Profile = new Profile(this);
    upLayout = new QHBoxLayout();
    upLayout->addWidget(m_Profile);
    upLayout->addLayout(rightLayout, 1);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(upLayout);
    mainLayout->addStretch(1);

    widget = new QWidget(this);
    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    m_Slider->hide();

    exportWidget = new ExportWidget();
    exportWidget->setWindowModality(Qt::ApplicationModal);
    exportWidget->hide();

    connect(openFileAction, &QAction::triggered, [&]
            {
        QString fileName = QFileDialog::getOpenFileName(this, tr("打开ADCP文件"), ".", "二进制文件(*.dat)");
        if(!fileName.isEmpty()){
            m_FileData.reset(new FileData(fileName));

            exportWidget->setFileData(m_FileData);
            m_WholeSet->setFileData(m_FileData);
            m_SubSet->setFileData(m_FileData);
            m_Profile->setFileData(m_FileData);
            m_Series->setFileData(m_FileData);
        } });

    connect(exportFileAction, &QAction::triggered, [&]
            {
                if (m_FileData.isNull())
                    return;

                exportWidget->show();
            });

    connect(m_WholeSet, &WholeSet::updateStartEndCol, m_SubSet, &SubSet::updateStartEndCol);
    connect(m_WholeSet, &WholeSet::updateStartCol, m_Series, &Series::updateStartCol);


    connect(m_WholeSet, &WholeSet::updateStartCol, [&](int startCol){
        m_Slider->setRange(0, m_FileData->Ping_Num - m_WholeSet->centerWidth() - 1);
        m_Slider->setValue(startCol);
        if(m_Slider->isHidden())
            m_Slider->setVisible(true);
    });

    connect(m_Slider, SIGNAL(valueChanged(int)), m_WholeSet, SLOT(updateStartColSlot(int)));
}

MainWindow::~MainWindow()
{
    //delete exportWidget;
}
