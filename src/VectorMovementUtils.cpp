//
// Created by wookie on 11/9/24.
//

#include "utils/VectorMovementUtils.h"
#include <QDebug>
#include <QVector3D>
#include <cmath>

void VectorMovementUtils::moveStep(QVector3D &position, QVector3D direction, float step)
{
    position += direction * step;
}

void VectorMovementUtils::moveAcrossCircle(QVector3D &position, QVector3D center, float radius, float angle)
{
    float currentAngle = atan2(position.y() - center.y(), position.x() - center.x());
    angle += currentAngle;
    position.setX(center.x() + radius * cos(angle));
    position.setY(center.y() + radius * sin(angle));
    position.setZ(center.z());
}