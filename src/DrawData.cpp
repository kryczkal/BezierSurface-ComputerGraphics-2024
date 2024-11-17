//
// Created by wookie on 11/8/24.
//

#include "models/DrawData.h"

DrawData::DrawData(QImage &canvas) : canvas(canvas)
{
    setBrushColor(Qt::magenta);
    X = canvas.width();
    Y = canvas.height();
    initZBuffer();
}

DrawData::DrawData(QImage &canvas, const QImage &texture) : canvas(canvas)
{
    setTexture(texture);
    X = canvas.width();
    Y = canvas.height();
    initZBuffer();
}

DrawData::DrawData(QImage &canvas, const QColor &brushColor) : canvas(canvas)
{
    setBrushColor(brushColor);
    X = canvas.width();
    Y = canvas.height();
    initZBuffer();
}

void DrawData::initZBuffer()
{
    zBuffer.reset(new float[X * Y]);
    clearZBuffer();
}

void DrawData::clearZBuffer() const
{
    for (int i = 0; i < X; i++)
    {
        for (int j = 0; j < Y; j++)
        {
            zBuffer.data()[i * Y + j] = -std::numeric_limits<float>::max();
        }
    }
}

void DrawData::setBrushColor(const QColor &color)
{
    brushColor = color;
    texture    = nullptr;
}

void DrawData::setTexture(const QImage &texture) { this->texture = QSharedPointer<QImage>::create(texture); }
