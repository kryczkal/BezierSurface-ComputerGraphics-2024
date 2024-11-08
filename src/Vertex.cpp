//
// Created by wookie on 11/7/24.
//

#include "geometry/Vertex.h"

Vertex::Vertex(QVector3D position, QVector3D normal, QVector3D uTangent, QVector3D vTangent) {
    _positionOriginal = position;
    _normalOriginal = normal;
    _uTangentOriginal = uTangent;
    _vTangentOriginal = vTangent;

    _positionTransformed = position;
    _normalTransformed = normal;
    _uTangentTransformed = uTangent;
    _vTangentTransformed = vTangent;
}