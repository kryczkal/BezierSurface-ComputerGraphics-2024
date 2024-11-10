//
// Created by wookie on 11/7/24.
//

#include "geometry/Triangle.h"
#include "geometry/Vertex.h"
#include "models/DrawData.h"
#include "settings/Settings.h"
#include <QColor>
#include <QDebug>
#include <QMatrix4x4>
#include <QVector2D>
#include <cmath>

[[maybe_unused]] Triangle::Triangle(Vertex a, Vertex b, Vertex c) : _a(a), _b(b), _c(c) {}

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

    QVector3D &posA = _a.getPositionTransformed();
    QVector3D &posB = _b.getPositionTransformed();
    QVector3D &posC = _c.getPositionTransformed();

    QVector3D &normalA = _a.getNormalTransformed();
    QVector3D &normalB = _b.getNormalTransformed();
    QVector3D &normalC = _c.getNormalTransformed();

    int width  = drawData.canvas.width();
    int height = drawData.canvas.height();

    qreal x0 = posA.x() * width;
    qreal y0 = posA.y() * height;
    qreal z0 = posA.z();

    qreal x1 = posB.x() * width;
    qreal y1 = posB.y() * height;
    qreal z1 = posB.z();

    qreal x2 = posC.x() * width;
    qreal y2 = posC.y() * height;
    qreal z2 = posC.z();

    struct Vertex
    {
        qreal x, y, z;
        QVector3D pos;
        QVector3D normal;
    };

    std::array<Vertex, 3> vertices = {
        {{x0, y0, z0, posA, normalA}, {x1, y1, z1, posB, normalB}, {x2, y2, z2, posC, normalC}}
    };

    std::sort(
        vertices.begin(), vertices.end(),
        [](const Vertex &a, const Vertex &b)
        {
            return a.y < b.y;
        }
    );

    struct Edge
    {
        qreal x;       // Current x-coordinate
        qreal xStep;   // Change in x per scan-line
        int yStart;    // Starting y-coordinate
        int yEnd;      // Ending y-coordinate
        Vertex vStart; // Starting vertex
        Vertex vEnd;   // Ending vertex
    };

    auto createEdge = [](const Vertex &v1, const Vertex &v2) -> Edge
    {
        int yStart  = static_cast<int>(std::ceil(v1.y));
        int yEnd    = static_cast<int>(std::ceil(v2.y)) - 1;
        qreal dy    = v2.y - v1.y;
        qreal dx    = v2.x - v1.x;
        qreal xStep = (dy != 0) ? dx / dy : 0.0;
        qreal x     = v1.x + (yStart - v1.y) * xStep;
        return {x, xStep, yStart, yEnd, v1, v2};
    };

    Edge edges[3] = {
        createEdge(vertices[0], vertices[1]), createEdge(vertices[1], vertices[2]), createEdge(vertices[0], vertices[2])
    };

    int minY = static_cast<int>(std::ceil(vertices[0].y));
    int maxY = static_cast<int>(std::ceil(vertices[2].y)) - 1;

    minY = std::max(0, minY);
    maxY = std::min(height - 1, maxY);

    int tableHeight = maxY - minY + 1;
    std::vector<Edge> edgeTable[tableHeight];

    for (const auto &edge : edges)
    {
        int yStart = std::max(edge.yStart, minY);
        int yEnd   = std::min(edge.yEnd, maxY);

        for (int y = yStart; y <= yEnd; ++y)
        {
            int idx = y - minY;
            edgeTable[idx].push_back(edge);
        }
    }
    if (settings.triangleSettings.debugDraw && drawData.textureOrBrushColor.canConvert<QColor>())
    {
        _a.draw(drawData);
        _b.draw(drawData);
        _c.draw(drawData);
    }

    auto drawPixel = [&](int x, int y, const QVector3D &pos, const QVector3D &normal)
    {
        if (drawData.textureOrBrushColor.canConvert<QColor>())
        {
            QColor color = drawData.textureOrBrushColor.value<QColor>();
            if (drawData.lightSource)
            {
                // Compute lighting
                QColor lightColor = drawData.lightSource->getColor();
                QVector3D L       = drawData.lightSource->calcVersorTo(pos).normalized();
                QVector3D N       = normal;
                QVector3D V(0.0f, 0.0f, 1.0f);

                float cosNL = std::max(0.0f, -QVector3D::dotProduct(N, L));

                QVector3D R = (2.0f * cosNL * N - L).normalized();
                float cosVR = std::max(0.0f, -QVector3D::dotProduct(V.normalized(), R));

                float kd = settings.lightSettings.kdCoef;
                float ks = settings.lightSettings.ksCoef;
                float m  = settings.lightSettings.m;

                // Compute intensity for each color component
                float r = kd * lightColor.redF() * color.redF() * cosNL +
                          ks * lightColor.redF() * color.redF() * std::pow(cosVR, m);
                float g = kd * lightColor.greenF() * color.greenF() * cosNL +
                          ks * lightColor.greenF() * color.greenF() * std::pow(cosVR, m);
                float b = kd * lightColor.blueF() * color.blueF() * cosNL +
                          ks * lightColor.blueF() * color.blueF() * std::pow(cosVR, m);

                // Clamp values to [0,1]
                r = std::min(1.0f, r);
                g = std::min(1.0f, g);
                b = std::min(1.0f, b);

                color = QColor::fromRgbF(r, g, b);
            }
            drawData.canvas.setPixelColor(x, y, color);
        }
        else
        {
            // TODO: Implement texture drawing
            QColor color = Qt::magenta;
            drawData.canvas.setPixelColor(x, y, color);
        }
    };

    auto drawPixelDebug = [&](int x, int y)
    {
        //        qreal dist0 = std::abs(A0 * x + B0 * y + C0) / std::sqrt(A0 * A0 + B0 * B0);
        //        qreal dist1 = std::abs(A1 * x + B1 * y + C1) / std::sqrt(A1 * A1 + B1 * B1);
        //        qreal dist2 = std::abs(A2 * x + B2 * y + C2) / std::sqrt(A2 * A2 + B2 * B2);
        //
        //        qreal minDist = std::min({dist0, dist1, dist2});

        qreal minDist = 1.0;

        if (minDist <= 0.5)
            drawData.canvas.setPixelColor(x, y, settings.triangleSettings.triangleEdgeColor);
        else
            drawData.canvas.setPixelColor(x, y, settings.triangleSettings.triangleFillColor);
    };

    for (int y = minY; y <= maxY; ++y)
    {
        int idx = y - minY;

        auto &activeEdges = edgeTable[idx];
        if (activeEdges.size() < 2)
            continue;

        for (auto &edge : activeEdges)
        {
            edge.x = edge.x + edge.xStep * (y - edge.yStart);
        }

        std::sort(
            activeEdges.begin(), activeEdges.end(),
            [](const Edge &a, const Edge &b)
            {
                return a.x < b.x;
            }
        );

        for (size_t i = 0; i < activeEdges.size(); i += 2)
        {
            if (i + 1 >= activeEdges.size())
                break;

            int xStart = static_cast<int>(std::ceil(activeEdges[i].x));
            int xEnd   = static_cast<int>(std::floor(activeEdges[i + 1].x));

            if (xStart > xEnd)
                continue;

            xStart = std::max(0, xStart);
            xEnd   = std::min(width - 1, xEnd);

            for (int x = xStart; x <= xEnd; ++x)
            {
                QVector3D barycentric = computeBarycentricCoordinates(
                    QVector2D(x, y), QVector2D(vertices[0].x, vertices[0].y), QVector2D(vertices[1].x, vertices[1].y),
                    QVector2D(vertices[2].x, vertices[2].y)
                );

                if (barycentric.x() < 0 || barycentric.y() < 0 || barycentric.z() < 0)
                    continue;

                qreal z =
                    barycentric.x() * vertices[0].z + barycentric.y() * vertices[1].z + barycentric.z() * vertices[2].z;

                if (z < drawData.zBuffer[x][y])
                    continue;
                drawData.zBuffer[x][y] = z;

                QVector3D pos = barycentric.x() * vertices[0].pos + barycentric.y() * vertices[1].pos +
                                barycentric.z() * vertices[2].pos;
                QVector3D normal = (barycentric.x() * vertices[0].normal + barycentric.y() * vertices[1].normal +
                                    barycentric.z() * vertices[2].normal)
                                       .normalized();

                if (settings.triangleSettings.debugDraw)
                {
                    drawPixelDebug(x, y);
                }
                else
                {
                    drawPixel(x, y, pos, normal);
                }
            }
        }
    }
}

QVector3D
Triangle::computeBarycentricCoordinates(const QVector2D &p, const QVector2D &a, const QVector2D &b, const QVector2D &c)
{
    qreal denom = (b.y() - c.y()) * (a.x() - c.x()) + (c.x() - b.x()) * (a.y() - c.y());
    if (denom == 0)
        return QVector3D(-1, -1, -1);

    qreal w0 = ((b.y() - c.y()) * (p.x() - c.x()) + (c.x() - b.x()) * (p.y() - c.y())) / denom;
    qreal w1 = ((c.y() - a.y()) * (p.x() - c.x()) + (a.x() - c.x()) * (p.y() - c.y())) / denom;
    qreal w2 = 1.0 - w0 - w1;

    return QVector3D(w0, w1, w2);
}

void Triangle::transform(QMatrix4x4 &matrix)
{
    _a.transform(matrix);
    _b.transform(matrix);
    _c.transform(matrix);
}