//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINE_H

#include <QGraphicsItem>
#include <QMutex>

class QGraphicsEngine : public QGraphicsItem {
public:
    // Constructors
    QGraphicsEngine(int width, int height);

    // Getters
    QRectF boundingRect() const override;
    int getWidth() const;
    int getHeight() const;
    QImage getQImage() const;

    // Drawing
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    // Test
    void testPixmap();

private:
    int _width;
    int _height;
    QImage _qImage;
    QMutex _drawMutex;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINE_H
