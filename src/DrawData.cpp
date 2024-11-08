//
// Created by wookie on 11/8/24.
//

#include "models/DrawData.h"

DrawData::DrawData(QImage &canvas, const QColor &brushColor) : canvas(canvas)
{
    setBrushColor(brushColor);
    X       = canvas.width();
    Y       = canvas.height();
    zBuffer = QVector<QVector<float>>(X, QVector<float>(Y, std::numeric_limits<float>::min()));
}

void DrawData::clear(const QColor &color)
{
    canvas.fill(color);
    clearZBuffer();
}

void DrawData::clear()
{
    canvas.fill(Qt::white);
    clearZBuffer();
}

void DrawData::clearZBuffer()
{
    for (int i = 0; i < X; i++)
    {
        for (int j = 0; j < Y; j++)
        {
            zBuffer[i][j] = std::numeric_limits<float>::min();
        }
    }
}

void DrawData::setBrushColor(const QColor &color) { textureOrBrushColor = QVariant::fromValue(color); }

void DrawData::setTexture(const QImage &texture) { textureOrBrushColor = QVariant::fromValue(texture); }
