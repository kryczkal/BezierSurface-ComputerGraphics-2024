//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINE_H
#include <QGraphicsItem>
#include <QMutex>

class QGraphicsEngine : public QGraphicsItem {
public:
    QGraphicsEngine(int width, int height);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void testPixmap();
private:
    int _width;
    int _height;
    QImage _qImage;
    QMutex _drawMutex;
};


#endif //BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINE_H
