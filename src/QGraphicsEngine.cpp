//
// Created by wookie on 11/7/24.
//
#include "graphics/QGraphicsEngine.h"
#include "models/DrawData.h"
#include "utils/Settings.h"
#include <QColor>
#include <QMutex>
#include <QPainter>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include <QWidget>

QGraphicsEngine::QGraphicsEngine(int width, int height) : _width(width), _height(height)
{
    _qImage = QImage(_width, _height, QImage::Format_ARGB32);
    _qImage.fill(Settings::getInstance().backgroundColor);
}

QRectF QGraphicsEngine::boundingRect() const { return {0, 0, static_cast<qreal>(_width), static_cast<qreal>(_height)}; }

void QGraphicsEngine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(0, 0, _qImage);
}

void QGraphicsEngine::testPixmap()
{
    QMutexLocker locker(&_drawMutex);

    QVector<QColor> colors = {Qt::red,     Qt::green, Qt::blue,  Qt::yellow, Qt::cyan,
                              Qt::magenta, Qt::gray,  Qt::black, Qt::white};

    QRandomGenerator *randomGen = QRandomGenerator::global();

    QColor currentColor = colors[randomGen->bounded(colors.size())];

    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            if (randomGen->bounded(100) < 5)
            {
                currentColor = colors[randomGen->bounded(colors.size())];
            }

            _qImage.setPixelColor(x, y, currentColor);
        }
    }

    update();
}

int QGraphicsEngine::getWidth() const { return _width; }

int QGraphicsEngine::getHeight() const { return _height; }

QImage QGraphicsEngine::getQImage() const { return _qImage; }

void QGraphicsEngine::addDrawable(QSharedPointer<QGraphicsEngineDrawable> &drawable)
{
    QMutexLocker locker(&_drawMutex);
    _drawables.append(drawable);
}

void QGraphicsEngine::clearDrawables() { _drawables.clear(); }

void QGraphicsEngine::draw()
{
    QMutexLocker locker(&_drawMutex);
    DrawData drawData(_qImage, Qt::darkRed);

    for (auto &drawable : _drawables)
    {
        drawable->draw(drawData);
    }

    update();
}
