#include <stdio.h>
#include <stdlib.h>

#include <QDebug>

#include "mat.h"

#include "FileData.h"

static void fskip(FILE *stream, long offset)
{
    static unsigned char uchar_data[256];
    fread(uchar_data, 1, offset, stream);
}

static int freadUint8ToInt(FILE *stream)
{
    unsigned char uchar_data = 0;
    fread(&uchar_data, 1, 1, stream);
    return uchar_data;
}

static int freadUint8ToDouble(FILE *stream)
{
    unsigned char uchar_data = 0;
    fread(&uchar_data, 1, 1, stream);
    return uchar_data * 1.0;
}

static double freadUint16ToDouble(FILE *stream)
{
    unsigned short ushort_data = 0;
    fread(&ushort_data, 2, 1, stream);
    return ushort_data * 1.0;
}

static double freadInt16ToDouble(FILE *stream)
{
    short short_data = 0;
    fread(&short_data, 2, 1, stream);
    return short_data * 1.0;
}

static void fwriteDouble1(FILE *fp, const char *title, double *data, int min_ensemble, int max_ensemble)
{
    fprintf(fp, "#%s\n", title);
    for (int row = min_ensemble; row <= max_ensemble; row++)
    {
        fprintf(fp, "%7.3lf", data[row]);
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
}

static void fwriteDouble2(FILE *fp, const char *title, double *data, int cellNum,
                          int min_bin, int max_bin, int min_ensemble, int max_ensemble)
{
    fprintf(fp, "#%s\n", title);
    for (int row = min_ensemble; row <= max_ensemble; row++)
    {
        for (int col = min_bin; col <= max_bin; col++)
            fprintf(fp, "%7.3lf", data[cellNum * row + col]);
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
}

static void fwriteMat(MATFile *fd, const char *name, double *data, int height, int width)
{
    mxArray *array = mxCreateDoubleMatrix(width, height, mxREAL);
    // memcpy((void *)(mxGetPr(array)), (void *)(data), sizeof(double) * width * height );
    mxSetData(array, (void *)data);
    matPutVariable(fd, name, array);

    mxSetData(array, NULL);
    mxDestroyArray(array);
}

static void fwriteMatDouble1(MATFile *fd, const char *name, double *src, int min_ensemble, int max_ensemble)
{
    int height = 1;
    int width = max_ensemble - min_ensemble + 1;

    double *data = (double *)malloc(sizeof(double) * height * width);
    for (int row = min_ensemble; row <= max_ensemble; row++)
        data[row - min_ensemble] = src[row] * 1.0;

    fwriteMat(fd, name, data, height, width);

    free(data);
}

static void fwriteMatDouble2(MATFile *fd, const char *name, double *src, int cellNum,
                             int min_bin, int max_bin, int min_ensemble, int max_ensemble)
{
    int width = max_bin - min_bin + 1;
    int height = max_ensemble - min_ensemble + 1;

    double *data = (double *)malloc(sizeof(double) * width * height);
    for (int row = min_ensemble; row <= max_ensemble; row++)
        for (int col = min_bin; col <= max_bin; col++)
            data[width * (row - min_ensemble) + col - min_bin] = src[cellNum * row + col];

    fwriteMat(fd, name, data, height, width);

    free(data);
}

void FileData::getMinMax(const QString &str, double &min_value, double &max_value)
{
    if (m_Str2MinMaxValue.contains(str))
    {
        MinMaxValue mmV = m_Str2MinMaxValue.take(str);
        min_value = mmV.min_value;
        max_value = mmV.max_value;
        return;
    }

    int num = Ping_Num;

    if (str == "V1" || str == "V2" || str == "V3" || str == "V4" ||
        str == "E1" || str == "E2" || str == "E3" || str == "E4" ||
        str == "Cor1" || str == "Cor2" || str == "Cor3" || str == "Cor4")
    {
        num *= cellNum;
    }

    min_value = 999999.9;
    max_value = -999999.9;
    double *data = m_Str2Data[str];
    for (int i = 0; i < num; i++)
    {
        double data_value = data[i];
        if (data_value < min_value)
            min_value = data_value;
        else if (data_value > max_value)
            max_value = data_value;
    }
    m_Str2MinMaxValue.insert(str, {min_value, max_value});
}

FileData::FileData(const QString &FileName)
{
    fid = fopen(FileName.toStdString().c_str(), "rb");

    fseek(fid, 38, SEEK_SET);
    cellNum = freadUint8ToInt(fid);
    printf("cellNum=%d\n", cellNum);

    pingLength = 32 + 32 + (2 + cellNum * 8) * 2 + (2 + cellNum * 4) + 48 + 32 + 16;
    printf("pingLength=%d\n", pingLength);

    fseek(fid, 0, SEEK_END);
    count = ftell(fid);
    printf("count=%d\n", count);

    Ping_Num = count / (32 + 32 + 2 * (cellNum * 8 + 2) + (cellNum * 4 + 2) + 96);
    printf("Ping_Num=%d\n", Ping_Num);

    // 测量参数设置
    RTC_year = (double *)malloc(sizeof(double) * Ping_Num);
    RTC_month = (double *)malloc(sizeof(double) * Ping_Num);
    RTC_day = (double *)malloc(sizeof(double) * Ping_Num);
    RTC_hour = (double *)malloc(sizeof(double) * Ping_Num);
    RTC_min = (double *)malloc(sizeof(double) * Ping_Num);
    RTC_sec = (double *)malloc(sizeof(double) * Ping_Num);

    // 流速剖面数据
    V1 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);
    V2 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);
    V3 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);
    V4 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);

    // 回波强度剖面数据
    E1 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);
    E2 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);
    E3 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);
    E4 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);

    // 相关系数剖面数据
    Cor1 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);
    Cor2 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);
    Cor3 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);
    Cor4 = (double *)malloc(sizeof(double) * cellNum * Ping_Num);

    // 底跟踪数据
    BTV1 = (double *)malloc(sizeof(double) * Ping_Num);
    BTV2 = (double *)malloc(sizeof(double) * Ping_Num);
    BTV3 = (double *)malloc(sizeof(double) * Ping_Num);
    BTV4 = (double *)malloc(sizeof(double) * Ping_Num);

    BTDepth1 = (double *)malloc(sizeof(double) * Ping_Num);
    BTDepth2 = (double *)malloc(sizeof(double) * Ping_Num);
    BTDepth3 = (double *)malloc(sizeof(double) * Ping_Num);
    BTDepth4 = (double *)malloc(sizeof(double) * Ping_Num);

    BTE1 = (double *)malloc(sizeof(double) * Ping_Num);
    BTE2 = (double *)malloc(sizeof(double) * Ping_Num);
    BTE3 = (double *)malloc(sizeof(double) * Ping_Num);
    BTE4 = (double *)malloc(sizeof(double) * Ping_Num);

    BTCor1 = (double *)malloc(sizeof(double) * Ping_Num);
    BTCor2 = (double *)malloc(sizeof(double) * Ping_Num);
    BTCor3 = (double *)malloc(sizeof(double) * Ping_Num);
    BTCor4 = (double *)malloc(sizeof(double) * Ping_Num);

    BTP1 = (double *)malloc(sizeof(double) * Ping_Num);
    BTP2 = (double *)malloc(sizeof(double) * Ping_Num);
    BTP3 = (double *)malloc(sizeof(double) * Ping_Num);
    BTP4 = (double *)malloc(sizeof(double) * Ping_Num);

    // 传感器数据（磁罗盘、温度、压力及GPS数据）
    Pressure = (double *)malloc(sizeof(double) * Ping_Num);
    Temp = (double *)malloc(sizeof(double) * Ping_Num);
    Roll = (double *)malloc(sizeof(double) * Ping_Num);
    Pitch = (double *)malloc(sizeof(double) * Ping_Num);
    Heading = (double *)malloc(sizeof(double) * Ping_Num);

    fseek(fid, 0, SEEK_SET);
    for (int ind = 0; ind < Ping_Num; ind++)
    {
        // 数据头文件
        fskip(fid, 2);

        fskip(fid, 2);

        fskip(fid, 2);
        fskip(fid, 2);
        fskip(fid, 2);
        fskip(fid, 2);
        fskip(fid, 2);
        fskip(fid, 2);
        fskip(fid, 2);

        fskip(fid, 14);

        // 测量参数设置
        fskip(fid, 2);
        fskip(fid, 2);
        fskip(fid, 2);
        fskip(fid, 1);
        fskip(fid, 2);

        fskip(fid, 1);
        fskip(fid, 1);

        fskip(fid, 1);
        fskip(fid, 1);
        fskip(fid, 1);
        fskip(fid, 1);
        fskip(fid, 1);
        fskip(fid, 1);

        fskip(fid, 1);
        fskip(fid, 1);
        fskip(fid, 1);

        fskip(fid, 1);
        fskip(fid, 1);
        fskip(fid, 1);

        fskip(fid, 2);

        RTC_year[ind] = freadUint8ToDouble(fid);
        RTC_month[ind] = freadUint8ToDouble(fid);
        RTC_day[ind] = freadUint8ToDouble(fid);
        RTC_hour[ind] = freadUint8ToDouble(fid);
        RTC_min[ind] = freadUint8ToDouble(fid);
        RTC_sec[ind] = freadUint8ToDouble(fid);

        fskip(fid, 1);

        // 流速剖面数据
        fskip(fid, 2);

        for (int index = 0; index < cellNum; index++)
        {
            V1[Ping_Num * index + ind] = freadInt16ToDouble(fid) / 1000.0;
            V2[Ping_Num * index + ind] = freadInt16ToDouble(fid) / 1000.0;
            V3[Ping_Num * index + ind] = freadInt16ToDouble(fid) / 1000.0;
            V4[Ping_Num * index + ind] = freadInt16ToDouble(fid) / 1000.0;
        }

        // 回波强度剖面数据
        fskip(fid, 2);

        for (int index = 0; index < cellNum; index++)
        {
            E1[Ping_Num * index + ind] = freadUint16ToDouble(fid);
            E2[Ping_Num * index + ind] = freadUint16ToDouble(fid);
            E3[Ping_Num * index + ind] = freadUint16ToDouble(fid);
            E4[Ping_Num * index + ind] = freadUint16ToDouble(fid);
        }

        // 相关系数剖面数据
        fskip(fid, 2);

        for (int index = 0; index < cellNum; index++)
        {
            Cor1[Ping_Num * index + ind] = freadUint8ToDouble(fid);
            Cor2[Ping_Num * index + ind] = freadUint8ToDouble(fid);
            Cor3[Ping_Num * index + ind] = freadUint8ToDouble(fid);
            Cor4[Ping_Num * index + ind] = freadUint8ToDouble(fid);
        }

        // 底跟踪数据
        fskip(fid, 2);

        BTV1[ind] = freadInt16ToDouble(fid);
        BTV2[ind] = freadInt16ToDouble(fid);
        BTV3[ind] = freadInt16ToDouble(fid);
        BTV4[ind] = freadInt16ToDouble(fid);

        BTDepth1[ind] = freadUint16ToDouble(fid) / 100.0;
        BTDepth2[ind] = freadUint16ToDouble(fid) / 100.0;
        BTDepth3[ind] = freadUint16ToDouble(fid) / 100.0;
        BTDepth4[ind] = freadUint16ToDouble(fid) / 100.0;

        BTE1[ind] = freadUint16ToDouble(fid);
        BTE2[ind] = freadUint16ToDouble(fid);
        BTE3[ind] = freadUint16ToDouble(fid);
        BTE4[ind] = freadUint16ToDouble(fid);

        BTCor1[ind] = freadUint8ToDouble(fid);
        BTCor2[ind] = freadUint8ToDouble(fid);
        BTCor3[ind] = freadUint8ToDouble(fid);
        BTCor4[ind] = freadUint8ToDouble(fid);

        BTP1[ind] = freadUint8ToDouble(fid);
        BTP2[ind] = freadUint8ToDouble(fid);
        BTP3[ind] = freadUint8ToDouble(fid);
        BTP4[ind] = freadUint8ToDouble(fid);

        fskip(fid, 14);

        // 传感器数据（磁罗盘、温度、压力及GPS数据）
        fskip(fid, 2);

        Pressure[ind] = freadUint16ToDouble(fid) / 10.0;
        Temp[ind] = freadInt16ToDouble(fid) / 100.0;
        Roll[ind] = freadInt16ToDouble(fid) / 10.0;
        Pitch[ind] = freadInt16ToDouble(fid) / 10.0;
        Heading[ind] = freadInt16ToDouble(fid) / 100.0;

        fskip(fid, 1);
        fskip(fid, 2);
        fskip(fid, 4);

        fskip(fid, 1);
        fskip(fid, 2);
        fskip(fid, 4);

        fskip(fid, 6);

        // 数据尾及校验和数据
        fskip(fid, 2);
        fskip(fid, 2);
        fskip(fid, 10);
        fskip(fid, 2);
    }
    fclose(fid);

    min_bin = 0;
    max_bin = cellNum - 1;
    min_ensemble = 0;
    max_ensemble = Ping_Num - 1;

    m_Str2Data.insert("V1", V1);
    m_Str2Data.insert("V2", V2);
    m_Str2Data.insert("V3", V3);
    m_Str2Data.insert("V4", V4);
    m_Str2Data.insert("E1", E1);
    m_Str2Data.insert("E2", E2);
    m_Str2Data.insert("E3", E3);
    m_Str2Data.insert("E4", E4);
    m_Str2Data.insert("Cor1", Cor1);
    m_Str2Data.insert("Cor2", Cor2);
    m_Str2Data.insert("Cor3", Cor3);
    m_Str2Data.insert("Cor4", Cor4);

    m_Str2Data.insert("BTV1", BTV1);
    m_Str2Data.insert("BTV2", BTV2);
    m_Str2Data.insert("BTV3", BTV3);
    m_Str2Data.insert("BTV4", BTV4);
    m_Str2Data.insert("BTDepth1", BTDepth1);
    m_Str2Data.insert("BTDepth2", BTDepth2);
    m_Str2Data.insert("BTDepth3", BTDepth3);
    m_Str2Data.insert("BTDepth4", BTDepth4);
    m_Str2Data.insert("BTE1", BTE1);
    m_Str2Data.insert("BTE2", BTE2);
    m_Str2Data.insert("BTE3", BTE3);
    m_Str2Data.insert("BTE4", BTE4);
    m_Str2Data.insert("BTCor1", BTCor1);
    m_Str2Data.insert("BTCor2", BTCor2);
    m_Str2Data.insert("BTCor3", BTCor3);
    m_Str2Data.insert("BTCor4", BTCor4);
    m_Str2Data.insert("BTP1", BTP1);
    m_Str2Data.insert("BTP2", BTP2);
    m_Str2Data.insert("BTP3", BTP3);
    m_Str2Data.insert("BTP4", BTP4);
    m_Str2Data.insert("Pressure", Pressure);
    m_Str2Data.insert("Temp", Temp);
    m_Str2Data.insert("Roll", Roll);
    m_Str2Data.insert("Pitch", Pitch);
    m_Str2Data.insert("Heading", Heading);
}

FileData::~FileData()
{
    free(Heading);
    free(Pitch);
    free(Roll);
    free(Temp);
    free(Pressure);

    free(BTP4);
    free(BTP3);
    free(BTP2);
    free(BTP1);

    free(BTCor4);
    free(BTCor3);
    free(BTCor2);
    free(BTCor1);

    free(BTE4);
    free(BTE3);
    free(BTE2);
    free(BTE1);

    free(BTDepth4);
    free(BTDepth3);
    free(BTDepth2);
    free(BTDepth1);

    free(BTV4);
    free(BTV3);
    free(BTV2);
    free(BTV1);

    free(Cor4);
    free(Cor3);
    free(Cor2);
    free(Cor1);

    free(E4);
    free(E3);
    free(E2);
    free(E1);

    free(V4);
    free(V3);
    free(V2);
    free(V1);

    free(RTC_sec);
    free(RTC_min);
    free(RTC_hour);
    free(RTC_day);
    free(RTC_month);
    free(RTC_year);
}

void FileData::exportTxt(const QString &FileName)
{
    if (fid == NULL)
        return;

    FILE *fp = fopen(FileName.toStdString().c_str(), "wt");

    fprintf(fp, "min_bin = %d\n", min_bin);
    fprintf(fp, "max_bin = %d\n", max_bin);
    fprintf(fp, "min_ensemble = %d\n", min_ensemble);
    fprintf(fp, "max_ensemble = %d\n", max_ensemble);
    fprintf(fp, "\n");

    fwriteDouble2(fp, "V1", V1, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteDouble2(fp, "V2", V2, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteDouble2(fp, "V3", V3, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteDouble2(fp, "V4", V4, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);

    fwriteDouble2(fp, "E1", E1, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteDouble2(fp, "E2", E2, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteDouble2(fp, "E3", E3, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteDouble2(fp, "E4", E4, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);

    fwriteDouble2(fp, "Cor1", Cor1, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteDouble2(fp, "Cor2", Cor2, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteDouble2(fp, "Cor3", Cor3, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteDouble2(fp, "Cor4", Cor4, cellNum,
                  min_bin, max_bin, min_ensemble, max_ensemble);

    fwriteDouble1(fp, "BTV1", BTV1, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTV2", BTV2, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTV3", BTV3, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTV4", BTV4, min_ensemble, max_ensemble);

    fwriteDouble1(fp, "BTDepth1", BTDepth1, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTDepth2", BTDepth2, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTDepth3", BTDepth3, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTDepth4", BTDepth4, min_ensemble, max_ensemble);

    fwriteDouble1(fp, "BTE1", BTE1, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTE2", BTE2, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTE3", BTE3, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTE4", BTE4, min_ensemble, max_ensemble);

    fwriteDouble1(fp, "BTCor1", BTCor1, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTCor2", BTCor2, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTCor3", BTCor3, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTCor4", BTCor4, min_ensemble, max_ensemble);

    fwriteDouble1(fp, "BTP1", BTP1, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTP2", BTP2, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTP3", BTP3, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "BTP4", BTP4, min_ensemble, max_ensemble);

    fwriteDouble1(fp, "Pressure", Pressure, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "Temp", Temp, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "Roll", Roll, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "Pitch", Pitch, min_ensemble, max_ensemble);
    fwriteDouble1(fp, "Heading", Heading, min_ensemble, max_ensemble);

    fclose(fp);
}

void FileData::exportMat(const QString &FileName)
{
    if (fid == NULL)
        return;

    MATFile *fp = matOpen(FileName.toStdString().c_str(), "w");

    fwriteMatDouble2(fp, "V1", V1, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteMatDouble2(fp, "V2", V2, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteMatDouble2(fp, "V3", V3, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteMatDouble2(fp, "V4", V4, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);

    fwriteMatDouble2(fp, "E1", E1, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);

    fwriteMatDouble2(fp, "E2", E2, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteMatDouble2(fp, "E3", E3, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteMatDouble2(fp, "E4", E4, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);

    fwriteMatDouble2(fp, "Cor1", Cor1, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteMatDouble2(fp, "Cor2", Cor2, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteMatDouble2(fp, "Cor3", Cor3, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);
    fwriteMatDouble2(fp, "Cor4", Cor4, cellNum,
                     min_bin, max_bin, min_ensemble, max_ensemble);

    fwriteMatDouble1(fp, "BTV1", BTV1, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTV2", BTV2, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTV3", BTV3, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTV4", BTV4, min_ensemble, max_ensemble);

    fwriteMatDouble1(fp, "BTDepth1", BTDepth1, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTDepth2", BTDepth2, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTDepth3", BTDepth3, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTDepth4", BTDepth4, min_ensemble, max_ensemble);

    fwriteMatDouble1(fp, "BTE1", BTE1, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTE2", BTE2, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTE3", BTE3, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTE4", BTE4, min_ensemble, max_ensemble);

    fwriteMatDouble1(fp, "BTCor1", BTCor1, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTCor2", BTCor2, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTCor3", BTCor3, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTCor4", BTCor4, min_ensemble, max_ensemble);

    fwriteMatDouble1(fp, "BTP1", BTP1, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTP2", BTP2, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTP3", BTP3, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "BTP4", BTP4, min_ensemble, max_ensemble);

    fwriteMatDouble1(fp, "Pressure", Pressure, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "Temp", Temp, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "Roll", Roll, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "Pitch", Pitch, min_ensemble, max_ensemble);
    fwriteMatDouble1(fp, "Heading", Heading, min_ensemble, max_ensemble);

    matClose(fp);
}

void FileData::exportTxt(const QString &FileName, const QSet<QString> &m_CheckSet)
{
    if (fid == NULL)
        return;

    FILE *fp = fopen(FileName.toStdString().c_str(), "wt");

    fprintf(fp, "min_bin = %d\n", min_bin);
    fprintf(fp, "max_bin = %d\n", max_bin);
    fprintf(fp, "min_ensemble = %d\n", min_ensemble);
    fprintf(fp, "max_ensemble = %d\n", max_ensemble);
    fprintf(fp, "\n");

    if (m_CheckSet.contains("V1"))
        fwriteDouble2(fp, "V1", V1, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("V2"))
        fwriteDouble2(fp, "V2", V2, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("V3"))
        fwriteDouble2(fp, "V3", V3, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("V4"))
        fwriteDouble2(fp, "V4", V4, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("E1"))
        fwriteDouble2(fp, "E1", E1, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("E2"))
        fwriteDouble2(fp, "E2", E2, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("E3"))
        fwriteDouble2(fp, "E3", E3, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("E4"))
        fwriteDouble2(fp, "E4", E4, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("Cor1"))
        fwriteDouble2(fp, "Cor1", Cor1, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Cor2"))
        fwriteDouble2(fp, "Cor2", Cor2, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Cor3"))
        fwriteDouble2(fp, "Cor3", Cor3, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Cor4"))
        fwriteDouble2(fp, "Cor4", Cor4, cellNum,
                      min_bin, max_bin, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTV1"))
        fwriteDouble1(fp, "BTV1", BTV1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTV2"))
        fwriteDouble1(fp, "BTV2", BTV2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTV3"))
        fwriteDouble1(fp, "BTV3", BTV3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTV4"))
        fwriteDouble1(fp, "BTV4", BTV4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTDepth1"))
        fwriteDouble1(fp, "BTDepth1", BTDepth1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTDepth2"))
        fwriteDouble1(fp, "BTDepth2", BTDepth2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTDepth3"))
        fwriteDouble1(fp, "BTDepth3", BTDepth3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTDepth4"))
        fwriteDouble1(fp, "BTDepth4", BTDepth4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTE1"))
        fwriteDouble1(fp, "BTE1", BTE1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTE2"))
        fwriteDouble1(fp, "BTE2", BTE2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTE3"))
        fwriteDouble1(fp, "BTE3", BTE3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTE4"))
        fwriteDouble1(fp, "BTE4", BTE4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTCor1"))
        fwriteDouble1(fp, "BTCor1", BTCor1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTCor2"))
        fwriteDouble1(fp, "BTCor2", BTCor2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTCor3"))
        fwriteDouble1(fp, "BTCor3", BTCor3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTCor4"))
        fwriteDouble1(fp, "BTCor4", BTCor4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTP1"))
        fwriteDouble1(fp, "BTP1", BTP1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTP2"))
        fwriteDouble1(fp, "BTP2", BTP2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTP3"))
        fwriteDouble1(fp, "BTP3", BTP3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTP4"))
        fwriteDouble1(fp, "BTP4", BTP4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("Pressure"))
        fwriteDouble1(fp, "Pressure", Pressure, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Temp"))
        fwriteDouble1(fp, "Temp", Temp, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Roll"))
        fwriteDouble1(fp, "Roll", Roll, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Pitch"))
        fwriteDouble1(fp, "Pitch", Pitch, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Heading"))
        fwriteDouble1(fp, "Heading", Heading, min_ensemble, max_ensemble);

    fclose(fp);
}

void FileData::exportMat(const QString &FileName, const QSet<QString> &m_CheckSet)
{
    if (fid == NULL)
        return;

    MATFile *fp = matOpen(FileName.toStdString().c_str(), "w");

    if (m_CheckSet.contains("V1"))
        fwriteMatDouble2(fp, "V1", V1, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("V2"))
        fwriteMatDouble2(fp, "V2", V2, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("V3"))
        fwriteMatDouble2(fp, "V3", V3, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("V4"))
        fwriteMatDouble2(fp, "V4", V4, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("E1"))
        fwriteMatDouble2(fp, "E1", E1, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("E2"))
        fwriteMatDouble2(fp, "E2", E2, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("E3"))
        fwriteMatDouble2(fp, "E3", E3, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("E4"))
        fwriteMatDouble2(fp, "E4", E4, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("Cor1"))
        fwriteMatDouble2(fp, "Cor1", Cor1, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Cor2"))
        fwriteMatDouble2(fp, "Cor2", Cor2, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Cor3"))
        fwriteMatDouble2(fp, "Cor3", Cor3, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Cor4"))
        fwriteMatDouble2(fp, "Cor4", Cor4, cellNum,
                         min_bin, max_bin, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTV1"))
        fwriteMatDouble1(fp, "BTV1", BTV1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTV2"))
        fwriteMatDouble1(fp, "BTV2", BTV2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTV3"))
        fwriteMatDouble1(fp, "BTV3", BTV3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTV4"))
        fwriteMatDouble1(fp, "BTV4", BTV4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTDepth1"))
        fwriteMatDouble1(fp, "BTDepth1", BTDepth1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTDepth2"))
        fwriteMatDouble1(fp, "BTDepth2", BTDepth2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTDepth3"))
        fwriteMatDouble1(fp, "BTDepth3", BTDepth3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTDepth4"))
        fwriteMatDouble1(fp, "BTDepth4", BTDepth4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTE1"))
        fwriteMatDouble1(fp, "BTE1", BTE1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTE2"))
        fwriteMatDouble1(fp, "BTE2", BTE2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTE3"))
        fwriteMatDouble1(fp, "BTE3", BTE3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTE4"))
        fwriteMatDouble1(fp, "BTE4", BTE4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTCor1"))
        fwriteMatDouble1(fp, "BTCor1", BTCor1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTCor2"))
        fwriteMatDouble1(fp, "BTCor2", BTCor2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTCor3"))
        fwriteMatDouble1(fp, "BTCor3", BTCor3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTCor4"))
        fwriteMatDouble1(fp, "BTCor4", BTCor4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("BTP1"))
        fwriteMatDouble1(fp, "BTP1", BTP1, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTP2"))
        fwriteMatDouble1(fp, "BTP2", BTP2, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTP3"))
        fwriteMatDouble1(fp, "BTP3", BTP3, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("BTP4"))
        fwriteMatDouble1(fp, "BTP4", BTP4, min_ensemble, max_ensemble);

    if (m_CheckSet.contains("Pressure"))
        fwriteMatDouble1(fp, "Pressure", Pressure, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Temp"))
        fwriteMatDouble1(fp, "Temp", Temp, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Roll"))
        fwriteMatDouble1(fp, "Roll", Roll, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Pitch"))
        fwriteMatDouble1(fp, "Pitch", Pitch, min_ensemble, max_ensemble);
    if (m_CheckSet.contains("Heading"))
        fwriteMatDouble1(fp, "Heading", Heading, min_ensemble, max_ensemble);

    matClose(fp);
}
