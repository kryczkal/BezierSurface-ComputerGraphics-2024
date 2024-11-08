//
// Created by wookie on 11/8/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWHELPER_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWHELPER_H

#include "models/DrawData.h"

class DrawHelper
{
    public:
    static void drawLine(
        DrawData &drawData, const QVector3D &start, const QVector3D &end, const QColor &color = Qt::black,
        float width = 1.0f
    );
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_DRAWHELPER_H
