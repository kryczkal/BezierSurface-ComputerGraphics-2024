//
// Created by wookie on 11/7/24.
//

#include "geometry/BezierSurface.h"
#include <QDebug>
#include <QFile>

BezierSurface::BezierSurface(const QString &filename, const int tessellationLevel)
        : Mesh() {
    // Each line of the file contains 3 floats separated by a space (x y z)
    // Each line represents a control point

    readControlPoints(filename);
    _triangles = create2dTessellationTriangles(tessellationLevel);
    map3dBezierFrom2dMesh();
}


[[maybe_unused]] QVector<QVector3D> BezierSurface::getControlPoints() const {
    return _controlPoints;
}

void BezierSurface::readControlPoints(const QString &filename) {
    qDebug() << "Reading control points from file:" << filename;

    // Open the file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file" << filename;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList coords = line.split(" ");

        if (coords.size() != 3) {
            qDebug() << "Invalid line:" << line;
            return;
        }

        bool okX, okY, okZ;
        float x = coords[0].toFloat(&okX);
        float y = coords[1].toFloat(&okY);
        float z = coords[2].toFloat(&okZ);

        if (!okX || !okY || !okZ) {
            qDebug() << "Failed to convert line to floats:" << line;
            return;
        }

        _controlPoints.push_back(QVector3D(x, y, z));

        qDebug() << "Read control points:"
                 << QString("%1 %2 %3")
                         .arg(x, 0, 'f', 2)
                         .arg(y, 0, 'f', 2)
                         .arg(z, 0, 'f', 2);
    }
    file.close();

    qDebug() << "Read" << _controlPoints.size() << "control points";
}

void BezierSurface::map3dBezierFrom2dMesh() {
    QVector3D center(0, 0, 0);

    qDebug() << "Transforming 2D mesh to 3D";
    for(auto &triangle : _triangles) {
        for (int i = 0; i < 3; i++) {
            Vertex vertex = triangle[i];
            QVector3D position = evaluateBezierSurface(vertex.getPosition().x(), vertex.getPosition().y());

            vertex.setPosition(position);
            center += position;
        }
    }

    center /= static_cast<float>(_triangles.size() * 3);
    qDebug() << "Center:" << center;

    qDebug() << "Centering mesh";
    for(auto &triangle : _triangles) {
        for (int i = 0; i < 3; i++) {
            Vertex vertex = triangle[i];
            vertex.setPosition(vertex.getPosition() - center);
        }
    }
    qDebug() << "Centered mesh";
}

QVector3D BezierSurface::evaluateBezierSurface(float u, float v) const {
    int n = 4;
    QVector<QVector3D> tempPoints = _controlPoints;

    for (int k = 1; k < n; ++k) {
        for (int i = 0; i < n - k; ++i) {
            for (int j = 0; j < n - k; ++j) {
                tempPoints[i * n + j] = (1 - u) * tempPoints[i * n + j] + u * tempPoints[(i + 1) * n + j];
                tempPoints[i * n + j] = (1 - v) * tempPoints[i * n + j] + v * tempPoints[i * n + j + 1];
            }
        }
    }
    return tempPoints[0];
}
