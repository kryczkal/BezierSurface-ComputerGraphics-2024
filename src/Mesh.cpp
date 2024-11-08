//
// Created by wookie on 11/7/24.
//

#include "geometry/Mesh.h"
#include "geometry/Triangle.h"
#include "models/DrawData.h"
#include <cmath>

[[maybe_unused]] Mesh::Mesh(const QVector<Triangle> &triangles) { _triangles = triangles; }

[[maybe_unused]] Mesh::Mesh(QVector<Triangle> &&triangles) { _triangles = qMove(triangles); }

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
    for (auto &triangle : _triangles)
    {
        triangle.draw(drawData);
    }
}