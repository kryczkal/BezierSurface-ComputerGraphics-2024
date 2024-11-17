//
// Created by wookie on 11/8/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWDATA_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWDATA_H

#include "graphics/LightSource.h"
#include <QImage>
#include <QSharedPointer>
#include <QVariant>
#include <qmutex.h>

class DrawData
{
    public:
    explicit DrawData(QImage &canvas);
    DrawData(QImage &canvas, const QColor &brushColor);
    DrawData(QImage &canvas, const QImage &texture);

    QImage &canvas;

    QColor brushColor;
    QSharedPointer<QImage> texture;
    QSharedPointer<QImage> normalMap;

    QMutex zBufferMutex;
    QScopedPointer<float, QScopedPointerArrayDeleter<float>> zBuffer;
    LightSource *lightSource = nullptr;

    void initZBuffer();
    void clearZBuffer() const;

    void setTexture(const QImage &texture);

    void setBrushColor(const QColor &color);

    void setLightSource(LightSource *lightSource) { this->lightSource = lightSource; }

    private:
    int X, Y;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWDATA_H
