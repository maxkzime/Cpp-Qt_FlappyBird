#include "pillaritem.h"

#include "birditem.h"
#include "scene.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QGraphicsScene>

PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap(":/imgs/toppipe.png").scaled(QSize(42, 600)))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap(":/imgs/bottompipe.png").scaled(QSize(42, 600)))),
    pastByTheBird(false)
{

    topPillar->setPos(QPointF(0,0) -QPointF(topPillar->boundingRect().width()/2,
                                            topPillar->boundingRect().height() +60));

    bottomPillar->setPos(QPointF(0,0) + QPointF(-bottomPillar->boundingRect().width()/2,60));

    addToGroup(topPillar);
    addToGroup(bottomPillar);

    yPos = QRandomGenerator::global()->bounded(150);
    int xRandomizer = QRandomGenerator::global()->bounded(200);

    setPos(QPointF(0,0) + QPointF(260 + xRandomizer, yPos));

    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(-250);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(QRandomGenerator::global()->bounded(1900,2500));

    connect(xAnimation, &QPropertyAnimation::finished,[=](){
        qDebug() << "Animation finished";
        scene()->removeItem(this);
        delete this;
    });

    xAnimation->start();

}


PillarItem::~PillarItem()
{
    qDebug() << "Deleting Pillars ";
    delete topPillar;
    delete bottomPillar;
}


qreal PillarItem::x() const
{return m_x;}


void PillarItem::setX(qreal newX)
{
    //    qDebug() << "Pillar position : " << newX << ';' << yPos ;
    m_x = newX;

    if( newX < 0 && !pastByTheBird){
        pastByTheBird = true;
        QGraphicsScene * menuScene = scene();
        Scene * myScene = dynamic_cast<Scene *>(menuScene);
        if(myScene){
            myScene->incrementScore();
        }
    }


    if(collideWithBird()){
        emit collideFail();
    }
    setPos(QPointF(0,0) + QPointF(newX, yPos ));
}

void PillarItem::stopPillar()
{
    xAnimation->stop();
}

bool PillarItem::collideWithBird()
{
    bool collideValidation = false;
    QList<QGraphicsItem*> collidingItems = topPillar->collidingItems();
    collidingItems.append(bottomPillar->collidingItems());

    foreach (QGraphicsItem * item, collidingItems) {
        BirdItem * birdItem = dynamic_cast<BirdItem*>(item);
        if(birdItem){
            collideValidation = true;
        }
    }

    return collideValidation;
}
