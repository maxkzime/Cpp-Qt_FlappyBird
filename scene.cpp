#include "scene.h"




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
            showGameoverGraphics();
        });
        addItem(pillarItem);
    });
}

/* Intialize floor */
void Scene::addFloor()
{
    floorItem = new Floor(QPixmap(":/imgs/floorElongated.png").scaled(QSize(2464,112)));
    addItem(floorItem);
    floorItem->setPos(QPointF(-250,0) + QPointF(/*-floorItem->boundingRect().width()/2*/0,240));
}


void Scene::freezeGame()
{
    bird->stopFlying();
    floorItem->stopFloor();
    QList<QGraphicsItem *> sceneItems = items();
    foreach (QGraphicsItem * item, sceneItems) {
        PillarItem * pillar = dynamic_cast<PillarItem*>(item);
        if(pillar){
            pillar->stopPillar();
        }
    }
}


void Scene::cleanFloor()
{

    removeItem(floorItem);
    delete floorItem;

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
{return gameOn;}

void Scene::setGameOn(bool newGameOn)
{gameOn = newGameOn;}


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

/* Show game-over graphics :
 * - Game-over title
 *
 *
 */
void Scene::showGameoverGraphics()
{
    // game-over title
    gameoverPix = new QGraphicsPixmapItem(QPixmap(":/imgs/gameover.png").scaled(288,63));
    addItem(gameoverPix);
    gameoverPix->setPos(QPointF(0,0) - QPointF(gameoverPix->boundingRect().width()/2,
                                               gameoverPix->boundingRect().height()*3));

    gameoverBlock = new QGraphicsPixmapItem(QPixmap(":/imgs/cadre.png").scaled(226,116));
    addItem(gameoverBlock);
    gameoverBlock->setPos(QPointF(0,0) - QPointF(gameoverBlock->boundingRect().width()/2,
                                                 gameoverPix->boundingRect().height()*1.8));


    // à modifier avec affichage image (faire une division du score)
    //    scoreTextItem = new QGraphicsTextItem();
    //    QString htmlString = "<p> Score : " + QString::number(currentScore) + "<p/>"
    //            + "<p> Best Score :  " + QString::number(bestScore) + "</p>";
    //    QFont mFont("Consolas", 20, QFont::Bold);

    //    scoreTextItem->setHtml(htmlString);
    //    scoreTextItem->setFont(mFont);
    //    scoreTextItem->setDefaultTextColor(Qt::yellow);
    //    addItem(scoreTextItem);

    //    scoreTextItem->setPos(QPointF(0,0) - QPointF(scoreTextItem->boundingRect().width()/2,
    //                                                 -gameOverPix->boundingRect().height()/2));

    myMenuButton = new interactiveImage(QPixmap(":/imgs/menu.png").scaled(160,56));
    addItem(myMenuButton);
    myMenuButton->setPos(QPointF(0,0) - QPointF(myMenuButton->boundingRect().width()/2,
                                                gameoverPix->boundingRect().height()-80));

    connect(myMenuButton,&interactiveImage::clickOnBimg,[=](){
        qDebug() << "showing menu !";
        hideGraphics();
        cleanPillars();
    });

    myStartButton = new interactiveImage(QPixmap(":/imgs/start.png").scaled(QSize(160,56)));
    addItem(myStartButton);
    myStartButton->setPos(QPointF(0,0) - QPointF(myStartButton->boundingRect().width()/2,
                                                 gameoverPix->boundingRect().height()-140));

    connect(myStartButton,&interactiveImage::clickOnBimg,[=](){
        hideGraphics();
        startGame();
    });

}

/* Delete game-over and start graphics */
void Scene::hideGraphics()
{
    if(gameoverPix != nullptr){
        removeItem(gameoverPix);
        delete gameoverPix;
        gameoverPix = nullptr;
    }
    if(gameoverBlock != nullptr){
        removeItem(gameoverBlock);
        delete gameoverBlock;
        gameoverBlock = nullptr;
    }
    if(scoreTextItem != nullptr){
        removeItem(scoreTextItem);
        delete scoreTextItem;
        scoreTextItem = nullptr;
    }
    if(myStartButton != nullptr){
        removeItem(myStartButton);
        delete myStartButton;
        myStartButton = nullptr;
    }
    if(myMenuButton != nullptr){
        removeItem(myMenuButton);
        delete myMenuButton;
        myMenuButton = nullptr;
    }
}


/* Add the bird on the center of the scene
 * connect the bird with the floor = colliding detection = end of the game
 */
void Scene::addBird()
{
    bird = new BirdItem(QPixmap(":/imgs/flappyup.png"));
    addItem(bird);
    bird->setPos(-20,-20);

    connect(bird,&BirdItem::collidingFloor,[=](){
        endOfTheRound();
    });
}


/* End of the game :
 * - stop the pillars spawning
 * -
 * - stop game
 * - show game-over  summary with restart and menu buttons
 */
void Scene::endOfTheRound()
{
    pillarTimer->stop();
    freezeGame();
    setGameOn(false);
    showGameoverGraphics();
}


/*
 * Create start menu :
 * Title
 * Start button
 *
 */
void Scene::addStartMenu()
{
    QGraphicsPixmapItem * title = new QGraphicsPixmapItem(QPixmap(":/imgs/title.png").scaled(294,72));
    addItem(title);
    title->setPos(QPointF(0,0) - QPointF(title->boundingRect().width()/2,
                                         (title->boundingRect().height()/2)+100));

    myStartButton = new interactiveImage(QPixmap(":/imgs/start.png").scaled(QSize(160,56)));
    addItem(myStartButton);
    myStartButton->setPos(QPointF(0,0) - QPointF(myStartButton->boundingRect().width()/2,
                                                 myStartButton->boundingRect().height()/2));

    connect(myStartButton,&interactiveImage::clickOnBimg,[=](){
        startGame();
        delete title;
    });
}

/* Starting a game :
 * - Cleaning or adding floor (restarting animation)
 * - Starting bird animation
 * - Starting pillar animation
 * - reset score
 * - hide graphics (menu, game-over, ect..)
 */
void Scene::startGame()
{
    if(floorItem != nullptr){
        cleanFloor();
        addFloor();
    }else
        addFloor();


    // start bird anim
    bird->startFlying();

    // start pillars anim
    if(!pillarTimer->isActive()){
        cleanPillars();
        pillarTimer->start(1000); // initialize pillars spawning (rate)
    }

    setGameOn(true);

    setCurrentScore(0);

    hideGraphics();
}

/* Bird movement for key space */
void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space && gameOn)
        bird->moveUp();
    QGraphicsScene::keyPressEvent(event);
}
/* Bird movement for left button click */
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && gameOn)
        bird->moveUp();
    QGraphicsScene::mousePressEvent(event);
}
