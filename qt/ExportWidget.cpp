#include "ExportWidget.h"

#include <QIntValidator>
#include <QDebug>
#include <QScrollArea>

ExportWidget::ExportWidget(QWidget *parent) : QWidget(parent)
{
    m_FileType = new QGroupBox("File Type", this);

    m_txt = new QRadioButton("TXT", m_FileType);
    m_mat = new QRadioButton("MAT", m_FileType);
    QVBoxLayout *fileTypeLayout = new QVBoxLayout(m_FileType);
    fileTypeLayout->addWidget(m_txt);
    fileTypeLayout->addWidget(m_mat);
    m_txt->setChecked(true);

    m_Bins = new QGroupBox("Bins", this);
    selectAll = new QPushButton("All", this);
    selectNone = new QPushButton("None", this);
    /*
    m_MinBin = new QLineEdit(this);
    m_MaxBin = new QLineEdit(this);
    m_MinBin->setValidator(new QIntValidator(1, 12, this));
    m_MinBin->setMaxLength(2);
    m_MinBin->setMaximumWidth(25);
    m_MaxBin->setValidator(new QIntValidator(1, 12, this));
    m_MaxBin->setMaxLength(2);
    m_MaxBin->setMaximumWidth(25);
*/

    QHBoxLayout *Bins_Select_UpLayout = new QHBoxLayout();
    Bins_Select_UpLayout->addWidget(selectAll);
    Bins_Select_UpLayout->addWidget(selectNone);

    /*
    QHBoxLayout *Bins_UpLayout = new QHBoxLayout();
    Bins_UpLayout->addWidget(new QLabel("MinBin: ", m_Bins));
    Bins_UpLayout->addWidget(m_MinBin);
    Bins_UpLayout->addStretch();

    QHBoxLayout *Bins_DownLayout = new QHBoxLayout();
    Bins_DownLayout->addWidget(new QLabel("MaxBin: ", m_Bins));
    Bins_DownLayout->addWidget(m_MaxBin);
    Bins_DownLayout->addStretch();
    */

    QVBoxLayout *BinsLayout = new QVBoxLayout(m_Bins);
    BinsLayout->addLayout(Bins_Select_UpLayout);

    //多个深度选择
    QScrollArea *scrollArea  = new QScrollArea(this);
    QWidget * pWgt = new QWidget;
    QVBoxLayout *pLayout = new QVBoxLayout();//网格布局
    pLayout->addStretch();
    for(int i = 0; i < 12; i++)
    {
        QLabel *pBtn = new QLabel(QString("%1").arg(i+1));
        QCheckBox *pBox = new QCheckBox();

        QHBoxLayout *tmp  = new QHBoxLayout;
        tmp->addStretch();
        tmp->addWidget(pBtn, Qt::AlignRight);//把按钮添加到布局控件中
        tmp->addWidget(pBox, Qt::AlignLeft);//把按钮添加到布局控件中
        tmp->addStretch();
        pLayout->addLayout(tmp);
    }
    pLayout->addStretch();
    pWgt->setLayout(pLayout);

    //这一句setWidget必须放在pWgt里面的内容都准备完毕之后，否则显示有问题
    scrollArea->setWidget(pWgt);


    BinsLayout->addWidget(scrollArea);



    //BinsLayout->addLayout(Bins_UpLayout);
    //BinsLayout->addStretch();
    //BinsLayout->addLayout(Bins_DownLayout);

    m_Ensemble = new QGroupBox("Ensemble", this);
    m_MinEnsemble = new QLineEdit(this);
    m_MaxEnsemble = new QLineEdit(this);
    m_MinEnsemble->setValidator(new QIntValidator(1, 31700, this));
    m_MinEnsemble->setMaxLength(5);
    m_MinEnsemble->setMaximumWidth(50);
    m_MaxEnsemble->setValidator(new QIntValidator(1, 31700, this));
    m_MaxEnsemble->setMaxLength(5);
    m_MaxEnsemble->setMaximumWidth(50);

    QHBoxLayout *EnsembleLayout = new QHBoxLayout(m_Ensemble);
    EnsembleLayout->addWidget(new QLabel("MinEnsemble: ", m_Ensemble));
    EnsembleLayout->addWidget(m_MinEnsemble);
    EnsembleLayout->addStretch();
    EnsembleLayout->addWidget(new QLabel("MaxEnsemble: ", m_Ensemble));
    EnsembleLayout->addWidget(m_MaxEnsemble);

    m_DataType = new QGroupBox("Data Type", this);
    QGroupBox *m_Speed = new QGroupBox("流速剖面", this);
    QGroupBox *m_Strength = new QGroupBox("回波强度", this);
    QGroupBox *m_Cor = new QGroupBox("相关系数", this);
    QGroupBox *m_Bottom = new QGroupBox("底跟踪", this);
    QGroupBox *m_Sensor = new QGroupBox("传感器", this);
    QVBoxLayout *DataLayout = new QVBoxLayout(m_DataType);
    DataLayout->addWidget(m_Speed);
    DataLayout->addWidget(m_Strength);
    DataLayout->addWidget(m_Cor);
    DataLayout->addWidget(m_Bottom);
    DataLayout->addWidget(m_Sensor);
    DataLayout->addStretch();

    QStringList strList;

    QHBoxLayout *V_CBLayout = new QHBoxLayout(m_Speed);
    strList.clear();
    strList<<"V1"<<"V2"<<"V3"<<"V4";
    foreach(auto str, strList){
        QCheckBox *cb = new QCheckBox(str,this);
        V_CBLayout->addWidget(cb);
        connect(cb, &QCheckBox::clicked, this, &ExportWidget::CheckBox_Clicked);
    }
    V_CBLayout->addStretch();


    QHBoxLayout *E_CBLayout = new QHBoxLayout(m_Strength);
    strList.clear();
    strList<<"E1"<<"E2"<<"E3"<<"E4";
    foreach(auto str, strList){
        QCheckBox *cb = new QCheckBox(str,this);
        E_CBLayout->addWidget(cb);
        connect(cb, &QCheckBox::clicked, this, &ExportWidget::CheckBox_Clicked);
    }
    E_CBLayout->addStretch();


    QHBoxLayout *Cor_CBLayout = new QHBoxLayout(m_Cor);
    strList.clear();
    strList<<"Cor1"<<"Cor2"<<"Cor3"<<"Cor4";
    foreach(auto str, strList){
        QCheckBox *cb = new QCheckBox(str,this);
        Cor_CBLayout->addWidget(cb);
        connect(cb, &QCheckBox::clicked, this, &ExportWidget::CheckBox_Clicked);
    }
    Cor_CBLayout->addStretch();

    QGridLayout *Bottom_GLayout = new QGridLayout();
    QHBoxLayout *Bottom_CBLayout = new QHBoxLayout(m_Bottom);
    Bottom_CBLayout->addLayout(Bottom_GLayout);
    Bottom_CBLayout->addStretch();

    strList.clear();
    strList<<"BTV1"<<"BTV2"<<"BTV3"<<"BTV4";
    for(int i=0; i<strList.size(); i++){
        QCheckBox *cb = new QCheckBox(strList.at(i),this);
        Bottom_GLayout->addWidget(cb, 0, i);
        connect(cb, &QCheckBox::clicked, this, &ExportWidget::CheckBox_Clicked);
    }

    strList.clear();
    strList<<"BTDepth1"<<"BTDepth2"<<"BTDepth3"<<"BTDepth4";
    for(int i=0; i<strList.size(); i++){
        QCheckBox *cb = new QCheckBox(strList.at(i),this);
        Bottom_GLayout->addWidget(cb, 1, i);
        connect(cb, &QCheckBox::clicked, this, &ExportWidget::CheckBox_Clicked);
    }

    strList.clear();
    strList<<"BTE1"<<"BTE2"<<"BTE3"<<"BTE4";
    for(int i=0; i<strList.size(); i++){
        QCheckBox *cb = new QCheckBox(strList.at(i),this);
        Bottom_GLayout->addWidget(cb, 2, i);
        connect(cb, &QCheckBox::clicked, this, &ExportWidget::CheckBox_Clicked);
    }


    strList.clear();
    strList<<"BTCor1"<<"BTCor2"<<"BTCor3"<<"BTCor4";
    for(int i=0; i<strList.size(); i++){
        QCheckBox *cb = new QCheckBox(strList.at(i),this);
        Bottom_GLayout->addWidget(cb, 3, i);
        connect(cb, &QCheckBox::clicked, this, &ExportWidget::CheckBox_Clicked);
    }

    strList.clear();
    strList<<"BTP1"<<"BTP2"<<"BTP3"<<"BTP4";
    for(int i=0; i<strList.size(); i++){
        QCheckBox *cb = new QCheckBox(strList.at(i),this);
        Bottom_GLayout->addWidget(cb, 4, i);
        connect(cb, &QCheckBox::clicked, this, &ExportWidget::CheckBox_Clicked);
    }

    QHBoxLayout *SensorLayout = new QHBoxLayout(m_Sensor);
    strList.clear();
    strList<<"Pressure"<<"Temp"<<"Roll"<<"Pitch"<<"Heading";
    foreach(auto str, strList){
        QCheckBox *cb = new QCheckBox(str,this);
        SensorLayout->addWidget(cb);
        connect(cb, &QCheckBox::clicked, this, &ExportWidget::CheckBox_Clicked);
    }
    SensorLayout->addStretch();

    upLayout = new QGridLayout();
    upLayout->addWidget(m_FileType, 0, 0);
    upLayout->addWidget(m_Ensemble, 0, 1);
    upLayout->addWidget(m_Bins, 1, 0);
    upLayout->addWidget(m_DataType, 1, 1);

    upLayout->setColumnStretch(1, 1);

    okBtn = new QPushButton("导出", this);
    cancelBtn = new QPushButton("取消", this);
    downLayout = new QHBoxLayout();
    downLayout->addStretch();
    downLayout->addWidget(okBtn);
    downLayout->addWidget(cancelBtn);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(upLayout, 1);
    mainLayout->addLayout(downLayout);

    setMinimumSize(600, 600);

    connect(cancelBtn, &QPushButton::clicked, [=]()
    { hide(); });
    connect(okBtn, &QPushButton::clicked, [=]()
    {
        if(!m_CheckSet.empty()){
            if(m_txt->isChecked()){
                m_FileData->exportTxt("data.txt", m_CheckSet);
            }else{
                m_FileData->exportMat("data.mat", m_CheckSet);
            }
        }
        hide(); });
}

void ExportWidget::CheckBox_Clicked()
{
    QCheckBox *cb = static_cast<QCheckBox *>(sender());
    QString str = cb->text();

    if(cb->isChecked())
        m_CheckSet.insert(str);
    else
        m_CheckSet.remove(str);
}
