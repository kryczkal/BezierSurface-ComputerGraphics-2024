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
    struct Vertex
    {
        float x, y, z;
        QVector3D pos;
        QVector3D normal;
        float u, v;
    };

    struct Edge
    {
        int yMax;
        float x;
        float xStep;
    };

    Settings &settings = Settings::getInstance();

    QVector3D posA = _a.getPositionTransformed();
    QVector3D posB = _b.getPositionTransformed();
    QVector3D posC = _c.getPositionTransformed();

    QVector3D normalA = _a.getNormalTransformed().normalized();
    QVector3D normalB = _b.getNormalTransformed().normalized();
    QVector3D normalC = _c.getNormalTransformed().normalized();

    int width  = drawData.canvas.width();
    int height = drawData.canvas.height();

    float x0 = posA.x() * width;
    float y0 = posA.y() * height;
    float z0 = posA.z();

    float x1 = posB.x() * width;
    float y1 = posB.y() * height;
    float z1 = posB.z();

    float x2 = posC.x() * width;
    float y2 = posC.y() * height;
    float z2 = posC.z();

    Vertex v0 = {x0, y0, z0, posA, normalA, _a.getU(), _a.getV()};
    Vertex v1 = {x1, y1, z1, posB, normalB, _b.getU(), _b.getV()};
    Vertex v2 = {x2, y2, z2, posC, normalC, _c.getU(), _c.getV()};

    std::array<Vertex, 3> vertices = {v0, v1, v2};
    std::sort(
        vertices.begin(), vertices.end(),
        [](const Vertex &a, const Vertex &b)
        {
            return a.y < b.y;
        }
    );

    int minY = static_cast<int>(std::ceil(vertices[0].y));
    int maxY = static_cast<int>(std::floor(vertices[2].y));

    minY = std::max(minY, 0);
    maxY = std::min(maxY, height - 1);

    auto createEdge = [](const Vertex &vStart, const Vertex &vEnd) -> Edge
    {
        if (vStart.y == vEnd.y)
            return {0, 0, 0};

        float dy = vEnd.y - vStart.y;
        float dx = vEnd.x - vStart.x;

        float xStep = dx / dy;
        int yMin    = static_cast<int>(std::ceil(vStart.y));
        int yMax    = static_cast<int>(std::ceil(vEnd.y)) - 1;

        float x = vStart.x + (yMin - vStart.y) * xStep;

        return {yMax, x, xStep};
    };

    std::vector<std::vector<Edge>> edgeTable(height);

    auto addEdgeToTable = [&](const Vertex &vStart, const Vertex &vEnd)
    {
        if (vStart.y == vEnd.y)
            return;

        Edge edge  = createEdge(vStart, vEnd);
        int yIndex = static_cast<int>(std::ceil(vStart.y));
        if (yIndex >= 0 && yIndex < height)
            edgeTable[yIndex].push_back(edge);
    };

    addEdgeToTable(vertices[0], vertices[1]);
    addEdgeToTable(vertices[1], vertices[2]);
    addEdgeToTable(vertices[0], vertices[2]);

    std::vector<Edge> activeEdgeTable;

    for (int y = minY; y <= maxY; ++y)
    {
        if (y >= 0 && y < height)
        {
            activeEdgeTable.insert(activeEdgeTable.end(), edgeTable[y].begin(), edgeTable[y].end());
        }

        activeEdgeTable.erase(
            std::remove_if(
                activeEdgeTable.begin(), activeEdgeTable.end(),
                [y](const Edge &e)
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
            [](const Edge &a, const Edge &b)
            {
                return a.x < b.x;
            }
        );

        for (size_t i = 0; i + 1 < activeEdgeTable.size(); i += 2)
        {
            int xStart = static_cast<int>(std::ceil(activeEdgeTable[i].x));
            int xEnd   = static_cast<int>(std::floor(activeEdgeTable[i + 1].x));

            xStart = std::max(xStart, 0);
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

                if (z < drawData.zBuffer[x][y])
                    continue;
                drawData.zBuffer[x][y] = z;

                QVector3D pos = barycentric.x() * vertices[0].pos + barycentric.y() * vertices[1].pos +
                                barycentric.z() * vertices[2].pos;
                QVector3D normal = (barycentric.x() * vertices[0].normal + barycentric.y() * vertices[1].normal +
                                    barycentric.z() * vertices[2].normal)
                                       .normalized();

                float u =
                    barycentric.x() * vertices[0].u + barycentric.y() * vertices[1].u + barycentric.z() * vertices[2].u;
                float v =
                    barycentric.x() * vertices[0].v + barycentric.y() * vertices[1].v + barycentric.z() * vertices[2].v;

                int uX = 0;
                int vY = 0;

                if (drawData.normalMap)
                {

                    uX                 = static_cast<int>(u * drawData.normalMap->width());
                    vY                 = static_cast<int>(v * drawData.normalMap->height());
                    QColor normalColor = drawData.normalMap->pixelColor(uX, vY);
                    normal             = -QVector3D(
                                  normalColor.redF() * 2.0f - 1.0f, normalColor.greenF() * 2.0f - 1.0f,
                                  normalColor.blueF() * 2.0f - 1.0f
                    )
                                  .normalized();
                }

                QColor color;
                if (drawData.texture)
                {
                    uX    = static_cast<int>(u * drawData.texture->width());
                    vY    = static_cast<int>(v * drawData.texture->height());
                    color = drawData.texture->pixelColor(uX, vY);
                }
                else
                {
                    color = drawData.brushColor;
                }

                if (settings.triangleSettings.debugDraw)
                {
                    drawPixelDebug(drawData, x, y);
                }
                else
                {
                    drawPixel(drawData, pos, normal, color, x, y);
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

void Triangle::drawPixel(DrawData &drawData, const QVector3D &pos, const QVector3D &normal, QColor &color, int x, int y)
{
    Settings &settings = Settings::getInstance();

    if (settings.lightSettings.isLightSourceEnabled && drawData.lightSource)
    {
        QColor lightColor = drawData.lightSource->getColor();
        QVector3D L       = drawData.lightSource->calcVersorTo(pos).normalized();
        QVector3D N       = normal;
        QVector3D V(0.0f, 0.0f, 1.0f);

        float cosNL = std::max(0.0f, QVector3D::dotProduct(N, L));
        QVector3D R = (2.0f * QVector3D::dotProduct(N, L) * N - L).normalized();
        float cosVR = std::max(0.0f, QVector3D::dotProduct(V, R));

        float kd = settings.lightSettings.kdCoef;
        float ks = settings.lightSettings.ksCoef;
        float m  = settings.lightSettings.m;

        float IL_r = lightColor.redF();
        float IL_g = lightColor.greenF();
        float IL_b = lightColor.blueF();

        float IO_r = color.redF();
        float IO_g = color.greenF();
        float IO_b = color.blueF();

        float r = kd * IL_r * IO_r * cosNL + ks * IL_r * IO_r * std::pow(cosVR, m);
        float g = kd * IL_g * IO_g * cosNL + ks * IL_g * IO_g * std::pow(cosVR, m);
        float b = kd * IL_b * IO_b * cosNL + ks * IL_b * IO_b * std::pow(cosVR, m);

        r = std::min(1.0f, r);
        g = std::min(1.0f, g);
        b = std::min(1.0f, b);

        color = QColor::fromRgbF(r, g, b);
    }

    drawData.canvas.setPixelColor(x, y, color);
}

void Triangle::drawPixelDebug(DrawData &drawData, int x, int y)
{
    drawData.canvas.setPixelColor(x, y, Settings::getInstance().triangleSettings.triangleFillColor);
}
void Triangle::transform(QMatrix4x4 &matrix)
{
    _a.transform(matrix);
    _b.transform(matrix);
    _c.transform(matrix);
}