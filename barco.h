#ifndef BARCO_H
#define BARCO_H

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QObject>

class Barco : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Barco(QGraphicsView *view,
          const QPixmap pixmapPlayer,
          const QRectF sceneBounds,
          QObject *parent = nullptr);

public slots:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

private:
    QPixmap pixmapPlayer;
    QRectF sceneBounds;
    QGraphicsView *view;
};

#endif // BARCO_H
