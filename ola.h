#ifndef OLA_H
#define OLA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>

class Ola : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Ola(const QPixmap &sprite, int fila, int columna, QObject *parent = nullptr);

public slots:
    void mover();

private:
    QTimer *timer;
};

#endif // OLA_H
