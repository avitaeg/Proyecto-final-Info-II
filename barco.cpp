#include "barco.h"

Barco::Barco(QGraphicsView *view, const QPixmap pixmapPlayer, const QRectF sceneBounds, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(), pixmapPlayer(pixmapPlayer), sceneBounds(sceneBounds), view(view)
{
    setPixmap(pixmapPlayer);
}
//Movimiento del barco
void Barco::moveUp()
{
    setPos(x(), y() - 10);
}

void Barco::moveDown()
{
    setPos(x(), y() + 10);
}

void Barco::moveLeft()
{
    setPos(x() - 10, y());
}

void Barco::moveRight()
{
    setPos(x() + 10, y());
}
