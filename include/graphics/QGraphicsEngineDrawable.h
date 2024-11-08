//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINEDRAWABLE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINEDRAWABLE_H

#include <QImage>
class DrawData;

class QGraphicsEngineDrawable
{
    public:
    virtual void draw(DrawData &drawData)      = 0;
    virtual void transform(QMatrix4x4 &matrix) = 0;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINEDRAWABLE_H
