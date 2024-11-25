//
// Created by wookie on 11/8/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_SETTINGS_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_SETTINGS_H

#include "BezierSurfaceSettings.h"
#include "GraphicsEngineSettings.h"
#include "LightSettings.h"
#include "MeshSettings.h"
#include "TriangleSettings.h"
#include "VertexSettings.h"
#include <QColor>

class Settings
{
    public:
    static Settings &getInstance()
    {
        static Settings instance;
        return instance;
    }

    TriangleSettings triangleSettings;
    MeshSettings meshSettings;
    GraphicsEngineSettings graphicsEngineSettings;
    VertexSettings vertexSettings;
    LightSettings lightSettings;
    BezierSurfaceSettings bezierSurfaceSettings;

    private:
    Settings()                       = default;
    Settings(Settings const &)       = delete;
    void operator=(Settings const &) = delete;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_SETTINGS_H
