//
// Created by wookie on 11/8/24.
//

#include "utils/DrawHelper.h"
#include <QVector3D>
#include <cmath>

void DrawHelper::drawLine(
    DrawData &drawData, const QVector3D &start, const QVector3D &end, const QColor &color, float width
)
{
    int x0   = start.x() * drawData.canvas.width();
    int y0   = start.y() * drawData.canvas.height();
    int x1   = end.x() * drawData.canvas.width();
    int y1   = end.y() * drawData.canvas.height();
    float z0 = start.z();
    float z1 = end.z();

    int dx  = std::abs(x1 - x0);
    int dy  = -std::abs(y1 - y0);
    int sx  = x0 < x1 ? 1 : -1;
    int sy  = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    float totalLength = std::sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    float halfWidth   = std::max(1.0f, width / 2.0f);

    while (true)
    {
        float currentLength = std::sqrt(
            (x0 - start.x() * drawData.canvas.width()) * (x0 - start.x() * drawData.canvas.width()) +
            (y0 - start.y() * drawData.canvas.height()) * (y0 - start.y() * drawData.canvas.height())
        );
        float t      = totalLength == 0 ? 0.0f : currentLength / totalLength;
        float zValue = z0 + t * (z1 - z0);

        for (int wx = -halfWidth; wx <= halfWidth; ++wx)
        {
            for (int wy = -halfWidth; wy <= halfWidth; ++wy)
            {
                int px = x0 + wx;
                int py = y0 + wy;
                if (px >= 0 && px < drawData.canvas.width() && py >= 0 && py < drawData.canvas.height())
                {
                    if (zValue < drawData.zBuffer[px][py])
                        continue;
                    drawData.zBuffer[px][py] = zValue;
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
