//
// Created by wookie on 11/8/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLESETTINGS_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLESETTINGS_H

#include <QColor>

class TriangleSettings {
public:
  bool debugDraw = true;
  QColor triangleEdgeColor = QColor(0, 0, 0, 255);
  QColor triangleVertexColor = QColor(255, 0, 0, 255);
  QColor triangleFillColor = QColor(0, 255, 0, 100);
  float triangleEdgeDrawProximityCoef = 0.02;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_TRIANGLESETTINGS_H
