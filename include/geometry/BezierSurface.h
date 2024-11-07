//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_BEZIERSURFACE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_BEZIERSURFACE_H

#include <QVector3D>
#include <QVector>

class BezierSurface {
public:
  explicit BezierSurface(const QString &filename);

private:
  QVector<QVector3D> _controlPoints;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_BEZIERSURFACE_H
