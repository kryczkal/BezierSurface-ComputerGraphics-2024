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

    // Add a vertical spacer to push items to the top
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // Add the rotation box to the left toolbar layout
    leftToolbarLayout->addWidget(rotationBox);

    QGroupBox *bezierSurfaceBox      = new QGroupBox();
    QVBoxLayout *bezierSurfaceLayout = new QVBoxLayout(bezierSurfaceBox);
    bezierSurfaceLayout->setSpacing(5);

    QCheckBox *bezierSurfaceCheckbox = new QCheckBox("Draw Triangles - Debug");
    bezierSurfaceCheckbox->setChecked(true);
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

    leftToolbarLayout->addWidget(bezierSurfaceBox);

    // Graphics Scene and View Setup
    QGraphicsScene *scene           = new QGraphicsScene();
    QGraphicsView *renderEngineView = new QGraphicsView(scene);

    QGraphicsEngine *engine =
        new QGraphicsEngine(settings.graphicsEngineSettings.sizeX, settings.graphicsEngineSettings.sizeY);
    scene->addItem(engine);

    QSharedPointer<QGraphicsEngineDrawable> drawable =
        QSharedPointer<QGraphicsEngineDrawable>(new BezierSurface("crazy.txt"));
    engine->addDrawable(drawable);
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
