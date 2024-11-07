//
// Created by wookie on 11/7/24.
//

#include "geometry/BezierSurface.h"
#include <QDebug>
#include <QFile>

BezierSurface::BezierSurface(const QString &filename) {
  // Each line of the file contains 3 floats separated by a space (x y z)
  // Each line represents a control point

  qDebug() << "Reading control points from file:" << filename;

  // Open the file
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Could not open file" << filename;
    return;
  }

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList coords = line.split(" ");

    if (coords.size() != 3) {
      qDebug() << "Invalid line:" << line;
      return;
    }

    bool okX, okY, okZ;
    float x = coords[0].toFloat(&okX);
    float y = coords[1].toFloat(&okY);
    float z = coords[2].toFloat(&okZ);

    if (!okX || !okY || !okZ) {
      qDebug() << "Failed to convert line to floats:" << line;
      return;
    }

    _controlPoints.push_back(QVector3D(x, y, z));

    qDebug() << "Read control points:"
             << QString("%1 %2 %3")
                    .arg(x, 0, 'f', 2)
                    .arg(y, 0, 'f', 2)
                    .arg(z, 0, 'f', 2);
  }
  file.close();

  qDebug() << "Read" << _controlPoints.size() << "control points";
}
