#include "ui/MainWindow.h"
#include "geometry/BezierSurface.h"
#include "graphics/QGraphicsEngine.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSlider>
#include <QSplitter>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    Settings &settings = Settings::getInstance();

    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    QWidget *leftToolbar = new QWidget();
    leftToolbar->setFixedWidth(350);

    QVBoxLayout *leftToolbarLayout = new QVBoxLayout(leftToolbar);

    QSlider *xRotationSlider = new QSlider(Qt::Horizontal);
    xRotationSlider->setRange(-180, 180);
    xRotationSlider->setValue(0);
    xRotationSlider->setTickInterval(1000);
    xRotationSlider->setTickPosition(QSlider::TicksBelow);
    leftToolbarLayout->addWidget(xRotationSlider);
    connect(
        xRotationSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
            {
                qDebug() << "X rotation changed to " << value;
                engine->setRotationX(value);
            }
        }
    );
    QSlider *yRotationSlider = new QSlider(Qt::Horizontal);
    yRotationSlider->setRange(-180, 180);
    yRotationSlider->setValue(0);
    yRotationSlider->setTickInterval(1000);
    yRotationSlider->setTickPosition(QSlider::TicksBelow);
    leftToolbarLayout->addWidget(yRotationSlider);
    connect(
        yRotationSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
                engine->setRotationY(value);
        }
    );
    QSlider *zRotationSlider = new QSlider(Qt::Horizontal);
    zRotationSlider->setRange(-180, 180);
    zRotationSlider->setValue(0);
    zRotationSlider->setTickInterval(1000);
    zRotationSlider->setTickPosition(QSlider::TicksBelow);
    connect(
        zRotationSlider, &QSlider::valueChanged,
        [centralWidget](int value)
        {
            QGraphicsView *mainView = centralWidget->findChild<QGraphicsView *>();
            QGraphicsEngine *engine = dynamic_cast<QGraphicsEngine *>(mainView->scene()->items().first());
            if (engine)
                engine->setRotationZ(value);
        }
    );
    leftToolbarLayout->addWidget(zRotationSlider);

    QGraphicsScene *scene           = new QGraphicsScene();
    QGraphicsView *renderEngineView = new QGraphicsView(scene);

    QGraphicsEngine *engine =
        new QGraphicsEngine(settings.graphicsEngineSettings.sizeX, settings.graphicsEngineSettings.sizeY);
    scene->addItem(engine);
    //  engine->testPixmap();

    QSharedPointer<QGraphicsEngineDrawable> drawable =
        QSharedPointer<QGraphicsEngineDrawable>(new BezierSurface("controlPointsWavy.txt"));
    engine->addDrawable(drawable);
    engine->draw();

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
