//
// Created by wookie on 11/8/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWDATA_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWDATA_H

#include <QImage>
#include <QVariant>

class DrawData
{
    public:
    DrawData(QImage &canvas, const QColor &color);

    QImage &canvas;
    QVariant textureOrColor;
    QVector<QVector<float>> zBuffer;

    void clearZBuffer();

    void clear();

    void clear(const QColor &color);

    void setTexture(const QImage &texture);

    void setColor(const QColor &color);

    private:
    int X = 800;
    int Y = 600;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWDATA_H
