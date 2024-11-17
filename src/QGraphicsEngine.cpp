//
// Created by wookie on 11/7/24.
//
#include "graphics/QGraphicsEngine.h"
#include "models/DrawData.h"
#include "qobject.h"
#include "settings/Settings.h"
#include "utils/VectorMovementUtils.h"
#include <QColor>
#include <QDateTime>
#include <QMatrix4x4>
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
    _qImage.fill(Settings::getInstance().graphicsEngineSettings.backgroundColor);
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
    _qImage.fill(Settings::getInstance().graphicsEngineSettings.backgroundColor);
    DrawData drawData(_qImage);
    drawData.lightSource = _lightSources.size() > 0 ? _lightSources[0].data() : nullptr;

    //    QMutexLocker locker(&_drawMutex);
    for (auto &drawable : _drawables)
    {
        drawable->draw(drawData);
    }

    for (auto &lightSource : _lightSources)
    {
        lightSource->draw(drawData);
    }

    autoMoveLightSources();

    update();
}

void QGraphicsEngine::setRotationX(float rotationX)
{
    _rotationX = rotationX;
    rotate(_rotationX, _rotationY, _rotationZ);
}

void QGraphicsEngine::setRotationY(float rotationY)
{
    _rotationY = rotationY;
    rotate(_rotationX, _rotationY, _rotationZ);
}

void QGraphicsEngine::setRotationZ(float rotationZ)
{
    _rotationZ = rotationZ;
    rotate(_rotationX, _rotationY, _rotationZ);
}

void QGraphicsEngine::rotate(float x, float y, float z)
{
    QMatrix4x4 rotationMatrix;

    rotationMatrix.rotate(x, 1, 0, 0);
    rotationMatrix.rotate(y, 0, 1, 0);
    rotationMatrix.rotate(z, 0, 0, 1);

    QMutexLocker locker(&_drawMutex);
    for (auto &drawable : _drawables)
    {
        drawable->transform(rotationMatrix);
    }
}

void QGraphicsEngine::addLightSource(QSharedPointer<LightSource> lightSource)
{
    QMutexLocker locker(&_drawMutex);
    _lightSources.append(lightSource);
}

void QGraphicsEngine::clearLightSources()
{
    QMutexLocker locker(&_drawMutex);
    _lightSources.clear();
}

void QGraphicsEngine::autoMoveLightSources()
{
    Settings &settings = Settings::getInstance();

    QDateTime currentDateTime          = QDateTime::currentDateTime();
    qint64 currentMSecsSinceEpoch      = currentDateTime.toMSecsSinceEpoch();
    settings.lightSettings.orbitRadius = 0.15f + 0.10f * sin(currentMSecsSinceEpoch / 1000.0f);

    for (auto &lightSource : _lightSources)
    {
        QVector3D &position = lightSource->getPosition();
        VectorMovementUtils::moveAcrossCircle(
            position, settings.lightSettings.orbitCenter, settings.lightSettings.orbitRadius,
            settings.lightSettings.orbitSpeed
        );
    }
}

void QGraphicsEngine::setRotation(float x, float y, float z)
{
    _rotationX = x;
    _rotationY = y;
    _rotationZ = z;
    rotate(_rotationX, _rotationY, _rotationZ);
}

void QGraphicsEngine::setupAnimationTimer()
{
    if (!_animationTimer)
    {
        _animationTimer = QSharedPointer<QTimer>::create();
    }

    if (_animationTimer->isActive())
    {
        _animationTimer->stop();
        _animationTimer->disconnect();
    }

    QObject::connect(
        _animationTimer.data(), &QTimer::timeout,
        [this]()
        {
            this->autoMoveLightSources();
            this->draw();
        }
    );
    _animationTimer->start(1000 / 60);
}
