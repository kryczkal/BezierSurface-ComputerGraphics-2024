#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "MainWindow.h"
#include "QGraphicsEngine.h"
#include "BezierSurface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(scene);
    setCentralWidget(view);

    QGraphicsEngine *engine = new QGraphicsEngine(800, 600);
    scene->addItem(engine);
    engine->testPixmap();

    BezierSurface *surface = new BezierSurface("controlPoints.txt");

    view->fitInView(engine->boundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    QGraphicsView *view = static_cast<QGraphicsView*>(centralWidget());
    view->fitInView(view->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

MainWindow::~MainWindow() {
}

