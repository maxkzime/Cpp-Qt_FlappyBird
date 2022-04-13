#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>


Scene::Scene(QObject *parent)
    : QGraphicsScene(parent),
      gameOn(false),
      currentScore(0),
      bestScore(0)
{
    setUpPillarTimer();
}

void Scene::setUpPillarTimer()
{
    pillarTimer = new QTimer(this);
    connect(pillarTimer, &QTimer::timeout,[=](){
        PillarItem * pillarItem = new PillarItem();
        connect(pillarItem,&PillarItem::collideFail,[=](){
            pillarTimer->stop();
            freezeGame();
            setGameOn(false);
            showGameOverGraphics();
        });
        addItem(pillarItem);
    });
}

void Scene::freezeGame()
{
    bird->stopFlying();
    QList<QGraphicsItem *> sceneItems = items();
    foreach (QGraphicsItem * item, sceneItems) {
        PillarItem * pillar = dynamic_cast<PillarItem*>(item);
        if(pillar){
            pillar->stopPillar();
        }
    }
}


/*
 * Liste de tous les items de la scène,
 * détection si un item est un pillar,
 * si oui, on le retire puis supprime
 */
void Scene::cleanPillars()
{
    QList<QGraphicsItem *> sceneItems = items();
    foreach (QGraphicsItem * item, sceneItems) {
        PillarItem * pillar = dynamic_cast<PillarItem*>(item);
        if(pillar){
            removeItem(pillar);
            delete pillar;
        }
    }
}

bool Scene::getGameOn() const
{
    return gameOn;
}

void Scene::setGameOn(bool newGameOn)
{
    gameOn = newGameOn;
}

void Scene::incrementScore()
{
    currentScore++;
    if(currentScore > bestScore){
        bestScore = currentScore;
    }

    qDebug() << "Score : " << currentScore
             << " Bestscore : " << bestScore;
}

void Scene::setCurrentScore(int newCurrentScore)
{
    currentScore = newCurrentScore;
}

void Scene::showGameOverGraphics()
{
    gameOverPix = new QGraphicsPixmapItem(QPixmap(":/imgs/gameover.png").scaled(288,63));
    addItem(gameOverPix);
    gameOverPix->setPos(QPointF(0,0) - QPointF(gameOverPix->boundingRect().width()/2,
                                               gameOverPix->boundingRect().height()/2));

    // à modifier avec affichage image (faire une division du score)
    scoreTextItem = new QGraphicsTextItem();
    QString htmlString = "<p> Score : " + QString::number(currentScore) + "<p/>"
            + "<p> Best Score :  " + QString::number(bestScore) + "</p>";
    QFont mFont("Consolas", 20, QFont::Bold);

    scoreTextItem->setHtml(htmlString);
    scoreTextItem->setFont(mFont);
    scoreTextItem->setDefaultTextColor(Qt::yellow);
    addItem(scoreTextItem);

    scoreTextItem->setPos(QPointF(0,0) - QPointF(scoreTextItem->boundingRect().width()/2,
                                                 -gameOverPix->boundingRect().height()/2));


}

void Scene::hideGameOverGraphics()
{
    if(gameOverPix != nullptr){
        removeItem(gameOverPix);
        delete gameOverPix;
        gameOverPix = nullptr;
    }
    if(scoreTextItem != nullptr){
        removeItem(scoreTextItem);
        delete scoreTextItem;
        scoreTextItem = nullptr;
    }
}

void Scene::addBird()
{
    bird = new BirdItem(QPixmap(":/imgs/flappyup.png").scaled(QSize(width()/2, height()/2)));
    addItem(bird);
    bird->setPos(-20,-20);
}


void Scene::startGame()
{
    // start bird anim
    bird->startFlying();

    // start pillars anim
    if(!pillarTimer->isActive()){
        cleanPillars();
        pillarTimer->start(1000); // initialise l'apparition des pilliers (rate)
        setGameOn(true);
        setCurrentScore(0);

        //showGameOverGraphics();
        hideGameOverGraphics();

    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space && gameOn){
        bird->shootUp();
    }
    QGraphicsScene::keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && gameOn){
        bird->shootUp();
    }
    QGraphicsScene::mousePressEvent(event);
}
