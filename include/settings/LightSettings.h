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
    // Basic Coefficients
    float kdCoef = 1.0f;
    float ksCoef = 1.0f;
    int m        = 8;

    // Orbit Settings
    float orbitRadius         = 0.3f;
    float orbitSpeed          = 0.01f;
    float baseOrbitRadius     = 0.3f;
    float sineCoeff           = 0.1f;
    QVector3D orbitCenter     = QVector3D(0.5, 0.5, 0.5);
    QVector3D centerToPointAt = QVector3D(0.5, 0.5, 0);

    // Reflector Settings
    int mCoeffReflector = 8;

    // Paint Settings
    int lightSourceObjectSize = 2;

    // Debug Settings
    bool isLightSourceEnabled = true;
    bool isReflectorEnabled   = true;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSETTINGS_H
