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
    // Constructors
    explicit BezierSurface(const QString &filename, int tessellationLevel = -1);

    // Getters
    [[maybe_unused]] [[nodiscard]] QVector<QVector3D> getControlPoints() const;

    // Public Methods
    void draw(DrawData &drawData) override;
    void transform(QMatrix4x4 &matrix) override;

    void setTessellationLevel(int tessellationLevel);

    private:
    QVector<QVector3D> _controlPointsNormal;
    QVector<QVector3D> _controlPointsTransformed;

    void readControlPoints(const QString &filename);
    void map3dBezierFrom2dMesh();

    void evaluateBezierSurface(Vertex &vertex) const;
    void drawControlPointsAndGrid(DrawData &drawData);
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_BEZIERSURFACE_H
