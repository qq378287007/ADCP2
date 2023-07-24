#include "InfoWidget.h"

#include <QLabel>
#include <QHBoxLayout>

InfoWidget::InfoWidget(QWidget *parent) : QScrollArea (parent)
{
    titleFont.setPointSize(18);
    contentFont.setPointSize(10);

    titlePe.setColor(QPalette::Window, Qt::blue);
    contentPe.setColor(QPalette::WindowText,Qt::red);

    infos.insert("File", {{"Start time", "2023/5/20 6:00:00"},
                          {"End time", "2023/5/21 14:00:00"}});
    infos.insert("System", {{"Serial number", "17195"},
                            {"Model", "Workhorse"},
                            {"Frequency(kHz)", "614"},
                            {"Firmware version", "51.4"}});
    infos.insert("Setup", {{"Model", "1"},
                           {"Number of cells", "12(WN)"},
                           {"Cell size(m)", "0.20(WS)"},
                           {"First cell range(m)", "1.31"}});
    infos.insert("Timing", {{"Sample time", "2023/5/20 6:00:00"}});
    infos.insert("Profile", {{"Mean speed(m/s)", "0.920"},
                             {"Mean direction(°)", "262.89"}});
    infos.insert("Compass", {{"Heading(°)", "49.07"},
                             {"Pitch(°)", "-49.07"},
                             {"Roll(°)", "49.07"}});
    infos.insert("Sensors", {{"Temperature(°)", "49.07"},
                             {"Pressure(kPa)", "5.1"},
                             {"Pressure depth(m)", "0.10"},
                             {"Voltage(V)", "51.1"}});
    infos.insert("Bottom track", {{"Mode", "50"},
                                  {"Avg.beam depth(m)", "22.74"},
                                  {"BT speed(m/s)", "0.067"},
                                  {"BT direction(°)", "19.25"}});
    infos.insert("Boundary", {{"Water depth(m)", "22.74"}});
    infos.insert("Boundary", {{"Built in test(hex)", "0"}});

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    for (const auto& key : infos.keys()) {
        QLabel *title = new QLabel(key, this);
        title->setFont(titleFont);
        title->setAutoFillBackground(true);
        title->setPalette(titlePe);
        mainLayout->addWidget(title);

        for(const auto& pair :infos.value(key)){
            QLabel *left = new QLabel(QString("  %1").arg(pair.first), this);
            left->setFont(contentFont);
            left->setPalette(contentPe);
            QLabel *right = new QLabel(QString("%1").arg(pair.second), this);
            right->setFont(contentFont);
            right->setPalette(contentPe);
            QHBoxLayout *tmp = new QHBoxLayout();
            tmp->addWidget(left);
            tmp->addStretch();
            tmp->addWidget(right);
            mainLayout->addLayout(tmp);
        }
    }
    mainLayout->addStretch();

    widget = new QWidget(this);
    widget->setLayout(mainLayout);
    setWidget(widget);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignCenter); // 居中对齐
    setWidgetResizable(true); // 自动调整大小
    setMinimumSize(300, 60);

    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::black);
    setPalette(palette);
}
