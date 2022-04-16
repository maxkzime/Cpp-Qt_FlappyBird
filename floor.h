#ifndef FLOOR_H
#define FLOOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class Floor : public QObject,  public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)

public:
    explicit Floor(QPixmap pixmap);
    qreal x() const;
    void setX(qreal newX);
    void stopFloor();

private:
    QPropertyAnimation * xAnimation;
    qreal m_x;
};

#endif // FLOOR_H
