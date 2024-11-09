//
// Created by wookie on 11/9/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSETTINGS_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSETTINGS_H

#include <QColor>

class LightSettings
{
    public:
    float kdCoef = 1.0f;
    float ksCoef = 1.0f;
    int m        = 8;

    QColor lightSourceObjectColor = QColor(155, 155, 0);
    int lightSourceObjectSize     = 10;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_LIGHTSETTINGS_H
