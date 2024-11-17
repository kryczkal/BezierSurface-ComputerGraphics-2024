//
// Created by wookie on 11/9/24.
//

#include "graphics/LightSource.h"
#include "models/DrawData.h"
#include "settings/Settings.h"
#include "utils/DrawUtils.h"
#include <QMatrix4x4>

void LightSource::draw(DrawData &drawData)
{
    const float zFactor = 1.0f + position.z();

    Settings &settings = Settings::getInstance();
    DrawUtils::drawPoint(
        drawData, position, Qt::black, settings.lightSettings.lightSourceObjectSize * 2 * zFactor,
        settings.lightSettings.lightSourceObjectSize * 2 * zFactor
    );
    DrawUtils::drawPoint(
        drawData, position, color, settings.lightSettings.lightSourceObjectSize * zFactor,
        settings.lightSettings.lightSourceObjectSize * zFactor
    );
}

void LightSource::transform(QMatrix4x4 &matrix) { position = matrix * position; }
