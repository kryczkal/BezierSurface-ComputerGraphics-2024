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
    LightSource(QColor color = QColor(255, 255, 255)) : color(color) {};

    // Getters and setters
    QVector3D &getPosition() { return position; }
    void setPosition(QVector3D position) { this->position = position; }
    QVector3D &getDirection() { return direction; }
    void setDirection(QVector3D direction) { this->direction = direction; }

    QColor getColor() const { return color; }
    void setColor(QColor color) { this->color = color; }

    // Public methods
    QVector3D calcVersorTo(QVector3D point) const { return (point - position).normalized(); }

    // QGraphicsEngineDrawable interface
    void draw(DrawData &drawData) override;
    void transform(QMatrix4x4 &matrix) override;

    private:
    QVector3D position;
    QVector3D direction;
    QColor color;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSOURCE_H
