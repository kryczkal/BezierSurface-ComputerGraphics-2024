//
// Created by wookie on 11/7/24.
//

#include "geometry/Triangle.h"
#include "geometry/Mesh.h"

[[maybe_unused]] Mesh::Mesh(const QVector<Triangle> &triangles) {
    _triangles = triangles;
}

[[maybe_unused]] Mesh::Mesh(QVector<Triangle> &&triangles) {
    _triangles = qMove(triangles);
}

[[maybe_unused]] QVector<Triangle> Mesh::getTriangles() const {
    return _triangles;
}

[[maybe_unused]] Mesh Mesh::create2dTessellation(const int tessellationLevel) {
    return Mesh(create2dTessellationTriangles(tessellationLevel));
}

QVector<Triangle> Mesh::create2dTessellationTriangles(const int tessellationLevel) {
    QVector<Triangle> triangles;
    for (int i = 0; i < tessellationLevel; i++) {
        for (int j = 0; j < tessellationLevel; j++) {
            Vertex a(i / static_cast<float>(tessellationLevel), j / static_cast<float>(tessellationLevel), 0);
            Vertex b((i + 1) / static_cast<float>(tessellationLevel), j / static_cast<float>(tessellationLevel), 0);
            Vertex c(i / static_cast<float>(tessellationLevel), (j + 1) / static_cast<float>(tessellationLevel), 0);
            Vertex d((i + 1) / static_cast<float>(tessellationLevel), (j + 1) / static_cast<float>(tessellationLevel), 0);
            triangles.append(Triangle(a, b, c));
            triangles.append(Triangle(b, c, d));
        }
    }
    return triangles;
}

void Mesh::draw(const QImage &canvas, const QImage &texture) {
    for (const auto &triangle : _triangles) {
        triangle.draw(canvas, texture);
    }
}