#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

#include "pillaritem.h"
#include "birditem.h"
#include "interactiveimage.h"
#include "floor.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void addBird();
    void addFloor();
    void addStartMenu();

    void setGameOn(bool newGameOn);

    void startGame();
    void incrementScore();

private:
    void setUpPillarTimer();
    void endOfTheRound();
    void freezeGame();

    void cleanFloor();
    void cleanPillars();

    void showGameoverGraphics();
    void hideGraphics();

    void setCurrentScore(int newCurrentScore);


    bool gameOn; // player can't play if not true (allow ketpress and mousepress)

    QTimer * pillarTimer;
    BirdItem * bird;
    Floor * floorItem;

    QGraphicsPixmapItem * gameoverTitle = nullptr;
    QGraphicsPixmapItem * medal = nullptr;
    QGraphicsTextItem * scoreTextItem = nullptr;
    QGraphicsTextItem * bestscoreTextItem = nullptr;
    QGraphicsPixmapItem * gameoverBlock = nullptr;

    interactiveImage * myStartButton = nullptr;
    interactiveImage * myMenuButton = nullptr;
    interactiveImage * myScoreboardButton = nullptr;

    int currentScore;
    int bestScore;

    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
