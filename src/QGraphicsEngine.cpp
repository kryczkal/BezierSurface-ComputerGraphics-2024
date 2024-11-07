//
// Created by wookie on 11/7/24.
//
#include "graphics/QGraphicsEngine.h"
#include <QColor>
#include <QMutex>
#include <QPainter>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include <QWidget>
#include <QtConcurrent/QtConcurrentRun>

QGraphicsEngine::QGraphicsEngine(int width, int height)
    : _width(width), _height(height) {
  _qImage = QImage(_width, _height, QImage::Format_ARGB32);
  _qImage.fill(Qt::darkRed);
}

QRectF QGraphicsEngine::boundingRect() const {
  return {0, 0, static_cast<qreal>(_width), static_cast<qreal>(_height)};
}

void QGraphicsEngine::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget) {
  painter->drawImage(0, 0, _qImage);
}

void QGraphicsEngine::testPixmap() {
  QMutexLocker locker(&_drawMutex);

  QVector<QColor> colors = {Qt::red,    Qt::green, Qt::blue,
                            Qt::yellow, Qt::cyan,  Qt::magenta,
                            Qt::gray,   Qt::black, Qt::white};

  QColor currentColor = colors[qrand() % colors.size()];
  for (int x = 0; x < _width; x++) {
    for (int y = 0; y < _height; y++) {
      if (qrand() % 100 < 5) {
        currentColor = colors[qrand() % colors.size()];
      }
      _qImage.setPixelColor(x, y, currentColor);
    }
  }

  update();
}
