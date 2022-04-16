#include "floor.h"

#include "scene.h"
#include <QDebug>
#include <QGraphicsScene>

Floor::Floor(QPixmap pixmap)
{
    setPixmap(pixmap);

    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(-250);
    xAnimation->setEndValue(-1000);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(15000);

    connect(xAnimation, &QPropertyAnimation::finished,[=](){
        qDebug() << "Animation finished";
        xAnimation->start();
    });

    xAnimation->start();
}


qreal Floor::x() const
{return m_x;}


void Floor::setX(qreal newX)
{
    qDebug() << "floor position : " << newX;
    m_x = newX;

    setPos(QPointF(x(),y()) + QPointF(newX,0));
}

void Floor::stopFloor()
{xAnimation->stop();}
