#ifndef VIENTO_H
#define VIENTO_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QTimer>
#include <QList>

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
    QList<QGraphicsLineItem*> lineas;
};

#endif // VIENTO_H
