#ifndef INTERACTIVEIMAGE_H
#define INTERACTIVEIMAGE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>

class interactiveImage : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    interactiveImage(QPixmap pixmap);

signals:
    void clickOnBimg();

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // INTERACTIVEIMAGE_H
