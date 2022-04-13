#include "birditem.h"
#include <QTimer>
#include <QRandomGenerator>

BirdItem::BirdItem(QPixmap pixmap)
    :
      wingPos(UP),
      wingDirection(0)
{
    setPixmap(pixmap);

    QTimer * birdWingsTimer = new QTimer(this);
    connect(birdWingsTimer, &QTimer::timeout, [=](){
        updatePixmap();
    });
    birdWingsTimer->start(80);

    groundPosition = scenePos().y() + 270; // adjustable value

    yAnimation = new QPropertyAnimation(this,"y",this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(groundPosition);
    yAnimation->setEasingCurve(QEasingCurve::BezierSpline);
    yAnimation->setDuration(1000);

    connect(yAnimation,&QPropertyAnimation::finished, [=](){
        fallBackToGround();
    });



    rotationAnimation = new QPropertyAnimation(this, "rotation", this);

}

void BirdItem::updatePixmap()
{
    if(wingPos == MIDDLE)
    {
        if(wingDirection){
            // Up
            setPixmap(QPixmap(":/imgs/flappyup.png").scaled(QSize(40, 40)));
            wingPos = UP;
            wingDirection = 0;
        }else{
            // Down
            setPixmap(QPixmap(":/imgs/flappydown.png").scaled(QSize(40, 40)));
            wingPos = DOWN;
            wingDirection = 1;
        }

    }else{
        setPixmap(QPixmap(":/imgs/flappymiddle.png").scaled(QSize(40, 40)));
        wingPos = MIDDLE;
    }
}


qreal BirdItem::rotation() const
{return m_rotation;}

void BirdItem::setRotation(qreal newRotation)
{
    m_rotation = newRotation;

    QPointF c = boundingRect().center();
    QTransform t;
    t.translate(c.x(),c.y());       // translation
    t.rotate(newRotation);          // rotation
    t.translate(-c.x(),-c.y());     // translation inverse
    setTransform(t);                // transformation sur le bird



}

qreal BirdItem::y() const
{return m_y;}

void BirdItem::shootUp()
{
    yAnimation->stop();
    rotationAnimation->stop();

    qreal curPosY = y();
    yAnimation->setStartValue(curPosY);
    yAnimation->setEndValue(curPosY - scene()->sceneRect().height()/QRandomGenerator::global()->bounded(7,9)); // adjustable
    yAnimation->setEasingCurve(QEasingCurve::Linear);
    yAnimation->setDuration(200);
    yAnimation->start();

    rotateTo(-20,200,QEasingCurve::BezierSpline);
}

void BirdItem::startFlying()
{
    yAnimation->start();
    rotateTo(90,1200,QEasingCurve::BezierSpline); // 90 deg vers le bas
}

void BirdItem::stopFlying()
{
    yAnimation->stop();
    rotationAnimation->stop();
}

void BirdItem::setY(qreal newY)
{
    moveBy(0,newY - m_y);
    m_y = newY;
}

void BirdItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation->setStartValue(rotation());
    rotationAnimation->setEndValue(end);
    rotationAnimation->setEasingCurve(curve);
    rotationAnimation->setDuration(duration);
    rotationAnimation->start();
}

void BirdItem::fallBackToGround()
{
    // called when yAnimation->stop();
    if( y() < groundPosition)
    {
        rotationAnimation->stop();


        yAnimation->setStartValue(y());
        yAnimation->setEndValue(groundPosition);
        yAnimation->setEasingCurve(QEasingCurve::Linear);
        yAnimation->setDuration(1000);
        yAnimation->start();

        rotateTo(90,1100,QEasingCurve::BezierSpline); // 90 deg vers le bas
    }
}
