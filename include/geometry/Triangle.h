//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLE_H

#include "Vertex.h"

class Triangle {
public:
  [[maybe_unused]] Triangle(Vertex a, Vertex b, Vertex c);
  [[nodiscard]] Vertex getA() const;
  [[nodiscard]] Vertex getB() const;
  [[nodiscard]] Vertex getC() const;

private:
  Vertex _a;
  Vertex _b;
  Vertex _c;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLE_H
