//
// Created by wookie on 11/7/24.
//

#ifndef BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINE_H
#define BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINE_H

#include "QGraphicsEngineDrawable.h"
#include <QGraphicsItem>
#include <QMutex>

class QGraphicsEngine : public QGraphicsItem
{
    public:
    // Constructors
    QGraphicsEngine(int width, int height);

    // Getters
    QRectF boundingRect() const override;
    int getWidth() const;
    int getHeight() const;
    QImage getQImage() const;

    void setRotationX(float rotationX);
    void setRotationY(float rotationY);
    void setRotationZ(float rotationZ);

    // Inheritance Methods
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Public Methods
    /// Drawing
    void clearDrawables();
    void addDrawable(QSharedPointer<QGraphicsEngineDrawable> &drawable);
    void draw();

    // Test Methods
    void testPixmap();

    private:
    // Private Methods
    void rotate(float x, float y, float z);

    // Private Fields
    float _rotationX = 0;
    float _rotationY = 0;
    float _rotationZ = 0;

    int _width;
    int _height;
    QImage _qImage;
    QMutex _drawMutex;
    QVector<QSharedPointer<QGraphicsEngineDrawable>> _drawables;
};

#endif // BEZIERSURFACE_COMPUTERGRAPHICS_2024_QGRAPHICSENGINE_H
