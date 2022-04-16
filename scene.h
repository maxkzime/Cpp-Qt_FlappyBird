#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>

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
    void addStartMenu();
    void addFloor();

    void startGame();

    bool getGameOn() const;
    void setGameOn(bool newGameOn);

    void incrementScore();

    void setCurrentScore(int newCurrentScore);

signals:

public slots:

private:
    void endOfTheRound();

    void showGameoverGraphics();
    void hideGraphics();
    void setUpPillarTimer();

    void freezeGame();
    bool gameOn;
    void cleanPillars();


    QTimer * pillarTimer;
    BirdItem * bird;
    Floor * floorItem;


    QGraphicsPixmapItem * gameoverPix = nullptr;
    QGraphicsTextItem * scoreTextItem = nullptr;
    QGraphicsPixmapItem * gameoverBlock = nullptr;

    interactiveImage * myStartButton = nullptr;
    interactiveImage * myMenuButton = nullptr;




    int currentScore;
    int bestScore;



    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
