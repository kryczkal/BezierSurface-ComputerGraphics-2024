#include "ui/MainWindow.h"
#include "geometry/BezierSurface.h"
#include "graphics/QGraphicsEngine.h"
#include <QCheckBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGroupBox>
#include <QLabel>
#include <QSlider>
#include <QSpacerItem>
#include <QSplitter>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    Settings &settings = Settings::getInstance();

    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    // Create the left toolbar with a fixed width
    QWidget *leftToolbar = new QWidget();
    leftToolbar->setFixedWidth(350);

    QVBoxLayout *leftToolbarLayout = new QVBoxLayout(leftToolbar);
    leftToolbarLayout->setSpacing(10);

    // Create a group box to contain the sliders and labels
    QGroupBox *rotationBox      = new QGroupBox();
    QVBoxLayout *rotationLayout = new QVBoxLayout(rotationBox);

    rotationLayout->setSpacing(5);

    // X Rotation Slider
    QLabel *xLabel           = new QLabel("X Rotation");
    QSlider *xRotationSlider = new QSlider(Qt::Horizontal);
    xRotationSlider->setRange(-180, 180);
    xRotationSlider->setValue(0);
    xRotationSlider->setTickInterval(1000);
    xRotationSlider->setTickPosition(QSlider::TicksBelow);
    rotationLayout->addWidget(xLabel);
    rotationLayout->addWidget(xRotationSlider);
    connect(
        xRotationSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->setRotationX(value);
            }
        }
    );

    // Y Rotation Slider
    QLabel *yLabel           = new QLabel("Y Rotation");
    QSlider *yRotationSlider = new QSlider(Qt::Horizontal);
    yRotationSlider->setRange(-180, 180);
    yRotationSlider->setValue(0);
    yRotationSlider->setTickInterval(1000);
    yRotationSlider->setTickPosition(QSlider::TicksBelow);
    rotationLayout->addWidget(yLabel);
    rotationLayout->addWidget(yRotationSlider);
    connect(
        yRotationSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->setRotationY(value);
            }
        }
    );

    // Z Rotation Slider
    QLabel *zLabel           = new QLabel("Z Rotation");
    QSlider *zRotationSlider = new QSlider(Qt::Horizontal);
    zRotationSlider->setRange(-180, 180);
    zRotationSlider->setValue(0);
    zRotationSlider->setTickInterval(1000);
    zRotationSlider->setTickPosition(QSlider::TicksBelow);

    rotationLayout->addWidget(zLabel);
    rotationLayout->addWidget(zRotationSlider);
    connect(
        zRotationSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->setRotationZ(value);
            }
        }
    );

    QGroupBox *lightSettingsBox      = new QGroupBox();
    QVBoxLayout *lightSettingsLayout = new QVBoxLayout(lightSettingsBox);
    lightSettingsLayout->setSpacing(5);

    QLabel *kdLabel   = new QLabel("Kd Coef");
    QSlider *kdSlider = new QSlider(Qt::Horizontal);
    kdSlider->setRange(0, 100);
    kdSlider->setValue(50);
    kdSlider->setTickInterval(10);
    kdSlider->setTickPosition(QSlider::TicksBelow);
    lightSettingsLayout->addWidget(kdLabel);
    lightSettingsLayout->addWidget(kdSlider);
    connect(
        kdSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            Settings &settings            = Settings::getInstance();
            settings.lightSettings.kdCoef = value / 100.0f;

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );

    QLabel *ksLabel   = new QLabel("Ks Coef");
    QSlider *ksSlider = new QSlider(Qt::Horizontal);
    ksSlider->setRange(0, 100);
    ksSlider->setValue(50);
    ksSlider->setTickInterval(10);
    ksSlider->setTickPosition(QSlider::TicksBelow);
    lightSettingsLayout->addWidget(ksLabel);
    lightSettingsLayout->addWidget(ksSlider);
    connect(
        ksSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            Settings &settings            = Settings::getInstance();
            settings.lightSettings.ksCoef = value / 100.0f;

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );

    QLabel *mLabel   = new QLabel("M");
    QSlider *mSlider = new QSlider(Qt::Horizontal);
    mSlider->setRange(0, 40);
    mSlider->setValue(8);
    mSlider->setTickInterval(1);
    mSlider->setTickPosition(QSlider::TicksBelow);
    lightSettingsLayout->addWidget(mLabel);
    lightSettingsLayout->addWidget(mSlider);
    connect(
        mSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            Settings &settings       = Settings::getInstance();
            settings.lightSettings.m = value;

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );

    leftToolbarLayout->addWidget(lightSettingsBox);

    // Add a vertical spacer to push items to the top
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // Add the rotation box to the left toolbar layout
    leftToolbarLayout->addWidget(rotationBox);

    QGroupBox *bezierSurfaceBox      = new QGroupBox();
    QVBoxLayout *bezierSurfaceLayout = new QVBoxLayout(bezierSurfaceBox);
    bezierSurfaceLayout->setSpacing(5);

    QCheckBox *bezierSurfaceCheckbox = new QCheckBox("Draw Triangles - Debug");
    bezierSurfaceCheckbox->setChecked(settings.triangleSettings.debugDraw);
    bezierSurfaceLayout->addWidget(bezierSurfaceCheckbox);
    connect(
        bezierSurfaceCheckbox, &QCheckBox::stateChanged,
        [centralWidget](int state)
        {
            Settings &settings                  = Settings::getInstance();
            settings.triangleSettings.debugDraw = state == Qt::Checked;

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );
    QCheckBox *bezierSurfaceCheckbox2 = new QCheckBox("Draw Normals");
    bezierSurfaceCheckbox2->setChecked(settings.vertexSettings.drawNormals);
    bezierSurfaceLayout->addWidget(bezierSurfaceCheckbox2);
    connect(
        bezierSurfaceCheckbox2, &QCheckBox::stateChanged,
        [centralWidget](int state)
        {
            Settings &settings                  = Settings::getInstance();
            settings.vertexSettings.drawNormals = state == Qt::Checked;

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );
    QCheckBox *bezierSurfaceCheckbox3 = new QCheckBox("Draw Tangents");
    bezierSurfaceCheckbox3->setChecked(settings.vertexSettings.drawTangents);
    bezierSurfaceLayout->addWidget(bezierSurfaceCheckbox3);
    connect(
        bezierSurfaceCheckbox3, &QCheckBox::stateChanged,
        [centralWidget](int state)
        {
            Settings &settings                   = Settings::getInstance();
            settings.vertexSettings.drawTangents = state == Qt::Checked;

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );

    leftToolbarLayout->addWidget(bezierSurfaceBox);

    // Graphics Scene and View Setup
    QGraphicsScene *scene           = new QGraphicsScene();
    QGraphicsView *renderEngineView = new QGraphicsView(scene);

    QGraphicsEngine *engine =
        new QGraphicsEngine(settings.graphicsEngineSettings.sizeX, settings.graphicsEngineSettings.sizeY);
    scene->addItem(engine);

    QSharedPointer<QGraphicsEngineDrawable> drawable =
        QSharedPointer<QGraphicsEngineDrawable>(new BezierSurface("crazy.txt"));
    QSharedPointer<LightSource> lightSource = QSharedPointer<LightSource>(new LightSource(QVector3D(0, 0, 0)));
    engine->addDrawable(drawable);
    engine->addLightSource(lightSource);
    engine->draw();

    leftToolbarLayout->setAlignment(Qt::AlignTop);
    leftToolbarLayout->addSpacerItem(verticalSpacer);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    mainSplitter->addWidget(leftToolbar);
    mainSplitter->addWidget(renderEngineView);
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 3);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(mainSplitter);

    renderEngineView->fitInView(engine->boundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QGraphicsView *mainView = findChild<QGraphicsView *>();
    if (mainView)
    {
        mainView->fitInView(mainView->scene()->sceneRect(), Qt::KeepAspectRatio);
    }
}

MainWindow::~MainWindow() {}
