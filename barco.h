#ifndef BARCO_H
#define BARCO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

class Barco : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Barco(QGraphicsView *view, const QPixmap pixmapPlayer, const QRectF sceneBounds,QObject *parent = nullptr);

signals:

private:
    QPixmap pixmapPlayer;
    QRectF sceneBounds;
    QGraphicsView *view;
};

#endif // BARCO_H
