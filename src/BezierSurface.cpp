//
// Created by wookie on 11/7/24.
//

#include "geometry/BezierSurface.h"
#include <QDebug>
#include <QFile>

BezierSurface::BezierSurface(const QString &filename, int tessellationLevel) : Mesh()
{
    if (tessellationLevel == -1)
    {
        tessellationLevel = Settings::getInstance().meshSettings.tessellationLevel;
    }
    Q_ASSERT(tessellationLevel >= 0);

    readControlPoints(filename);
    _triangles = create2dTessellationTriangles(tessellationLevel);
    map3dBezierFrom2dMesh();
}

[[maybe_unused]] QVector<QVector3D> BezierSurface::getControlPoints() const { return _controlPoints; }

void BezierSurface::readControlPoints(const QString &filename)
{
    // Each line of the file contains 3 floats separated by a space (x y z)
    // Each line represents a control point
    qDebug() << "Reading control points from file:" << filename;

    // Open the file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open file" << filename;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line       = in.readLine();
        QStringList coords = line.split(" ");

        if (coords.size() != 3)
        {
            qDebug() << "Invalid line:" << line;
            return;
        }

        bool okX, okY, okZ;
        float x = coords[0].toFloat(&okX);
        float y = coords[1].toFloat(&okY);
        float z = coords[2].toFloat(&okZ);

        if (!okX || !okY || !okZ)
        {
            qDebug() << "Failed to convert line to floats:" << line;
            return;
        }

        _controlPoints.push_back(QVector3D(x, y, z));

        qDebug() << "Read control points:" << QString("%1 %2 %3").arg(x, 0, 'f', 2).arg(y, 0, 'f', 2).arg(z, 0, 'f', 2);
    }
    file.close();

    qDebug() << "Read" << _controlPoints.size() << "control points";
}

void BezierSurface::map3dBezierFrom2dMesh()
{
    QVector3D center(0, 0, 0);

    qDebug() << "Transforming 2D mesh to 3D";

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    for (auto &triangle : _triangles)
    {
        for (int i = 0; i < 3; i++)
        {
            Vertex &vertex = triangle[i];
            evaluateBezierSurface(vertex);

            minX = std::min(minX, vertex.getPositionOriginal().x());
            minY = std::min(minY, vertex.getPositionOriginal().y());
            maxX = std::max(maxX, vertex.getPositionOriginal().x());
            maxY = std::max(maxY, vertex.getPositionOriginal().y());
        }
    }
    qDebug() << "Bounding box:" << minX << minY << maxX << maxY;

    static constexpr float lowerBound = 0.20;
    static constexpr float upperBound = 0.80;
    static constexpr float scale      = upperBound - lowerBound;
    static constexpr float offset     = (1 - scale) / 2;

    float scaleX = scale / (maxX - minX);
    float scaleY = scale / (maxY - minY);

    qDebug() << "Centering and scaling mesh to:" << lowerBound << "-" << upperBound;

    for (auto &triangle : _triangles)
    {
        for (int i = 0; i < 3; i++)
        {
            Vertex &vertex = triangle[i];

            QVector3D originalPosition = vertex.getPositionOriginal();
            QVector3D scaledPosition   = (originalPosition - QVector3D(minX, minY, 0)) * QVector3D(scaleX, scaleY, 1) +
                                       QVector3D(offset, offset, 0);
            vertex.setPositionOriginal(scaledPosition);

            QVector3D uTangent = vertex.getUTangentOriginal() * QVector3D(scaleX, scaleX, scaleX).normalized();
            QVector3D vTangent = vertex.getVTangentOriginal() * QVector3D(scaleY, scaleY, scaleY).normalized();

            QVector3D normal = QVector3D::crossProduct(uTangent, vTangent).normalized();

            vertex.setUTangentOriginal(uTangent);
            vertex.setVTangentOriginal(vTangent);
            vertex.setNormalOriginal(normal);
        }
    }
}

void BezierSurface::evaluateBezierSurface(Vertex &vertex) const
{
    float u = vertex.getPositionOriginal().x();
    float v = vertex.getPositionOriginal().y();
    vertex.setU(u);
    vertex.setV(v);

    QVector3D position(0, 0, 0);
    QVector3D uTangent(0, 0, 0);
    QVector3D vTangent(0, 0, 0);

    // Coefficients for Bernstein polynomials of degree 3
    float Bu[4] = {(1 - u) * (1 - u) * (1 - u), 3 * u * (1 - u) * (1 - u), 3 * u * u * (1 - u), u * u * u};
    float Bv[4] = {(1 - v) * (1 - v) * (1 - v), 3 * v * (1 - v) * (1 - v), 3 * v * v * (1 - v), v * v * v};

    // Derivatives of Bernstein polynomials with respect to u and v
    float dBu[4] = {-3 * (1 - u) * (1 - u), 3 * (1 - u) * (1 - 3 * u), 3 * u * (2 - 3 * u), 3 * u * u};
    float dBv[4] = {-3 * (1 - v) * (1 - v), 3 * (1 - v) * (1 - 3 * v), 3 * v * (2 - 3 * v), 3 * v * v};

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            QVector3D controlPoint = _controlPoints[i * 4 + j];
            position += Bu[i] * Bv[j] * controlPoint;
            uTangent += dBu[i] * Bv[j] * controlPoint;
            vTangent += Bu[i] * dBv[j] * controlPoint;
        }
    }

    QVector3D normal = QVector3D::crossProduct(uTangent, vTangent).normalized();

    vertex.setPositionOriginal(position);
    vertex.setUTangentOriginal(uTangent);
    vertex.setVTangentOriginal(vTangent);
    vertex.setNormalOriginal(normal);
}
