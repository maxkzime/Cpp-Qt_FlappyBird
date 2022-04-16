#ifndef BIRDITEM_H
#define BIRDITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QGraphicsScene>
#include "floor.h"

class BirdItem : public QObject ,  public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal y READ y WRITE setY)

public:
    explicit BirdItem(QPixmap pixmap);
    qreal rotation() const;
    qreal y() const;

    void moveUp();
    void startFlying();
    void stopFlying();

public slots:
    void setRotation(qreal newRotation);

    void setY(qreal newY);

    void rotateTo(const qreal &end, const int& duration, const QEasingCurve& curve);

    void fallBackToGround();

signals:
    void collidingFloor();

private:
    enum WingPosition{UP,MIDDLE,DOWN};
    void updatePixmap();
    WingPosition wingPos;
    bool wingDirection; // 0: down, 1: up


    qreal m_rotation;
    qreal m_y;
    qreal groundPosition;

    QPropertyAnimation * rotationAnimation;
    QPropertyAnimation * yAnimation;


    bool collideWithFloor();
    void detectCollide();
};

#endif // BIRDITEM_H
