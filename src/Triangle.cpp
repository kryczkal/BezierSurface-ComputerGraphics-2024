//
// Created by wookie on 11/7/24.
//

#include "geometry/Triangle.h"
#include "geometry/Vertex.h"
#include "models/DrawData.h"
#include "settings/Settings.h"
#include "utils/DrawUtils.h"
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

    const QVector3D posA = _a.getPositionTransformed();
    const QVector3D posB = _b.getPositionTransformed();
    const QVector3D posC = _c.getPositionTransformed();

    const QVector3D normalA = _a.getNormalTransformed().normalized();
    const QVector3D normalB = _b.getNormalTransformed().normalized();
    const QVector3D normalC = _c.getNormalTransformed().normalized();

    const int width  = drawData.canvas.width();
    const int height = drawData.canvas.height();

    float x0 = posA.x() * width;
    float y0 = posA.y() * height;
    float z0 = posA.z();

    float x1 = posB.x() * width;
    float y1 = posB.y() * height;
    float z1 = posB.z();

    float x2 = posC.x() * width;
    float y2 = posC.y() * height;
    float z2 = posC.z();

    const VertexStruct v0 = {
        x0, y0, z0, posA, normalA, _a.getU(), _a.getV(), _a.getUTangentTransformed(), _a.getVTangentTransformed()
    };
    const VertexStruct v1 = {
        x1, y1, z1, posB, normalB, _b.getU(), _b.getV(), _b.getUTangentTransformed(), _b.getVTangentTransformed()
    };
    const VertexStruct v2 = {
        x2, y2, z2, posC, normalC, _c.getU(), _c.getV(), _c.getUTangentTransformed(), _c.getVTangentTransformed()
    };

    std::array<VertexStruct, 3> vertices = {v0, v1, v2};
    std::sort(
        vertices.begin(), vertices.end(),
        [](const VertexStruct &a, const VertexStruct &b)
        {
            return a.y < b.y;
        }
    );

    const int minY = std::max(static_cast<int>(std::ceil(vertices[0].y)), 0);
    const int maxY = std::min(static_cast<int>(std::floor(vertices[2].y)), height - 1);

    auto createEdge = [](const VertexStruct &vStart, const VertexStruct &vEnd) -> EdgeStruct
    {
        if (vStart.y == vEnd.y)
            return {0, 0, 0};

        const float dy = vEnd.y - vStart.y;
        const float dx = vEnd.x - vStart.x;

        const float xStep = dx / dy;
        const int yMin    = static_cast<int>(std::ceil(vStart.y));
        const int yMax    = static_cast<int>(std::ceil(vEnd.y)) - 1;

        const float x = vStart.x + (yMin - vStart.y) * xStep;

        return {yMax, x, xStep};
    };

    std::vector<std::vector<EdgeStruct>> edgeTable(height);

    auto addEdgeToTable = [&](const VertexStruct &vStart, const VertexStruct &vEnd)
    {
        if (vStart.y == vEnd.y)
            return;

        const EdgeStruct edge = createEdge(vStart, vEnd);
        const int yIndex      = static_cast<int>(std::ceil(vStart.y));
        if (yIndex >= 0 && yIndex < height)
            edgeTable[yIndex].push_back(edge);
    };

    addEdgeToTable(vertices[0], vertices[1]);
    addEdgeToTable(vertices[1], vertices[2]);
    addEdgeToTable(vertices[0], vertices[2]);

    std::vector<EdgeStruct> activeEdgeTable;

    for (int y = minY; y <= maxY; ++y)
    {
        if (y >= 0 && y < height)
        {
            activeEdgeTable.insert(activeEdgeTable.end(), edgeTable[y].begin(), edgeTable[y].end());
        }

        activeEdgeTable.erase(
            std::remove_if(
                activeEdgeTable.begin(), activeEdgeTable.end(),
                [y](const EdgeStruct &e)
                {
                    return y > e.yMax;
                }
            ),
            activeEdgeTable.end()
        );

        if (activeEdgeTable.size() < 2)
            continue;

        std::sort(
            activeEdgeTable.begin(), activeEdgeTable.end(),
            [](const EdgeStruct &a, const EdgeStruct &b)
            {
                return a.x < b.x;
            }
        );

        for (size_t i = 0; i + 1 < activeEdgeTable.size(); i += 2)
        {
            int xStart = static_cast<int>(std::ceil(activeEdgeTable[i].x));
            int xEnd   = static_cast<int>(std::floor(activeEdgeTable[i + 1].x));

            xStart = std::max(xStart - 1, 0);
            xEnd   = std::min(xEnd, width - 1);

            for (int x = xStart; x <= xEnd; ++x)
            {
                QVector3D barycentric = computeBarycentricCoordinates(
                    QVector2D(x, y), QVector2D(vertices[0].x, vertices[0].y), QVector2D(vertices[1].x, vertices[1].y),
                    QVector2D(vertices[2].x, vertices[2].y)
                );

                if (barycentric.x() < 0 || barycentric.y() < 0 || barycentric.z() < 0)
                    continue;

                float z =
                    barycentric.x() * vertices[0].z + barycentric.y() * vertices[1].z + barycentric.z() * vertices[2].z;

                if (z < drawData.zBuffer.data()[x * height + y])
                    continue;
                drawData.zBuffer.data()[x * height + y] = z;

                const QVector3D pos = barycentric.x() * vertices[0].pos + barycentric.y() * vertices[1].pos +
                                      barycentric.z() * vertices[2].pos;
                QVector3D normal = (barycentric.x() * vertices[0].normal + barycentric.y() * vertices[1].normal +
                                    barycentric.z() * vertices[2].normal)
                                       .normalized();
                const float u =
                    barycentric.x() * vertices[0].u + barycentric.y() * vertices[1].u + barycentric.z() * vertices[2].u;
                const float v =
                    barycentric.x() * vertices[0].v + barycentric.y() * vertices[1].v + barycentric.z() * vertices[2].v;

                if (drawData.normalMap)
                {
                    const QVector3D uTangent = barycentric.x() * vertices[0].uTangent +
                                               barycentric.y() * vertices[1].uTangent +
                                               barycentric.z() * vertices[2].uTangent;
                    const QVector3D vTangent = barycentric.x() * vertices[0].vTangent +
                                               barycentric.y() * vertices[1].vTangent +
                                               barycentric.z() * vertices[2].vTangent;
                    getNormalFromMap(drawData, vertices, u, v, normal, uTangent, vTangent);
                }

                QColor color;
                getColor(drawData, u, v, color);

                if (settings.triangleSettings.debugDraw)
                {
                    drawPixelDebug(drawData, settings, y, x, barycentric, pos, normal);
                }
                else
                {
                    DrawUtils::drawPixel(drawData, pos, normal, color, x, y);
                }
            }
        }

        for (auto &edge : activeEdgeTable)
        {
            edge.x += edge.xStep;
        }
    }

    if (settings.triangleSettings.debugDraw)
    {
        _a.draw(drawData);
        _b.draw(drawData);
        _c.draw(drawData);
    }
}

void Triangle::getNormalFromMap(
    const DrawData &drawData, const std::array<VertexStruct, 3> &vertices, float u, float v, QVector3D &normal,
    const QVector3D &uTangent, const QVector3D &vTangent
)
{
    const int uX = qBound(0, static_cast<int>(u * drawData.normalMap->width()), drawData.normalMap->width() - 1);
    const int vY = qBound(0, static_cast<int>(v * drawData.normalMap->height()), drawData.normalMap->height() - 1);
    const QColor normalColor = drawData.normalMap->pixelColor(uX, vY);
    const QVector3D textureVector =
        QVector3D(
            normalColor.redF() * 2.0f - 1.0f, normalColor.greenF() * 2.0f - 1.0f, normalColor.blueF() * 2.0f - 1.0f
        )
            .normalized();
    QMatrix4x4 M3x3;
    M3x3.setColumn(0, uTangent);
    M3x3.setColumn(1, vTangent);
    M3x3.setColumn(2, normal);
    M3x3 = M3x3.inverted();

    const QVector3D normalMapNormal = (M3x3 * textureVector).normalized();
    normal                          = -textureVector;
}

QColor &Triangle::getColor(const DrawData &drawData, float u, float v, QColor &color)
{
    if (drawData.texture)
    {
        const int uX = qBound(0, static_cast<int>(u * drawData.texture->width()), drawData.texture->width() - 1);
        const int vY = qBound(0, static_cast<int>(v * drawData.texture->height()), drawData.texture->height() - 1);
        color        = drawData.texture->pixelColor(uX, vY);
    }
    else
    {
        color = drawData.brushColor;
    }
    return color;
}

void Triangle::drawPixelDebug(
    DrawData &drawData, Settings &settings, int y, int x, const QVector3D &barycentric, const QVector3D &pos,
    const QVector3D &normal
)
{
    drawData.lightSource = nullptr;
    const float coeff    = settings.triangleSettings.triangleEdgeDrawProximityCoef;
    if (barycentric.x() < coeff || barycentric.y() < coeff || barycentric.z() < coeff)
    {
        DrawUtils::drawPixel(drawData, pos, normal, settings.triangleSettings.triangleEdgeColor, x, y);
    }
    else
    {
        DrawUtils::drawPixel(drawData, pos, normal, settings.triangleSettings.triangleFillColor, x, y);
    }
}

QVector3D
Triangle::computeBarycentricCoordinates(const QVector2D &p, const QVector2D &a, const QVector2D &b, const QVector2D &c)
{
    QVector2D v0 = b - a, v1 = c - a, v2 = p - a;
    float denom = v0.x() * v1.y() - v1.x() * v0.y();
    if (denom == 0)
        return QVector3D(-1, -1, -1);

    float invDenom = 1.0f / denom;
    float w1       = (v2.x() * v1.y() - v1.x() * v2.y()) * invDenom;
    float w2       = (v0.x() * v2.y() - v2.x() * v0.y()) * invDenom;
    float w0       = 1.0f - w1 - w2;

    return QVector3D(w0, w1, w2);
}

void Triangle::transform(QMatrix4x4 &matrix)
{
    _a.transform(matrix);
    _b.transform(matrix);
    _c.transform(matrix);
}
