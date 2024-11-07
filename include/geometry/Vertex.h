//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_VERTEX_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_VERTEX_H

#include <QVector3D>

class Vertex {
public:
  Vertex(qreal x, qreal y, qreal z);
  explicit Vertex(QVector3D position);

  Vertex(const Vertex &) = default;

  [[maybe_unused]] [[nodiscard]] QVector3D getPosition() const;

    void setPosition(QVector3D vector3D);

private:
  QVector3D _position;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_VERTEX_H
