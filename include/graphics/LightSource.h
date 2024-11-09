//
// Created by wookie on 11/9/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSOURCE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSOURCE_H

#include "QGraphicsEngineDrawable.h"
#include <QVector3D>
#include <cmath>

class LightSource : public QGraphicsEngineDrawable
{
    public:
    // Constructors
    LightSource(QVector3D position = QVector3D(0, 0, 0));

    // Getters and setters
    QVector3D &getPosition() { return position; }
    void setPosition(QVector3D position) { this->position = position; }

    QColor getColor() const { return color; }
    void setColor(QColor color) { this->color = color; }

    // Public methods
    QVector3D calcVersorTo(QVector3D point) const { return (point - position).normalized(); }

    // QGraphicsEngineDrawable interface
    void draw(DrawData &drawData) override;
    void transform(QMatrix4x4 &matrix) override;

    private:
    QVector3D position;
    QColor color = QColor(155, 155, 0);
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSOURCE_H
