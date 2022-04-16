#include "floor.h"

Floor::Floor(QPixmap pixmap)
{
    setPixmap(pixmap);
}

Floor::~Floor()
{
    qDebug() << "Deleting floor ";
    delete floor;
}
