#include "ui/MainWindow.h"
#include "geometry/BezierSurface.h"
#include "graphics/QGraphicsEngine.h"
#include <QCheckBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
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

    // Graphics Scene and View Setup
    QGraphicsScene *scene           = new QGraphicsScene();
    QGraphicsView *renderEngineView = new QGraphicsView(scene);

    QGraphicsEngine *engine =
        new QGraphicsEngine(settings.graphicsEngineSettings.sizeX, settings.graphicsEngineSettings.sizeY);
    scene->addItem(engine);

    QSharedPointer<BezierSurface> bezierSurface      = QSharedPointer<BezierSurface>(new BezierSurface("crazy.txt"));
    QSharedPointer<QGraphicsEngineDrawable> drawable = QSharedPointer<QGraphicsEngineDrawable>(bezierSurface);

    QSharedPointer<LightSource> lightSource = QSharedPointer<LightSource>(new LightSource(QVector3D(0, 0, 3)));
    engine->addDrawable(drawable);
    engine->addLightSource(lightSource);
    engine->draw();

    QSharedPointer<QImage> texture   = QSharedPointer<QImage>(new QImage("testTexture1.png"));
    QSharedPointer<QImage> normalMap = nullptr;

    bezierSurface->setTexture(texture);
    bezierSurface->setNormalMap(normalMap);

    // Validate if the texture was loaded correctly
    if (texture->isNull())
    {
        qDebug() << "Texture not loaded correctly";
    }

    // Create the left toolbar with a fixed width
    QWidget *leftToolbar = new QWidget();
    leftToolbar->setFixedWidth(350);

    QVBoxLayout *leftToolbarLayout = new QVBoxLayout(leftToolbar);
    leftToolbarLayout->setSpacing(10);

    QGroupBox *tesselationBox      = new QGroupBox();
    QVBoxLayout *tesselationLayout = new QVBoxLayout(tesselationBox);
    tesselationLayout->setSpacing(5);

    QLabel *tesselationLabel   = new QLabel("Tesselation Level");
    QSlider *tesselationSlider = new QSlider(Qt::Horizontal);
    tesselationSlider->setRange(1, 100);
    tesselationSlider->setValue(10);
    tesselationSlider->setTickInterval(1);
    tesselationSlider->setTickPosition(QSlider::TicksBelow);
    tesselationLayout->addWidget(tesselationLabel);
    tesselationLayout->addWidget(tesselationSlider);
    connect(
        tesselationSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            Settings &settings = Settings::getInstance();

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
            }
        }
    );

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
    // Add the rotation box to the left toolbar layout
    leftToolbarLayout->addWidget(rotationBox);

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

    QGroupBox *bezierSurfaceBox      = new QGroupBox();
    QVBoxLayout *bezierSurfaceLayout = new QVBoxLayout(bezierSurfaceBox);
    bezierSurfaceLayout->setSpacing(5);

    QCheckBox *bezierSurfaceCheckbox = new QCheckBox("Draw Triangles - (Enable Debug Mode)");
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
    QCheckBox *bezierSurfaceCheckbox2 = new QCheckBox("Draw Normals - (In Debug Mode)");
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
    QCheckBox *bezierSurfaceCheckbox3 = new QCheckBox("Draw Tangents - (In Debug Mode)");
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

    QCheckBox *bezierSurfaceCheckbox4 = new QCheckBox("Draw Control Points");
    bezierSurfaceCheckbox4->setChecked(settings.bezierSurfaceSettings.showControlPoints);
    bezierSurfaceLayout->addWidget(bezierSurfaceCheckbox4);
    connect(
        bezierSurfaceCheckbox4, &QCheckBox::stateChanged,
        [centralWidget](int state)
        {
            Settings &settings                               = Settings::getInstance();
            settings.bezierSurfaceSettings.showControlPoints = state == Qt::Checked;

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );

    QCheckBox *bezierSurfaceCheckbox5 = new QCheckBox("Enable Lighting");
    bezierSurfaceCheckbox5->setChecked(settings.lightSettings.isLightSourceEnabled);
    bezierSurfaceLayout->addWidget(bezierSurfaceCheckbox5);
    connect(
        bezierSurfaceCheckbox5, &QCheckBox::stateChanged,
        [centralWidget](int state)
        {
            Settings &settings                          = Settings::getInstance();
            settings.lightSettings.isLightSourceEnabled = state == Qt::Checked;

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );

    leftToolbarLayout->addWidget(bezierSurfaceBox);

    // Normal Map and Texture Settings
    QGroupBox *normalMapBox      = new QGroupBox();
    QVBoxLayout *normalMapLayout = new QVBoxLayout(normalMapBox);
    normalMapLayout->setSpacing(5);

    // Set tessellation level
    QLabel *tessellationLabel   = new QLabel("Tessellation Level");
    QSlider *tessellationSlider = new QSlider(Qt::Horizontal);
    tessellationSlider->setRange(1, 100);
    tessellationSlider->setValue(10);
    tessellationSlider->setTickInterval(10);
    tessellationSlider->setTickPosition(QSlider::TicksBelow);
    normalMapLayout->addWidget(tessellationLabel);
    normalMapLayout->addWidget(tessellationSlider);
    connect(
        tessellationSlider, &QSlider::valueChanged,
        [=](int value)
        {
            float rotationX, rotationY, rotationZ;
            rotationX = xRotationSlider->value();
            rotationY = yRotationSlider->value();
            rotationZ = zRotationSlider->value();
            bezierSurface->setTessellationLevel(value);
            engine->setRotation(rotationX, rotationY, rotationZ);

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );

    // Pick Normal Map Button
    QPushButton *pickNormalMapButton = new QPushButton("Pick Normal Map");
    normalMapLayout->addWidget(pickNormalMapButton);
    connect(
        pickNormalMapButton, &QPushButton::clicked,
        [=](bool)
        {
            QString path = QFileDialog::getOpenFileName(
                centralWidget, "Open Normal Map", QDir::homePath(), "Images (*.png *.jpg)"
            );
            if (!path.isEmpty())
            {
                Settings &settings = Settings::getInstance();
                bezierSurface->setNormalMap(QSharedPointer<QImage>(new QImage(path)));

                QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
                QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
                if (engine)
                {
                    engine->draw();
                }
            }
        }
    );
    // Pick Texture Button
    QPushButton *pickTextureButton = new QPushButton("Pick Texture");
    normalMapLayout->addWidget(pickTextureButton);
    connect(
        pickTextureButton, &QPushButton::clicked,
        [=](bool)
        {
            QString path =
                QFileDialog::getOpenFileName(centralWidget, "Open Texture", QDir::homePath(), "Images (*.png *.jpg)");
            if (!path.isEmpty())
            {
                Settings &settings = Settings::getInstance();
                bezierSurface->setTexture(QSharedPointer<QImage>(new QImage(path)));

                QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
                QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
                if (engine)
                {
                    engine->draw();
                }
            }
        }
    );

    // Clear Texture Button
    QPushButton *clearTextureButton = new QPushButton("Clear Texture");
    normalMapLayout->addWidget(clearTextureButton);
    connect(
        clearTextureButton, &QPushButton::clicked,
        [=](bool)
        {
            Settings &settings = Settings::getInstance();
            bezierSurface->setTexture(nullptr);

            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                engine->draw();
            }
        }
    );

    // Pick Bezier Surface Color
    QPushButton *pickBezierSurfaceColorButton = new QPushButton("Pick Bezier Surface Color");
    normalMapLayout->addWidget(pickBezierSurfaceColorButton);
    connect(
        pickBezierSurfaceColorButton, &QPushButton::clicked,
        [=](bool)
        {
            QColor color = QColorDialog::getColor(Qt::white, centralWidget);
            if (color.isValid())
            {
                Settings &settings                          = Settings::getInstance();
                settings.bezierSurfaceSettings.defaultColor = color;

                QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
                QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
                if (engine)
                {
                    engine->draw();
                }
            }
        }
    );

    // Pick light source color
    QPushButton *pickLightSourceColorButton = new QPushButton("Pick Light Source Color");
    normalMapLayout->addWidget(pickLightSourceColorButton);
    connect(
        pickLightSourceColorButton, &QPushButton::clicked,
        [=](bool)
        {
            QColor color = QColorDialog::getColor(Qt::white, centralWidget);
            if (color.isValid())
            {
                Settings &settings                            = Settings::getInstance();
                settings.lightSettings.lightSourceObjectColor = color;
                lightSource->setColor(color);

                QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
                QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
                if (engine)
                {
                    engine->draw();
                }
            }
        }
    );

    leftToolbarLayout->addWidget(normalMapBox);

    // Add a vertical spacer to push items to the top
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

    QTimer *timer = new QTimer(this);
    connect(
        timer, &QTimer::timeout,
        [engine]()
        {
            engine->autoMoveLightSources();
            engine->draw();
        }
    );
    timer->start(1000 / 60);
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
