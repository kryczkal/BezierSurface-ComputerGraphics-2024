//
// Created by wookie on 11/8/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_SETTINGS_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_SETTINGS_H

#include "MeshSettings.h"
#include "TriangleSettings.h"
#include <QColor>

class Settings
{
    public:
    static Settings &getInstance()
    {
        static Settings instance;
        return instance;
    }

    QColor backgroundColor = Qt::white;
    TriangleSettings triangleSettings;
    MeshSettings meshSettings;

    private:
    Settings()                       = default;
    Settings(Settings const &)       = delete;
    void operator=(Settings const &) = delete;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_SETTINGS_H
