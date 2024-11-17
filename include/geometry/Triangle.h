//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLE_H

#include "Vertex.h"
#include "graphics/QGraphicsEngineDrawable.h"
#include "settings/Settings.h"

class DrawData;

struct VertexStruct
{
    float x, y, z;
    QVector3D pos;
    QVector3D normal;
    float u, v;
    QVector3D uTangent;
    QVector3D vTangent;
};

struct EdgeStruct
{
    int yMax;
    float x;
    float xStep;
};

class Triangle : public QGraphicsEngineDrawable
{
    public:
    // Constructors
    [[maybe_unused]] Triangle(Vertex a, Vertex b, Vertex c);

    // Getters
    [[maybe_unused]] [[nodiscard]] Vertex &getA() { return _a; }
    [[maybe_unused]] [[nodiscard]] Vertex &getB() { return _b; }
    [[maybe_unused]] [[nodiscard]] Vertex &getC() { return _c; }
    [[maybe_unused]] [[nodiscard]] Vertex getA() const { return _a; }
    [[maybe_unused]] [[nodiscard]] Vertex getB() const { return _b; }
    [[maybe_unused]] [[nodiscard]] Vertex getC() const { return _c; }

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

    static QVector3D
    computeBarycentricCoordinates(const QVector2D &p, const QVector2D &a, const QVector2D &b, const QVector2D &c);

    static void drawPixelDebug(
        DrawData &drawData, Settings &settings, int y, int x, const QVector3D &barycentric, const QVector3D &pos,
        const QVector3D &normal
    );

    static QColor &getColor(const DrawData &drawData, float u, float v, QColor &color);

    static void getNormalFromMap(
        const DrawData &drawData, const std::array<VertexStruct, 3> &vertices, float u, float v, QVector3D &normal,
        const QVector3D &uTangent, const QVector3D &vTangent
    );
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLE_H
