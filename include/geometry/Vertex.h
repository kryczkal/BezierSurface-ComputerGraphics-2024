//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_VERTEX_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_VERTEX_H

#include "graphics/QGraphicsEngineDrawable.h"
#include <QMatrix3x3>
#include <QVector3D>

class Vertex : public QGraphicsEngineDrawable
{
    public:
    // Constructors
    explicit Vertex(
        QVector3D position, QVector3D normal = QVector3D(0, 0, 0), QVector3D uTangent = QVector3D(0, 0, 0),
        QVector3D vTangent = QVector3D(0, 0, 0)
    );

    // Copy constructor
    Vertex(const Vertex &) = default;

    // Getters
    [[maybe_unused]] [[nodiscard]] QVector3D getPositionOriginal() const { return _positionOriginal; }
    [[maybe_unused]] [[nodiscard]] QVector3D getNormalOriginal() const { return _normalOriginal; }
    [[maybe_unused]] [[nodiscard]] QVector3D getUTangentOriginal() const { return _uTangentOriginal; }
    [[maybe_unused]] [[nodiscard]] QVector3D getVTangentOriginal() const { return _vTangentOriginal; }

    [[maybe_unused]] [[nodiscard]] QVector3D getPositionTransformed() const { return _positionTransformed; }
    [[maybe_unused]] [[nodiscard]] QVector3D getNormalTransformed() const { return _normalTransformed; }
    [[maybe_unused]] [[nodiscard]] QVector3D getUTangentTransformed() const { return _uTangentTransformed; }
    [[maybe_unused]] [[nodiscard]] QVector3D getVTangentTransformed() const { return _vTangentTransformed; }

    [[maybe_unused]] void setPositionOriginal(const QVector3D &vector3D)
    {
        _positionOriginal    = vector3D;
        _positionTransformed = vector3D;
    }
    [[maybe_unused]] void setNormalOriginal(const QVector3D &vector3D)
    {
        _normalOriginal    = vector3D;
        _normalTransformed = vector3D;
    }
    [[maybe_unused]] void setUTangentOriginal(const QVector3D &vector3D)
    {
        _uTangentOriginal    = vector3D;
        _uTangentTransformed = vector3D;
    }
    [[maybe_unused]] void setVTangentOriginal(const QVector3D &vector3D)
    {
        _vTangentOriginal    = vector3D;
        _vTangentTransformed = vector3D;
    }

    [[maybe_unused]] void setPositionTransformed(const QVector3D &vector3D) { _positionTransformed = vector3D; }
    [[maybe_unused]] void setNormalTransformed(const QVector3D &vector3D) { _normalTransformed = vector3D; }
    [[maybe_unused]] void setUTangentTransformed(const QVector3D &vector3D) { _uTangentTransformed = vector3D; }
    [[maybe_unused]] void setVTangentTransformed(const QVector3D &vector3D) { _vTangentTransformed = vector3D; }

    [[maybe_unused]] [[nodiscard]] float getU() const { return _u; }
    [[maybe_unused]] [[nodiscard]] float getV() const { return _v; }

    [[maybe_unused]] void setU(float u) { _u = u; }
    [[maybe_unused]] void setV(float v) { _v = v; }

    // Public Methods
    void draw(DrawData &drawData) override;
    void transform(QMatrix4x4 &matrix, bool absolute, bool preprocessMatrix) override;
    void transform(QMatrix4x4 &matrix, QVector3D center, bool absolute, bool preprocessMatrix) override;

    private:
    QVector3D _positionOriginal;
    QVector3D _normalOriginal;
    QVector3D _uTangentOriginal;
    QVector3D _vTangentOriginal;

    QVector3D _positionTransformed;
    QVector3D _normalTransformed;
    QVector3D _uTangentTransformed;
    QVector3D _vTangentTransformed;

    float _u, _v;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_VERTEX_H
