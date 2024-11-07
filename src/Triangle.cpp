//
// Created by wookie on 11/7/24.
//

#include "geometry/Triangle.h"
#include "geometry/Vertex.h"

[[maybe_unused]] Triangle::Triangle(Vertex a, Vertex b, Vertex c)
    : _a(a), _b(b), _c(c) {}

Vertex Triangle::getA() const { return _a; }

Vertex Triangle::getB() const { return _b; }

Vertex Triangle::getC() const { return _c; }
