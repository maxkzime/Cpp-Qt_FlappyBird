#include "pillaritem.h"


PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap(":/imgs/toppipe.png").scaled(QSize(42, 600)))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap(":/imgs/bottompipe.png").scaled(QSize(42, 600)))),
    pastTheBird(false)
{
    topPillar->setPos(QPointF(0,0) -QPointF(topPillar->boundingRect().width()/2,
                                            topPillar->boundingRect().height() +60));

    bottomPillar->setPos(QPointF(0,0) + QPointF(-bottomPillar->boundingRect().width()/2,
                                                60));

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
        //qDebug() << "Animation finished";
        scene()->removeItem(this);
        delete this;
    });

    xAnimation->start();
}


/* Deleting Items */
PillarItem::~PillarItem()
{
    //qDebug() << "Deleting Pillars ";
    delete topPillar;
    delete bottomPillar;
}


qreal PillarItem::x() const {return m_x;}


/* Update position for animation
 * Increment score if pillar past the bird
 * Detect collision with ther bird
 */
void PillarItem::setX(qreal newX)
{
    //    qDebug() << "Pillar position : " << newX << ';' << yPos ;
    m_x = newX;

    if( newX < 0 && !pastTheBird){
        pastTheBird = true;
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


// Stop the pillar animation
void PillarItem::stopPillar()
{xAnimation->stop();}


/* True if collision with the bird
 * Detect the collision by listing all the colliding items and casting only the bird
 */
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
