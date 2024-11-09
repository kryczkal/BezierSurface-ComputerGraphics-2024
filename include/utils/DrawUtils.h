//
// Created by wookie on 11/8/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWUTILS_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWUTILS_H

#include "models/DrawData.h"

class DrawUtils
{
    public:
    static void drawLine(
        DrawData &drawData, const QVector3D &start, const QVector3D &end, const QColor &color = Qt::black,
        float width = 1.0f
    );
    static void drawPoint(
        DrawData &drawData, const QVector3D &point, const QColor &color = Qt::black, int radiusX = 1.0f,
        int radiusY = 1.0f
    );
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWUTILS_H
