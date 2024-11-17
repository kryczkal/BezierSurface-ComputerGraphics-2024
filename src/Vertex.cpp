//
// Created by wookie on 11/7/24.
//

#include "geometry/Vertex.h"
#include "models/DrawData.h"
#include "settings/Settings.h"
#include "utils/DrawUtils.h"
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
    QVector3D position   = _positionTransformed;

    const int radiusX = std::max(1.0f, settings.graphicsEngineSettings.sizeX * settings.vertexSettings.radiusCoef);
    const int radiusY = std::max(1.0f, settings.graphicsEngineSettings.sizeY * settings.vertexSettings.radiusCoef);

    DrawUtils::drawPoint(drawData, _positionTransformed, settings.vertexSettings.vertexColor, radiusX, radiusY);

    if (settings.vertexSettings.drawNormals)
    {
        QVector3D normal = _positionTransformed + -_normalTransformed * settings.vertexSettings.normalLength;
        DrawUtils::drawLine(drawData, _positionTransformed, normal, settings.vertexSettings.normalColor);
    }

    if (settings.vertexSettings.drawTangents)
    {
        QVector3D uTangent = _positionTransformed + _uTangentTransformed * settings.vertexSettings.tangentLength;
        DrawUtils::drawLine(drawData, _positionTransformed, uTangent, settings.vertexSettings.tangentColor);

        QVector3D vTangent = _positionTransformed + _vTangentTransformed * settings.vertexSettings.tangentLength;
        DrawUtils::drawLine(drawData, _positionTransformed, vTangent, settings.vertexSettings.tangentColor);
    }
}

void Vertex::transform(QMatrix4x4 &matrix)
{
    _positionTransformed = matrix * _positionOriginal;

    _normalTransformed   = matrix.mapVector(_normalOriginal);
    _uTangentTransformed = matrix.mapVector(_uTangentOriginal);
    _vTangentTransformed = matrix.mapVector(_vTangentOriginal);
}
