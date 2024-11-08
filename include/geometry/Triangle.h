//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLE_H

#include "Vertex.h"
#include "graphics/QGraphicsEngineDrawable.h"
#include "utils/Settings.h"

class DrawData;

class Triangle : public QGraphicsEngineDrawable
{
    public:
    // Constructors
    [[maybe_unused]] Triangle(Vertex a, Vertex b, Vertex c);

    // Getters
    [[maybe_unused]] [[nodiscard]] Vertex getA() const;

    [[maybe_unused]] [[nodiscard]] Vertex getB() const;

    [[maybe_unused]] [[nodiscard]] Vertex getC() const;

    // Public Methods
    void draw(DrawData &drawData) override;
    void transform(QMatrix4x4 &matrix) override;

    // Operators
    Vertex &operator[](int i);
    auto begin() { return std::array<Vertex *, 3>{&_a, &_b, &_c}.begin(); }
    auto end() { return std::array<Vertex *, 3>{&_a, &_b, &_c}.end(); }

    private:
    Vertex _a;
    Vertex _b;
    Vertex _c;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLE_H
