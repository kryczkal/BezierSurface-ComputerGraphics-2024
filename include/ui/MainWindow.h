#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "geometry/BezierSurface.h"
#include "graphics/QGraphicsEngine.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QSlider>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    explicit MainWindow(QWidget *parent = nullptr);

    void resizeEvent(QResizeEvent *event) override;
    ~MainWindow();

    void setupEngine(
        const Settings &settings, QGraphicsScene *scene, QGraphicsEngine *&engine,
        QSharedPointer<BezierSurface> &bezierSurface, QSharedPointer<LightSource> &lightSource,
        QSharedPointer<QImage> &texture
    ) const;

    void setupRotationBox(
        const QWidget *centralWidget, QVBoxLayout *leftToolbarLayout, QSlider *&xRotationSlider,
        QSlider *&yRotationSlider, QSlider *&zRotationSlider
    ) const;

    void setupBezierSurfaceBox(const QWidget *centralWidget, QVBoxLayout *leftToolbarLayout) const;

    void setupMiscBox(
        QWidget *centralWidget, QGraphicsEngine *engine, QSharedPointer<BezierSurface> &bezierSurface,
        QSharedPointer<LightSource> &lightSource, QVBoxLayout *leftToolbarLayout, const QSlider *xRotationSlider,
        const QSlider *yRotationSlider, const QSlider *zRotationSlider
    ) const;

    void setupLightningBox(const QWidget *centralWidget, QVBoxLayout *leftToolbarLayout) const;
};
#endif // MAINWINDOW_H
