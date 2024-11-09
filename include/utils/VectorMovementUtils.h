//
// Created by wookie on 11/9/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_VECTORMOVEMENTUTILS_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_VECTORMOVEMENTUTILS_H

#include <QVector3D>

class VectorMovementUtils
{
    public:
    static void moveStep(QVector3D &position, QVector3D direction, float step);
    static void moveAcrossCircle(QVector3D &position, QVector3D center, float radius, float angle);
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_VECTORMOVEMENTUTILS_H
