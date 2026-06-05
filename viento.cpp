#include "viento.h"
#include <QBrush>
#include <QColor>
#include <QPen>

Viento::Viento(QRectF sceneBounds, QObject *parent)
    : QObject(parent)
    , QGraphicsRectItem()
    , sceneBounds(sceneBounds)
    , activo(false)
{
    setRect(0, 0, 120, 400);
    setPen(QPen(QColor(255, 255, 255, 150), 3));
    setBrush(Qt::NoBrush);
    setZValue(5);
    setPos(sceneBounds.width(), 0);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Viento::mover);
    timerMovimiento->start(20);

    timerActivacion = new QTimer(this);
    connect(timerActivacion, &QTimer::timeout, this, &Viento::activar);
    timerActivacion->start(7000);
}

void Viento::mover()
{
    if (activo) {
        setPos(x() - 6, y());
        if (x() < -200) {
            activo = false;
            setPos(sceneBounds.width(), 0);
        }
    }
}

void Viento::activar()
{
    activo = true;
    setPos(sceneBounds.width(), 0);
}
