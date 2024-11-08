//
// Created by wookie on 11/7/24.
//

#include "geometry/Triangle.h"
#include "geometry/Vertex.h"
#include "models/DrawData.h"
#include "utils/Settings.h"
#include <QDebug>
#include <QMatrix4x4>
#include <cmath>

[[maybe_unused]] Triangle::Triangle(Vertex a, Vertex b, Vertex c) : _a(a), _b(b), _c(c) {}

[[maybe_unused]] Vertex Triangle::getA() const { return _a; }

[[maybe_unused]] Vertex Triangle::getB() const { return _b; }

[[maybe_unused]] Vertex Triangle::getC() const { return _c; }

Vertex &Triangle::operator[](int i)
{
    switch (i)
    {
    case 0:
        return _a;
    case 1:
        return _b;
    case 2:
        return _c;
    default:
    {
        qDebug() << "Triangle::operator[]: index out of bounds";
        return _a;
    }
    }
}

void Triangle::draw(DrawData &drawData)
{
    Settings &settings = Settings::getInstance();

    QVector3D posA = _a.getPositionTransformed();
    QVector3D posB = _b.getPositionTransformed();
    QVector3D posC = _c.getPositionTransformed();

    int width  = drawData.canvas.width();
    int height = drawData.canvas.height();

    // Map 3D positions to 2D by ignoring the z-coordinate
    qreal x0 = posA.x() * width;
    qreal y0 = posA.y() * height;
    qreal z0 = posA.z();

    qreal x1 = posB.x() * width;
    qreal y1 = posB.y() * height;
    qreal z1 = posB.z();

    qreal x2 = posC.x() * width;
    qreal y2 = posC.y() * height;
    qreal z2 = posC.z();

    // Compute the bounding box of the triangle
    int minX = std::floor(std::min({x0, x1, x2}));
    int maxX = std::ceil(std::max({x0, x1, x2}));
    int minY = std::floor(std::min({y0, y1, y2}));
    int maxY = std::ceil(std::max({y0, y1, y2}));

    // Clamp the bounding box to the canvas size
    minX = std::max(0, minX);
    maxX = std::min(width - 1, maxX);
    minY = std::max(0, minY);
    maxY = std::min(height - 1, maxY);

    qreal denom = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
    if (denom == 0)
    {
        // The triangle is degenerate; no need to draw
        return;
    }

    if (settings.triangleSettings.debugDraw && drawData.textureOrBrushColor.canConvert<QColor>())
    {
        _a.draw(drawData);
        _b.draw(drawData);
        _c.draw(drawData);
    }

    static constexpr auto drawPixel = [](DrawData &drawData, int y, int x)
    {
        if (drawData.textureOrBrushColor.canConvert<QColor>())
        {
            QColor color = drawData.textureOrBrushColor.value<QColor>();
            drawData.canvas.setPixelColor(x, y, color);
        }
        else
        {
            // TODO: Implement texture drawing
            QColor color = Qt::magenta;
            drawData.canvas.setPixelColor(x, y, color);
        }
    };

    qreal A0 = y1 - y0, B0 = x0 - x1, C0 = x1 * y0 - x0 * y1;
    qreal A1 = y2 - y1, B1 = x1 - x2, C1 = x2 * y1 - x1 * y2;
    qreal A2 = y0 - y2, B2 = x2 - x0, C2 = x0 * y2 - x2 * y0;

    auto drawPixelDebug = [&](DrawData &drawData, const Settings &settings, int y, int x, qreal w0, qreal w1, qreal w2)
    {
        qreal dist0 = std::abs(A0 * x + B0 * y + C0) / std::sqrt(A0 * A0 + B0 * B0);
        qreal dist1 = std::abs(A1 * x + B1 * y + C1) / std::sqrt(A1 * A1 + B1 * B1);
        qreal dist2 = std::abs(A2 * x + B2 * y + C2) / std::sqrt(A2 * A2 + B2 * B2);

        qreal minDist = std::min({dist0, dist1, dist2});

        if (minDist <= 0.5)
            drawData.canvas.setPixelColor(x, y, settings.triangleSettings.triangleEdgeColor);
        else
            drawData.canvas.setPixelColor(x, y, settings.triangleSettings.triangleFillColor);
    };

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            qreal w0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / denom;
            qreal w1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / denom;
            qreal w2 = 1 - w0 - w1;

            // Check if the pixel is inside the triangle
            if (!(w0 >= 0 && w1 >= 0 && w2 >= 0))
                continue;

            // z Test
            qreal z = w0 * z0 + w1 * z1 + w2 * z2;
            if (z < drawData.zBuffer[x][y])
                continue;

            drawData.zBuffer[x][y] = z;
            if (settings.triangleSettings.debugDraw)
            {
                drawPixelDebug(drawData, settings, y, x, w0, w1, w2);
                continue;
            }

            drawPixel(drawData, y, x);
        }
    }
}

void Triangle::transform(QMatrix4x4 &matrix, bool absolute, bool preprocessMatrix)
{
    matrix           = preprocessMatrix ? matrix.inverted().transposed() : matrix;
    preprocessMatrix = false;
    _a.transform(matrix, absolute, preprocessMatrix);
    _b.transform(matrix, absolute, preprocessMatrix);
    _c.transform(matrix, absolute, preprocessMatrix);
}

void Triangle::transform(QMatrix4x4 &matrix, QVector3D center, bool absolute, bool preprocessMatrix)
{
    matrix           = preprocessMatrix ? matrix.inverted().transposed() : matrix;
    preprocessMatrix = false;
    _a.transform(matrix, center, absolute, preprocessMatrix);
    _b.transform(matrix, center, absolute, preprocessMatrix);
    _c.transform(matrix, center, absolute, preprocessMatrix);
}
