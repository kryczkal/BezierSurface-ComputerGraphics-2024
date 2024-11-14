//
// Created by wookie on 11/8/24.
//

#include "models/DrawData.h"

DrawData::DrawData(QImage &canvas) : canvas(canvas)
{
    setBrushColor(Qt::magenta);
    X       = canvas.width();
    Y       = canvas.height();
    zBuffer = QVector<QVector<float>>(X, QVector<float>(Y, -std::numeric_limits<float>::infinity()));
}

DrawData::DrawData(QImage &canvas, const QImage &texture) : canvas(canvas)
{
    setTexture(texture);
    X       = canvas.width();
    Y       = canvas.height();
    zBuffer = QVector<QVector<float>>(X, QVector<float>(Y, -std::numeric_limits<float>::infinity()));
}

DrawData::DrawData(QImage &canvas, const QColor &brushColor) : canvas(canvas)
{
    setBrushColor(brushColor);
    X       = canvas.width();
    Y       = canvas.height();
    zBuffer = QVector<QVector<float>>(X, QVector<float>(Y, -std::numeric_limits<float>::infinity()));
}

void DrawData::clear(const QColor &color)
{
    QMutexLocker locker(&zBufferMutex);
    canvas.fill(color);
    clearZBuffer();
}

void DrawData::clear()
{
    QMutexLocker locker(&zBufferMutex);
    canvas.fill(Qt::white);
    clearZBuffer();
}

void DrawData::clearZBuffer()
{
    QMutexLocker locker(&zBufferMutex);
    for (int i = 0; i < X; i++)
    {
        for (int j = 0; j < Y; j++)
        {
            zBuffer[i][j] = -std::numeric_limits<float>::infinity();
        }
    }
}

void DrawData::setBrushColor(const QColor &color)
{
    brushColor = color;
    texture    = nullptr;
}

void DrawData::setTexture(const QImage &texture) { this->texture = QSharedPointer<QImage>::create(texture); }
