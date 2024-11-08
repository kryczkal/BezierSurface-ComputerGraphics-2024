//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_MESH_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_MESH_H

#include "Triangle.h"
#include "graphics/QGraphicsEngineDrawable.h"
#include <QVector>

class Mesh : public QGraphicsEngineDrawable {
public:
  // Constructors
  Mesh() = default;
  [[maybe_unused]] explicit Mesh(const QVector<Triangle> &vertices);
  [[maybe_unused]] explicit Mesh(QVector<Triangle> &&vertices);

  // Getters
  [[maybe_unused]] [[nodiscard]] QVector<Triangle> getTriangles() const;

  // Tessellation
  [[maybe_unused]] static Mesh create2dTessellation(int tessellationLevel);
  static QVector<Triangle> create2dTessellationTriangles(int tessellationLevel);

  // Drawing
  void draw(DrawData &drawData) override;

protected:
  QVector<Triangle> _triangles;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_MESH_H
