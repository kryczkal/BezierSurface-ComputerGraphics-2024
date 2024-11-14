//
// Created by wookie on 11/7/24.
//

#include "geometry/Mesh.h"
#include "geometry/Triangle.h"
#include "models/DrawData.h"
#include <QMatrix4x4>
#include <QtConcurrent>
#include <algorithm>
#include <cmath>
#include <qtconcurrentmap.h>

[[maybe_unused]] Mesh::Mesh(const QVector<Triangle> &triangles)
{
    _triangles = triangles;
    sortTrianglesByDepth();
}

[[maybe_unused]] Mesh::Mesh(QVector<Triangle> &&triangles)
{
    _triangles = qMove(triangles);
    sortTrianglesByDepth();
}

[[maybe_unused]] QVector<Triangle> Mesh::getTriangles() const { return _triangles; }

[[maybe_unused]] Mesh Mesh::create2dTessellation(const int tessellationLevel)
{
    return Mesh(create2dTessellationTriangles(tessellationLevel));
}

QVector<Triangle> Mesh::create2dTessellationTriangles(const int tessellationLevel)
{
    QVector<Triangle> triangles;
    int tessellationLevelX = std::ceil(std::sqrt(tessellationLevel));
    int tessellationLevelY = std::ceil(std::sqrt(tessellationLevel));

    for (int x = 0; x < tessellationLevelX; x++)
    {
        for (int y = 0; y < tessellationLevelY; y++)
        {
            Vertex a(QVector3D(x / (float)tessellationLevelX, y / (float)tessellationLevelY, 0));
            Vertex b(QVector3D((x + 1) / (float)tessellationLevelX, y / (float)tessellationLevelY, 0));
            Vertex c(QVector3D(x / (float)tessellationLevelX, (y + 1) / (float)tessellationLevelY, 0));
            Vertex d(QVector3D((x + 1) / (float)tessellationLevelX, (y + 1) / (float)tessellationLevelY, 0));

            triangles.push_back(Triangle(a, b, c));
            triangles.push_back(Triangle(b, c, d));
        }
    }

    return triangles;
}

void Mesh::draw(DrawData &drawData)
{
    QMutexLocker locker(&_mutex);
    drawData.texture   = _texture;
    drawData.normalMap = _normalMap;

    QtConcurrent::blockingMap(
        _triangles,
        [&drawData](Triangle &triangle)
        {
            triangle.draw(drawData);
        }
    );
}

void Mesh::transform(QMatrix4x4 &matrix)
{
    QMutexLocker locker(&_mutex);
    QMatrix4x4 translateToOrigin;
    translateToOrigin.translate(-_position);

    QMatrix4x4 translateBack;
    translateBack.translate(_position);

    _modelMatrix = translateBack * matrix * translateToOrigin;

    QtConcurrent::blockingMap(
        _triangles,
        [&](Triangle &triangle)
        {
            triangle.transform(_modelMatrix);
        }
    );

    sortTrianglesByDepth();
}

void Mesh::sortTrianglesByDepth()
{
    std::sort(
        _triangles.begin(), _triangles.end(),
        [](const Triangle &a, const Triangle &b)
        {
            float aDepth = std::max(
                {a.getA().getPositionTransformed().z(), a.getB().getPositionTransformed().z(),
                 a.getC().getPositionTransformed().z()}
            );
            float bDepth = std::max(
                {b.getA().getPositionTransformed().z(), b.getB().getPositionTransformed().z(),
                 b.getC().getPositionTransformed().z()}
            );
            return aDepth > bDepth;
        }
    );
}
