//
// Created by wookie on 11/7/24.
//

#include "geometry/Vertex.h"

[[maybe_unused]] Vertex::Vertex(qreal x, qreal y, qreal z) { _position = QVector3D(x, y, z); }

Vertex::Vertex(QVector3D position) { _position = position; }

[[maybe_unused]] QVector3D Vertex::getPosition() const { return _position; }

void Vertex::setPosition(QVector3D newPosition) { _position = newPosition; }
