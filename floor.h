#ifndef FLOOR_H
#define FLOOR_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Floor : public QObject,  public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Floor(QPixmap pixmap);
    ~Floor();

private:
    QGraphicsPixmapItem  * floor;

};

#endif // FLOOR_H
