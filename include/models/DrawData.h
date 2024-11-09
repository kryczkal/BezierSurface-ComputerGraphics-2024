//
// Created by wookie on 11/8/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWDATA_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWDATA_H

#include "graphics/LightSource.h"
#include <QImage>
#include <QVariant>

class DrawData
{
    public:
    DrawData(QImage &canvas, const QColor &brushColor);

    QImage &canvas;
    QVariant textureOrBrushColor;
    QVector<QVector<float>> zBuffer;
    LightSource *lightSource = nullptr;

    void clearZBuffer();

    void clear();

    void clear(const QColor &color);

    void setTexture(const QImage &texture);

    void setBrushColor(const QColor &color);

    void setLightSource(LightSource *lightSource) { this->lightSource = lightSource; }

    private:
    int X, Y;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWDATA_H
