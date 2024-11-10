//
// Created by wookie on 11/9/24.
//

#include "graphics/LightSource.h"
#include "models/DrawData.h"
#include "settings/Settings.h"
#include "utils/DrawUtils.h"
#include <QMatrix4x4>

LightSource::LightSource(QVector3D position) : position(position) {}

void LightSource::draw(DrawData &drawData)
{
    Settings &settings = Settings::getInstance();
    DrawUtils::drawPoint(
        drawData, position, settings.lightSettings.lightSourceObjectColor, settings.lightSettings.lightSourceObjectSize,
        settings.lightSettings.lightSourceObjectSize
    );
}

void LightSource::transform(QMatrix4x4 &matrix) { position = matrix * position; }
