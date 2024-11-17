//
// Created by wookie on 11/9/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSETTINGS_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSETTINGS_H

#include <QColor>
#include <QVector3D>

class LightSettings
{
    public:
    float kdCoef = 1.0f;
    float ksCoef = 1.0f;
    int m        = 8;

    float orbitRadius     = 0.3f;
    float orbitSpeed      = 0.01f;
    QVector3D orbitCenter = QVector3D(0.5, 0.5, 0.5);

    QColor lightSourceObjectColor = QColor(155, 155, 0);
    int lightSourceObjectSize     = 5;

    bool isLightSourceEnabled = true;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSETTINGS_H
