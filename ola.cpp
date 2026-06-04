#include "ola.h"
#include <QDebug>

Ola::Ola(const QPixmap &sprite, int fila, int columna, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem()
{
    // Recorta la ola correspondiente del spritesheet (2 columnas x 2 filas)
    int w = sprite.width() / 2;
    int h = sprite.height() / 2;
    QPixmap recorte = sprite.copy(columna * w, fila * h, w, h);
    setPixmap(recorte);
    qDebug() << "Ola recorte size:" << recorte.size() << "isNull:" << recorte.isNull();
    setScale(0.20);
    setZValue(2);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Ola::mover);
    timer->start(20);
}

void Ola::mover()
{
    setPos(x() - 4, y());
    if (scene() && x() < -pixmap().width() * scale())
    {
        setPos(scene()->sceneRect().width(), y());
    }
}
