#include <QtMath>

#include "data2image.h"

QImage getSeries1(double *data,
                  int start_col, int end_col,
                  double min_value, double max_value,
                  int image_width, int image_height,
                  QColor back_color, QColor point_color)
{
    QImage image(image_width, image_height, QImage::Format_RGBA8888);
    image.fill(back_color);

    if (min_value >= max_value)
        return image;

    for (int x = 0; x < image_width; x++)
    {
        int ind = qRound(x / (image_width - 1.0) * (end_col - start_col) + start_col);

        double data_value = data[ind];
        if (data_value < min_value)
            data_value = min_value;
        else if (data_value > max_value)
            data_value = max_value;

        int y = qRound((data_value - min_value) / (max_value - min_value) * (image_height - 1));

        image.setPixelColor(x, y, point_color);
    }

    return image;
}

QImage getSeries2(double *data, int data_width,
                  int pick_row,
                  int start_col, int end_col,
                  double min_value, double max_value,
                  int image_width, int image_height,
                  QColor back_color, QColor point_color)
{
    QImage image(image_width, image_height, QImage::Format_RGBA8888);
    image.fill(back_color);

    if (min_value >= max_value)
        return image;

    for (int x = 0; x < image_width; x++)
    {
        int col = qRound(x / (image_width - 1.0) * (end_col - start_col) + start_col);

        int ind = data_width * pick_row + col;

        double data_value = data[ind];
        if (data_value < min_value)
            data_value = min_value;
        else if (data_value > max_value)
            data_value = max_value;

        int y = qRound((data_value - min_value) / (max_value - min_value) * (image_height - 1));

        image.setPixelColor(x, y, point_color);
    }

    return image;
}

QImage getProfile2(double *data, int data_width,
                   int pick_col,
                   int start_row, int end_row,
                   double min_value, double max_value,
                   int image_width, int image_height,
                   QColor back_color, QColor point_color)
{
    QImage image(image_width, image_height, QImage::Format_RGBA8888);
    image.fill(back_color);

    if (min_value >= max_value)
        return image;

    for (int y = 0; y < image_height; y++)
    {
        int row = qRound(y / (image_height - 1.0) * (end_row - start_row) + start_row);

        int ind = data_width * row + pick_col;

        double data_value = data[ind];
        if (data_value < min_value)
            data_value = min_value;
        else if (data_value > max_value)
            data_value = max_value;

        int x = qRound((data_value - min_value) / (max_value - min_value) * (image_width - 1));

        image.setPixelColor(x, y, point_color);
    }

    return image;
}

QImage getContour(double *data, int data_width,
                  int start_row, int end_row,
                  int start_col, int end_col,
                  double min_value, double max_value,
                  int image_width, int image_height,
                  const QVector<QColor> &color_bar, QColor back_color)
{
    QImage image(image_width, image_height, QImage::Format_RGBA8888);
    image.fill(back_color);

    int color_number = color_bar.size();
    if (min_value >= max_value || color_number <= 0)
        return image;

    for (int y = 0; y < image_height; y++)
    {
        int row = qRound(y / (image_height - 1.0) * (end_row - start_row) + start_row);

        for (int x = 0; x < image_width; x++)
        {
            int col = qRound(x / (image_width - 1.0) * (end_col - start_col) + start_col);

            int ind = data_width * row + col;

            double value = data[ind];
            if (value < min_value)
                value = min_value;
            else if (value > max_value)
                value = max_value;

            int color_ind = qRound((value - min_value) / (max_value - min_value) * (color_number - 1));
            QColor color = color_bar.at(color_ind);
            image.setPixelColor(x, y, color);
        }
    }
    return image;
}
