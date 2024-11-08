//
// Created by wookie on 11/7/24.
//

#include "geometry/Vertex.h"
#include "models/DrawData.h"
#include "utils/DrawHelper.h"
#include "utils/Settings.h"
#include <QColor>
#include <QMatrix4x4>
#include <QVector3D>
#include <limits>

Vertex::Vertex(QVector3D position, QVector3D normal, QVector3D uTangent, QVector3D vTangent)
{
    _positionOriginal = position;
    _normalOriginal   = normal;
    _uTangentOriginal = uTangent;
    _vTangentOriginal = vTangent;

    _positionTransformed = position;
    _normalTransformed   = normal;
    _uTangentTransformed = uTangent;
    _vTangentTransformed = vTangent;
}

void Vertex::draw(DrawData &drawData)
{
    const auto &settings = Settings::getInstance();
    int width            = drawData.canvas.width();
    int height           = drawData.canvas.height();
    QVector3D position   = _positionTransformed;
    int x                = position.x() * width;
    int y                = position.y() * height;

    int radiusX = std::max(1.0f, settings.graphicsEngineSettings.sizeX * settings.vertexSettings.radiusCoef);
    int radiusY = std::max(1.0f, settings.graphicsEngineSettings.sizeY * settings.vertexSettings.radiusCoef);

    for (int xm = x - radiusX; xm <= x + radiusX; ++xm)
    {
        for (int ym = y - radiusY; ym <= y + radiusY; ++ym)
        {
            if (xm >= 0 && xm < width && ym >= 0 && ym < height)
            {
                if (position.z() < drawData.zBuffer[xm][ym])
                    continue;
                drawData.zBuffer[xm][ym] = position.z();
                drawData.canvas.setPixelColor(xm, ym, settings.triangleSettings.triangleVertexColor);
            }
        }
    }

    if (settings.vertexSettings.drawNormals)
    {
        QVector3D normal = _positionTransformed + -_normalTransformed * settings.vertexSettings.normalLength;
        DrawHelper::drawLine(drawData, _positionTransformed, normal, settings.vertexSettings.normalColor);
    }

    if (settings.vertexSettings.drawTangents)
    {
        QVector3D uTangent = _positionTransformed + _uTangentTransformed * settings.vertexSettings.tangentLength;
        DrawHelper::drawLine(drawData, _positionTransformed, uTangent, settings.vertexSettings.tangentColor);

        QVector3D vTangent = _positionTransformed + _vTangentTransformed * settings.vertexSettings.tangentLength;
        DrawHelper::drawLine(drawData, _positionTransformed, vTangent, settings.vertexSettings.tangentColor);
    }
}

void Vertex::transform(QMatrix4x4 &matrix, bool absolute, bool preprocessMatrix)
{
    _positionTransformed = absolute ? (matrix * _positionOriginal) : (matrix * _positionTransformed);
    matrix               = preprocessMatrix ? matrix : matrix.inverted().transposed();

    _normalTransformed   = absolute ? (matrix * _normalOriginal) : (matrix * _normalTransformed);
    _uTangentTransformed = absolute ? (matrix * _uTangentOriginal) : (matrix * _uTangentTransformed);
    _vTangentTransformed = absolute ? (matrix * _vTangentOriginal) : (matrix * _vTangentTransformed);
}

void Vertex::transform(QMatrix4x4 &matrix, QVector3D center, bool absolute, bool preprocessMatrix)
{
    QVector3D position = absolute ? _positionOriginal : _positionTransformed;
    position -= center;
    position = matrix * position;
    position += center;

    _positionTransformed = position;

    QMatrix4x4 normalMatrix = preprocessMatrix ? matrix.inverted().transposed() : matrix;
    normalMatrix.setColumn(3, QVector4D(0, 0, 0, 1));

    _normalTransformed   = absolute ? (normalMatrix * _normalOriginal) : (normalMatrix * _normalTransformed);
    _uTangentTransformed = absolute ? (normalMatrix * _uTangentOriginal) : (normalMatrix * _uTangentTransformed);
    _vTangentTransformed = absolute ? (normalMatrix * _vTangentOriginal) : (normalMatrix * _vTangentTransformed);
}
