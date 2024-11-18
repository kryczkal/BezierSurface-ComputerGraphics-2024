//
// Created by wookie on 11/7/24.
//

#include "geometry/Mesh.h"
#include "geometry/Triangle.h"
#include "models/DrawData.h"
#include <QFile>
#include <QImageReader>
#include <QMatrix4x4>
#include <QRegularExpression>
#include <QTextStream>
#include <QVector2D>
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

void Mesh::readFromFile(const QString &path)
{
    QMutexLocker locker(&_mutex);

    // Clear existing data
    _triangles.clear();

    // Containers for .obj data
    QVector<QVector3D> positions;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;

    // Open the .obj file
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Cannot open file:" << path;
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.startsWith("#") || line.isEmpty())
            continue;

        QStringList tokens = line.split(QRegularExpression("\\s+"));
        QString prefix     = tokens[0];

        if (prefix == "v")
        {
            // Vertex position
            positions.append(QVector3D(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat()));
        }
        else if (prefix == "vt")
        {
            // Texture coordinate
            texCoords.append(QVector2D(tokens[1].toFloat(), tokens[2].toFloat()));
        }
        else if (prefix == "vn")
        {
            // Vertex normal
            normals.append(QVector3D(tokens[1].toFloat(), tokens[2].toFloat(), -tokens[3].toFloat()));
        }
        else if (prefix == "f")
        {
            // Face definition
            if (tokens.size() < 4)
                continue; // Not enough vertices for a triangle

            QVector<Vertex> faceVertices;
            for (int i = 1; i <= 3; ++i)
            {
                QStringList indices = tokens[i].split('/');

                int posIndex  = indices[0].toInt() - 1;
                int texIndex  = indices.size() > 1 && !indices[1].isEmpty() ? indices[1].toInt() - 1 : -1;
                int normIndex = indices.size() > 2 && !indices[2].isEmpty() ? indices[2].toInt() - 1 : -1;

                QVector3D position = positions[posIndex];
                QVector3D normal   = normIndex >= 0 ? normals[normIndex] : QVector3D(0, 0, 0);
                float u            = texIndex >= 0 ? texCoords[texIndex].x() : 0.0f;
                float v            = texIndex >= 0 ? texCoords[texIndex].y() : 0.0f;

                Vertex vertex(position, normal);
                vertex.setU(u);
                vertex.setV(v);

                faceVertices.append(vertex);
            }

            // Create a triangle and add it to the mesh
            Triangle triangle(faceVertices[0], faceVertices[1], faceVertices[2]);
            _triangles.append(triangle);
        }
    }

    file.close();

    locker.unlock();
    normalize();
    // Calculate the center of the mesh
    QVector3D center(0.0f, 0.0f, 0.0f);
    int vertexCount = 0;

    for (const Triangle &triangle : _triangles)
    {
        for (const Vertex &vertex : {triangle.getA(), triangle.getB(), triangle.getC()})
        {
            center += vertex.getPositionOriginal();
            ++vertexCount;
        }
    }

    if (vertexCount > 0)
    {
        center /= static_cast<float>(vertexCount);
    }

    // Set the mesh's position to its center
    _position = center;
    calculateTangents();
}
void Mesh::calculateTangents()
{
    for (Triangle &triangle : _triangles)
    {
        Vertex &v0 = triangle.getA();
        Vertex &v1 = triangle.getB();
        Vertex &v2 = triangle.getC();

        QVector3D deltaPos1 = v1.getPositionOriginal() - v0.getPositionOriginal();
        QVector3D deltaPos2 = v2.getPositionOriginal() - v0.getPositionOriginal();

        QVector2D deltaUV1 = QVector2D(v1.getU(), v1.getV()) - QVector2D(v0.getU(), v0.getV());
        QVector2D deltaUV2 = QVector2D(v2.getU(), v2.getV()) - QVector2D(v0.getU(), v0.getV());

        float r             = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
        QVector3D tangent   = (deltaPos1 * deltaUV2.y() - deltaPos2 * deltaUV1.y()) * r;
        QVector3D bitangent = (deltaPos2 * deltaUV1.x() - deltaPos1 * deltaUV2.x()) * r;

        // Set the tangents in the vertices
        v0.setUTangentOriginal(tangent);
        v1.setUTangentOriginal(tangent);
        v2.setUTangentOriginal(tangent);

        v0.setVTangentOriginal(bitangent);
        v1.setVTangentOriginal(bitangent);
        v2.setVTangentOriginal(bitangent);
    }
}
void Mesh::normalize()
{
    QMutexLocker locker(&_mutex);

    if (_triangles.isEmpty())
        return;

    // Initialize min and max values
    QVector3D minCoords(
        std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()
    );
    QVector3D maxCoords(
        std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()
    );

    // Find min and max coordinates
    for (const Triangle &triangle : _triangles)
    {
        for (const Vertex &vertex : {triangle.getA(), triangle.getB(), triangle.getC()})
        {
            const QVector3D &pos = vertex.getPositionOriginal();
            minCoords.setX(std::min(minCoords.x(), pos.x()));
            minCoords.setY(std::min(minCoords.y(), pos.y()));
            minCoords.setZ(std::min(minCoords.z(), pos.z()));

            maxCoords.setX(std::max(maxCoords.x(), pos.x()));
            maxCoords.setY(std::max(maxCoords.y(), pos.y()));
            maxCoords.setZ(std::max(maxCoords.z(), pos.z()));
        }
    }

    // Compute the size of the bounding box
    QVector3D size = maxCoords - minCoords;

    // Find the maximum dimension size
    float maxSize = std::max(size.x(), std::max(size.y(), size.z()));

    if (maxSize == 0.0f)
        return; // Avoid division by zero

    // Compute scaling factor
    float scale = 1.0f / maxSize;

    // Normalize vertices
    for (Triangle &triangle : _triangles)
    {
        for (Vertex *vertex : {&triangle.getA(), &triangle.getB(), &triangle.getC()})
        {
            // Translate to origin
            QVector3D pos = vertex->getPositionOriginal() - minCoords;

            // Scale to [0, 1]
            pos *= scale;

            // Update the vertex positions
            vertex->setPositionOriginal(pos);
            vertex->setPositionTransformed(pos);
        }
    }
}
