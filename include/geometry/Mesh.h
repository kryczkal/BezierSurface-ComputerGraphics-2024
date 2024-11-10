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

    QSharedPointer<QImage> getTexture() const { return _texture; }
    void setTexture(QSharedPointer<QImage> texture) { _texture = texture; }

    QSharedPointer<QImage> getNormalMap() const { return _normalMap; }
    void setNormalMap(QSharedPointer<QImage> normalMap) { _normalMap = normalMap; }

    // Tessellation
    [[maybe_unused]] static Mesh create2dTessellation(int tessellationLevel);
    static QVector<Triangle> create2dTessellationTriangles(int tessellationLevel);

    // Public Methods
    void draw(DrawData &drawData) override;
    void transform(QMatrix4x4 &matrix) override;

    [[maybe_unused]] void loadTexture(const QString &path) { setTexture(QSharedPointer<QImage>::create(QImage(path))); }
    [[maybe_unused]] void loadNormalMap(const QString &path)
    {
        setNormalMap(QSharedPointer<QImage>::create(QImage(path)));
    }

    protected:
    QVector3D _position;
    QMatrix4x4 _modelMatrix;
    QVector<Triangle> _triangles;
    QSharedPointer<QImage> _texture;
    QSharedPointer<QImage> _normalMap;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_MESH_H
