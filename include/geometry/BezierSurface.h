//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_BEZIERSURFACE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_BEZIERSURFACE_H

#include "Mesh.h"
#include <QVector3D>
#include <QVector>

class BezierSurface : public Mesh
{
    public:
    explicit BezierSurface(const QString &filename, int tessellationLevel = -1);

    [[maybe_unused]] [[nodiscard]] QVector<QVector3D> getControlPoints() const;

    private:
    QVector<QVector3D> _controlPoints;

    void readControlPoints(const QString &filename);
    void map3dBezierFrom2dMesh();

    QVector3D evaluateBezierSurface(float u, float v) const;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_BEZIERSURFACE_H
