//
// Created by wookie on 11/7/24.
//

#include <QDebug>
#include "geometry/Triangle.h"
#include "geometry/Vertex.h"

[[maybe_unused]] Triangle::Triangle(Vertex a, Vertex b, Vertex c)
    : _a(a), _b(b), _c(c) {}

[[maybe_unused]] Vertex Triangle::getA() const { return _a; }

[[maybe_unused]] Vertex Triangle::getB() const { return _b; }

[[maybe_unused]] Vertex Triangle::getC() const { return _c; }

Vertex Triangle::operator[](int i) const {
    switch(i) {
        case 0: return _a;
        case 1: return _b;
        case 2: return _c;
        default: {
            qDebug() << "Triangle::operator[]: index out of bounds";
            return {0,0,0};
        }
    }
}

void Triangle::draw(const QImage &data, const QImage &texture) {

}

