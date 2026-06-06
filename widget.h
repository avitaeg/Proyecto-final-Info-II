#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QList>
#include <QKeyEvent>
#include <QFont>
#include "barco.h"
#include "ola.h"
#include "viento.h"

class Nivel2Oscuridad;

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
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void bgMove();
    void verificarColisiones();
    void crearOla();
    void juegoGanado();
    void iniciarNivel2();

private:
    Ui::Widget          *ui;
    QGraphicsScene      *scene;
    QGraphicsPixmapItem *bg1, *bg2;
    QTimer *bgTimer;
    QTimer *colisionTimer;
    QTimer *olaTimer;
    QTimer *timerMeta;
    QTimer *timerHUD;

    Barco  *barco;
    QList<Ola*> olas;
    Viento *viento;

    int vidas;
    int tiempoRestante;

    QGraphicsTextItem *vidasTexto;
    QGraphicsTextItem *txtTiempo;
    QGraphicsTextItem *txtPuntos;

    Nivel2Oscuridad *m_nivel2;
};

#endif // WIDGET_H
