#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QList>
#include "barco.h"
#include "ola.h"
#include "viento.h"
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void keyPressEvent(QKeyEvent *event);

public slots:
    void bgMove();
    void verificarColisiones();
    void crearOla();
    void juegoGanado();

private:
    Ui::Widget *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *bg1, *bg2;
    QTimer *bgTimer;
    QTimer *colisionTimer;
    QTimer *olaTimer;
    Barco *barco;
    QList<Ola*> olas;
    Viento *viento;
    int vidas;
    QGraphicsTextItem *vidasTexto;
    QTimer *timerMeta;

};

#endif // WIDGET_H
