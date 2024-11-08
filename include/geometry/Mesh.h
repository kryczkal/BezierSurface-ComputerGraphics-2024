//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_MESH_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_MESH_H

#include "Triangle.h"
#include "graphics/QGraphicsEngineDrawable.h"
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QVector>

class Mesh : public QGraphicsEngineDrawable
{
    public:
    // Constructors
    Mesh() = default;
    [[maybe_unused]] explicit Mesh(const QVector<Triangle> &vertices);
    [[maybe_unused]] explicit Mesh(QVector<Triangle> &&vertices);

    // Getters
    [[maybe_unused]] [[nodiscard]] QVector<Triangle> getTriangles() const;

    // Tessellation
    [[maybe_unused]] static Mesh create2dTessellation(int tessellationLevel);
    static QVector<Triangle> create2dTessellationTriangles(int tessellationLevel);

    // Public Methods
    void draw(DrawData &drawData) override;
    void transform(QMatrix4x4 &matrix, bool absolute, bool preprocessMatrix) override;
    void transform(QMatrix4x4 &matrix, QVector3D center, bool absolute, bool preprocessMatrix) override;

    protected:
    QMatrix4x4 modelMatrix;
    QVector<Triangle> _triangles;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_MESH_H
