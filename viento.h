#ifndef VIENTO_H
#define VIENTO_H

#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QList>
#include <QObject>
#include <QTimer>

class Viento : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Viento(QRectF sceneBounds, QObject *parent = nullptr);
    bool estaActivo() const { return activo; }

public slots:
    void mover();
    void activar();

private:
    QTimer *timerMovimiento;
    QTimer *timerActivacion;
    bool activo;
    QRectF sceneBounds;
    QList<QGraphicsLineItem *> lineas;
};

#endif // VIENTO_H
