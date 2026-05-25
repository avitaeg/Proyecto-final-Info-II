#include "barco.h"

Barco::Barco(QGraphicsView *view, const QPixmap pixmapPlayer, const QRectF sceneBounds, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(), pixmapPlayer(pixmapPlayer), sceneBounds(sceneBounds), view(view)
{
    setPixmap(pixmapPlayer);
}
