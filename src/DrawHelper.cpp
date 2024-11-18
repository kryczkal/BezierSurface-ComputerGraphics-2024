//
// Created by wookie on 11/8/24.
//

#include "settings/Settings.h"
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

void

DrawUtils::drawPixel(DrawData &drawData, const QVector3D &position3d, const QVector3D &normal, QColor &color, const int x, const int y)
{
    Settings &settings = Settings::getInstance();

    if (settings.lightSettings.isLightSourceEnabled && !drawData.lightSources.empty())
    {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        for (QSharedPointer<LightSource> lightSource : drawData.lightSources)
        {
            QColor lightColor = lightSource->getColor();
            QVector3D L       = lightSource->calcVersorTo(position3d).normalized();
            QVector3D N       = normal;
            QVector3D V(0.0f, 0.0f, -1.0f);

            float cosNL = std::max(0.0f, QVector3D::dotProduct(N, L));
            QVector3D R = (2.0f * QVector3D::dotProduct(N, L) * N - L).normalized();
            float cosVR = std::max(0.0f, QVector3D::dotProduct(V, R));

            const float kd = settings.lightSettings.kdCoef;
            const float ks = settings.lightSettings.ksCoef;
            const float m  = settings.lightSettings.m;

            float lightPower = 1.0f;
            if (settings.lightSettings.isReflectorEnabled)
            {
                const int mCoeffReflector = settings.lightSettings.mCoeffReflector;
                lightPower =
                    std::pow(std::max(0.0f, QVector3D::dotProduct(L, lightSource->getDirection())), mCoeffReflector);
            }

            float IL_r = lightColor.redF() * lightPower;
            float IL_g = lightColor.greenF() * lightPower;
            float IL_b = lightColor.blueF() * lightPower;

            float IO_r = color.redF();
            float IO_g = color.greenF();
            float IO_b = color.blueF();

            r += kd * IL_r * IO_r * cosNL + ks * IL_r * IO_r * std::pow(cosVR, m);
            g += kd * IL_g * IO_g * cosNL + ks * IL_g * IO_g * std::pow(cosVR, m);
            b += kd * IL_b * IO_b * cosNL + ks * IL_b * IO_b * std::pow(cosVR, m);

            r = std::min(1.0f, r);
            g = std::min(1.0f, g);
            b = std::min(1.0f, b);
        }
        color = QColor::fromRgbF(r, g, b);
    }

    drawData.canvas.setPixelColor(x, y, color);
}
