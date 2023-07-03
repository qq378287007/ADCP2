#pragma once

#include <QString>
#include <QMap>
#include <QImage>
#include <QSet>

class FileData
{
public:
    struct MinMaxValue
    {
        double min_value;
        double max_value;
    };

    QMap<QString, MinMaxValue> m_Str2MinMaxValue;
    QMap<QString, double *> m_Str2Data;

    double *str2Data(const QString &str) const {
        return m_Str2Data.value(str, nullptr);
    }

    void getMinMax(const QString &str, double &min_value, double &max_value);

public:
    explicit FileData(const QString &FileName);
    ~FileData();

public:
    void exportTxt(const QString &FileName);
    void exportMat(const QString &FileName);

    void exportTxt(const QString &FileName, const QSet<QString> &m_CheckSet);
    void exportMat(const QString &FileName, const QSet<QString> &m_CheckSet);
public:
    FILE *fid;

    int cellNum;    // 行数，12
    int pingLength; // 帧长
    int count;      // 文件字节数
    int Ping_Num;   // 列数，31700

    // 测量参数设置
    // Ping_Num
    double *RTC_year;
    double *RTC_month;
    double *RTC_day;
    double *RTC_hour;
    double *RTC_min;
    double *RTC_sec;

    // 流速剖面数据
    // cellNum * Ping_Num
    double *V1;
    double *V2;
    double *V3;
    double *V4;

    // 回波强度剖面数据
    // cellNum * Ping_Num
    double *E1;
    double *E2;
    double *E3;
    double *E4;

    // 相关系数剖面数据
    // cellNum * Ping_Num
    double *Cor1;
    double *Cor2;
    double *Cor3;
    double *Cor4;

    // 底跟踪数据
    // Ping_Num
    double *BTV1;
    double *BTV2;
    double *BTV3;
    double *BTV4;

    double *BTDepth1;
    double *BTDepth2;
    double *BTDepth3;
    double *BTDepth4;

    double *BTE1;
    double *BTE2;
    double *BTE3;
    double *BTE4;

    double *BTCor1;
    double *BTCor2;
    double *BTCor3;
    double *BTCor4;

    double *BTP1;
    double *BTP2;
    double *BTP3;
    double *BTP4;

    // 传感器数据（磁罗盘、温度、压力及GPS数据）
    // Ping_Num
    double *Pressure;
    double *Temp;
    double *Roll;
    double *Pitch;
    double *Heading;

    int min_bin;
    int max_bin;
    int min_ensemble;
    int max_ensemble;
};
