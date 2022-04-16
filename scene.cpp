#include "scene.h"

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent),
      gameOn(false),
      currentScore(0),
      bestScore(0)
{setUpPillarTimer();}


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


/* Intialize floor */
void Scene::addFloor()
{
    floorItem = new Floor(QPixmap(":/imgs/floorElongated.png").scaled(QSize(2464,112)));
    addItem(floorItem);
    floorItem->setPos(QPointF(-250,0) + QPointF(0,240));
}


/* Initialize pillar animation and collision */
void Scene::setUpPillarTimer()
{
    pillarTimer = new QTimer(this);
    connect(pillarTimer, &QTimer::timeout,[=](){
        PillarItem * pillarItem = new PillarItem();
        connect(pillarItem,&PillarItem::collideFail,[=](){
            endOfTheRound();
        });
        addItem(pillarItem);
    });
}


/* End of the round :
 * - stop the pillars spawning
 * -
 * - stop game
 * - show game-over summary with restart and menu buttons
 */
void Scene::endOfTheRound()
{
    pillarTimer->stop();
    freezeGame();
    setGameOn(false);
    showGameoverGraphics();
}


/* Freeze the game :
 * - freeze bird anim
 * - freeze floor anim
 * - freeze pillar anim
 */
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


/* Getter + Setter for gameOn */
//bool Scene::getGameOn() const {return gameOn;}
void Scene::setGameOn(bool newGameOn) {gameOn = newGameOn;}


/* Delete floor */
void Scene::cleanFloor()
{
    removeItem(floorItem);
    delete floorItem;

}


/* Delete pillars
 * List elements of the scene,
 * detect if its a pillar,
 * if true, remove and delete it
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


/* increment the score,
 * update best score
 */
void Scene::incrementScore()
{
    currentScore++;
    if(currentScore > bestScore){ bestScore = currentScore; }

    //qDebug() << "Score : " << currentScore << " Bestscore : " << bestScore;
}


/*
 * Create start menu :
 * - Title
 * - Start button
 * - Scoreboard button
 */
void Scene::addStartMenu()
{
    // Title
    QGraphicsPixmapItem * title = new QGraphicsPixmapItem(QPixmap(":/imgs/title.png").scaled(294,72));
    addItem(title);
    title->setPos(QPointF(0,0) - QPointF(title->boundingRect().width()/2,
                                         (title->boundingRect().height()/2)+100));

    // Start button
    myStartButton = new interactiveImage(QPixmap(":/imgs/start.png").scaled(QSize(160,56)));
    addItem(myStartButton);
    myStartButton->setPos(QPointF(0,0) - QPointF(myStartButton->boundingRect().width()/2,
                                                 myStartButton->boundingRect().height()/2));

    connect(myStartButton,&interactiveImage::clickOnBimg,[=](){
        startGame();
        delete title;
    });

    // Scoreboard button
    myScoreboardButton = new interactiveImage(QPixmap(":/imgs/score.png").scaled(QSize(160,56)));
    addItem(myScoreboardButton);
    myScoreboardButton->setPos(QPointF(0,0) - QPointF(myScoreboardButton->boundingRect().width()/2,
                                                      (myScoreboardButton->boundingRect().height()/2)-100));

    connect(myScoreboardButton,&interactiveImage::clickOnBimg,[=](){
        qDebug() << "showing scoreboard";
        //delete myStartButton;
    });
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
    if(currentScore > (bestScore != 0 ? bestScore-1 : bestScore)) // gold medal
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
        hideGraphics();
        cleanPillars();
        addStartMenu();
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


/* Starting a game :
 * - Cleaning or adding floor (restarting animation)
 * - Starting bird animation
 * - Starting pillar animation
 * - reset score
 * - hide graphics (menu, game-over, ect..)
 */
void Scene::startGame()
{
    // adding floor if necessary
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

    setCurrentScore(0); // reset score

    hideGraphics(); // hiding graphics
}


// Setter for the score (score = 0 at the start of a round)
void Scene::setCurrentScore(int newCurrentScore)
{currentScore = newCurrentScore;}


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
