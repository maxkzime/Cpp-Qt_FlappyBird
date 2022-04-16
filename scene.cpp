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
 * - Game-over block
 * - Score and Best score
 * - Menu button
 * - Start button
 * - Medals
 */
void Scene::showGameoverGraphics()
{
    // game-over title
    gameoverTitle = new QGraphicsPixmapItem(QPixmap(":/imgs/gameover.png").scaled(288,63));
    addItem(gameoverTitle);
    gameoverTitle->setPos(QPointF(0,0) - QPointF(gameoverTitle->boundingRect().width()/2,
                                                 gameoverTitle->boundingRect().height()*3));
    // game-over block
    gameoverBlock = new QGraphicsPixmapItem(QPixmap(":/imgs/cadre.png").scaled(226,116));
    addItem(gameoverBlock);
    gameoverBlock->setPos(QPointF(0,0) - QPointF(gameoverBlock->boundingRect().width()/2,
                                                 gameoverTitle->boundingRect().height()*1.8));

    // score and best score for game-over block
    scoreTextItem = new QGraphicsTextItem(QString::number(currentScore));
    bestscoreTextItem = new QGraphicsTextItem(QString::number(bestScore));

    QFont mFont("Consolas", 20, QFont::Bold);

    scoreTextItem->setFont(mFont);
    bestscoreTextItem->setFont(mFont);

    scoreTextItem->setDefaultTextColor(Qt::darkYellow);
    bestscoreTextItem->setDefaultTextColor(Qt::darkYellow);

    addItem(scoreTextItem);
    addItem(bestscoreTextItem);

    scoreTextItem->setPos(QPointF(0,0) - QPointF((scoreTextItem->boundingRect().width()/2)-gameoverBlock->boundingRect().width()/3,
                                                 gameoverTitle->boundingRect().height()*1.4));
    bestscoreTextItem->setPos(QPointF(0,0) - QPointF((bestscoreTextItem->boundingRect().width()/2)-gameoverBlock->boundingRect().width()/3,
                                                     gameoverTitle->boundingRect().height()*0.7));

    // Medals
    if(currentScore > bestScore-1) // gold medal
    {
        medal = new QGraphicsPixmapItem(QPixmap(":/imgs/goldMedal.png").scaled(QSize(60,60)));
        addItem(medal);
        medal->setPos(QPointF(0,0) + QPointF((-medal->boundingRect().width()-36),
                                             (-medal->boundingRect().height()-22)));
    }
    else if(currentScore > 9 && currentScore < 16) // Bronze medal
    {
        medal = new QGraphicsPixmapItem(QPixmap(":/imgs/bronzeMedal.png").scaled(QSize(60,60)));
        addItem(medal);
        medal->setPos(QPointF(0,0) + QPointF((-medal->boundingRect().width()-36),
                                             (-medal->boundingRect().height()-22)));
    }
    else if(currentScore > 49) // silver medal
    {
        medal = new QGraphicsPixmapItem(QPixmap(":/imgs/silverMedal.png").scaled(QSize(60,60)));
        addItem(medal);
        medal->setPos(QPointF(0,0) + QPointF((-medal->boundingRect().width()-36),
                                             (-medal->boundingRect().height()-22)));
    }


    // menu button
    myMenuButton = new interactiveImage(QPixmap(":/imgs/menu.png").scaled(160,56));
    addItem(myMenuButton);
    myMenuButton->setPos(QPointF(0,0) - QPointF(myMenuButton->boundingRect().width()/2,
                                                gameoverTitle->boundingRect().height()-80));

    connect(myMenuButton,&interactiveImage::clickOnBimg,[=](){
        qDebug() << "showing menu !";
        hideGraphics();
        cleanPillars();
    });

    // start button
    myStartButton = new interactiveImage(QPixmap(":/imgs/start.png").scaled(QSize(160,56)));
    addItem(myStartButton);
    myStartButton->setPos(QPointF(0,0) - QPointF(myStartButton->boundingRect().width()/2,
                                                 gameoverTitle->boundingRect().height()-150));

    connect(myStartButton,&interactiveImage::clickOnBimg,[=](){
        hideGraphics();
        startGame();
    });

}


/* Delete game-over and start graphics :
 * - Game-over title
 * - Game-over block (showing score)
 * - Score text & Bestscore text
 * - Start button
 * - Menu button
 * - Scoreboard button
 * - Medal
 */
void Scene::hideGraphics()
{
    if(gameoverTitle != nullptr){
        removeItem(gameoverTitle);
        delete gameoverTitle;
        gameoverTitle = nullptr;
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
    if(bestscoreTextItem != nullptr){
        removeItem(bestscoreTextItem);
        delete bestscoreTextItem;
        bestscoreTextItem = nullptr;
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
    if(myScoreboardButton != nullptr){
        removeItem(myScoreboardButton);
        delete myScoreboardButton;
        myScoreboardButton = nullptr;
    }
    if(medal != nullptr){
        removeItem(medal);
        delete medal;
        medal = nullptr;
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
 * - Title
 * - Start button
 * Scoreboard button
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

    myScoreboardButton = new interactiveImage(QPixmap(":/imgs/score.png").scaled(QSize(160,56)));
    addItem(myScoreboardButton);
    myScoreboardButton->setPos(QPointF(0,0) - QPointF(myScoreboardButton->boundingRect().width()/2,
                                                      (myScoreboardButton->boundingRect().height()/2)-100));

    connect(myScoreboardButton,&interactiveImage::clickOnBimg,[=](){
        qDebug() << "showing scoreboard";
        //delete myStartButton;
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
