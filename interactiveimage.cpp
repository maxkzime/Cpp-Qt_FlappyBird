#include "interactiveimage.h"

interactiveImage::interactiveImage(QPixmap pixmap)
{setPixmap(QPixmap(pixmap));}


void interactiveImage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::RightButton)
        emit clickOnBimg();
}
