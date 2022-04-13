#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>

#include "pillaritem.h"
#include "birditem.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    void addBird();

    void startGame();

    bool getGameOn() const;
    void setGameOn(bool newGameOn);

    void incrementScore();

    void setCurrentScore(int newCurrentScore);

signals:

public slots:

private:
    void showGameOverGraphics();
    void hideGameOverGraphics();
    void setUpPillarTimer();

    void freezeGame();
    bool gameOn;
    void cleanPillars();


    QTimer * pillarTimer;
    BirdItem * bird;

    int currentScore;
    int bestScore;

    QGraphicsPixmapItem * gameOverPix = nullptr;
    QGraphicsTextItem * scoreTextItem = nullptr;

    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
