//
// Created by wookie on 11/8/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_VERTEXSETTINGS_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_VERTEXSETTINGS_H

#include <QColor>

class VertexSettings
{
    public:
    // Vertices
    float radiusCoef = 0.005f;
    float offset     = 0.05f;

    // Normals
    bool drawNormals   = false;
    float normalLength = 0.1f;
    QColor normalColor = QColor(0, 0, 255);

    // Tangents
    bool drawTangents   = false;
    float tangentLength = 0.1f;
    QColor tangentColor = QColor(0, 125, 125);
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_VERTEXSETTINGS_H
