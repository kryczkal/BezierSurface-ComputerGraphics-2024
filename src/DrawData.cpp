//
// Created by wookie on 11/8/24.
//

#include "models/DrawData.h"

DrawData::DrawData(QImage &canvas, const QColor &color) : canvas(canvas) {
  setColor(color);
  X = canvas.width();
  Y = canvas.height();
  zBuffer = QVector<QVector<float>>(
      X, QVector<float>(Y, std::numeric_limits<float>::max()));
}

void DrawData::clear(const QColor &color) {
  canvas.fill(color);
  clearZBuffer();
}

void DrawData::clear() {
  canvas.fill(Qt::white);
  clearZBuffer();
}

void DrawData::clearZBuffer() {
  for (int i = 0; i < X; i++) {
    for (int j = 0; j < Y; j++) {
      zBuffer[i][j] = std::numeric_limits<float>::min();
    }
  }
}

void DrawData::setColor(const QColor &color) {
  textureOrColor = QVariant::fromValue(color);
}

void DrawData::setTexture(const QImage &texture) {
  textureOrColor = QVariant::fromValue(texture);
}
