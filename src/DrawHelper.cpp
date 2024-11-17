//
// Created by wookie on 11/8/24.
//

#include "utils/DrawUtils.h"
#include <QDebug>
#include <QVector3D>
#include <cmath>

void DrawUtils::drawLine(
    DrawData &drawData, const QVector3D &start, const QVector3D &end, const QColor &color, float width
)
{
    const int canvasWidth  = drawData.canvas.width();
    const int canvasHeight = drawData.canvas.height();
    int x0                 = (int)(start.x() * canvasWidth);
    int y0                 = (int)(start.y() * canvasHeight);
    int x1                 = (int)(end.x() * canvasWidth);
    int y1                 = (int)(end.y() * canvasHeight);
    float z0               = start.z();
    float z1               = end.z();

    int dx  = std::abs(x1 - x0);
    int dy  = -std::abs(y1 - y0);
    int sx  = x0 < x1 ? 1 : -1;
    int sy  = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    auto totalLength = (float)std::sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    float halfWidth  = std::max(1.0f, width / 2.0f);

    while (true)
    {
        float currentLength = std::sqrt(
            (x0 - start.x() * canvasWidth) * (x0 - start.x() * canvasWidth) +
            (y0 - start.y() * canvasHeight) * (y0 - start.y() * canvasHeight)
        );
        float t      = totalLength == 0 ? 0.0f : currentLength / totalLength;
        float zValue = z0 + t * (z1 - z0);

        for (int wx = -halfWidth; wx <= halfWidth; ++wx)
        {
            for (int wy = -halfWidth; wy <= halfWidth; ++wy)
            {
                int px = x0 + wx;
                int py = y0 + wy;
                if (px >= 0 && px < canvasWidth && py >= 0 && py < canvasHeight)
                {
                    if (zValue < drawData.zBuffer.data()[px * canvasWidth + py])
                        continue;
                    drawData.zBuffer.data()[px * canvasWidth + py] = zValue;
                    drawData.canvas.setPixelColor(px, py, color);
                }
            }
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void DrawUtils::drawPoint(DrawData &drawData, const QVector3D &point, const QColor &color, int radiusX, int radiusY)
{
    const int canvasWidth  = drawData.canvas.width();
    const int canvasHeight = drawData.canvas.height();
    int x                  = point.x() * canvasWidth;
    int y                  = point.y() * canvasHeight;
    for (int xm = x - radiusX; xm <= x + radiusX; ++xm)
    {
        for (int ym = y - radiusY; ym <= y + radiusY; ++ym)
        {
            if (xm >= 0 && xm < canvasWidth && ym >= 0 && ym < canvasHeight)
            {
                if (point.z() < drawData.zBuffer.data()[xm * canvasWidth + ym])
                    continue;
                drawData.zBuffer.data()[xm * canvasWidth + ym] = point.z();
                drawData.canvas.setPixelColor(xm, ym, color);
            }
        }
    }
}
