#include "widget.h"
#include "ui_widget.h"
#include "barco.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //Configuración
    scene = new QGraphicsScene(ui->graphicsView->rect(), this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(scene);

    //Fondo de pantalla
    QPixmap originalImagen(":/imagenes/fondo_juego.png");
    QPixmap mirroredImagen = originalImagen.transformed(QTransform().scale(-1,1));
    bg1 = new QGraphicsPixmapItem(originalImagen);
    bg2 = new QGraphicsPixmapItem(mirroredImagen);
    bg1->setPos(0, 0);
    bg2->setPos(originalImagen.width(), 0.0);
    scene->addItem(bg1);
    scene->addItem(bg2);

    //Timer para mover el fondo
    bgTimer = new QTimer(this);
    connect(bgTimer, &QTimer::timeout, this, &Widget::bgMove);
    bgTimer->start(10);

    //Añadir barco
    QPixmap pixmapPlayer(":/imagenes/Barco.png");
    barco = new Barco(ui->graphicsView,
                      pixmapPlayer,
                      scene->sceneRect(),
                      this);
    barco->setScale(0.06);
    barco->setPos(50, 250);
    barco->setZValue(1);
    scene->addItem(barco);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        barco->moveUp();
    else if (event->key() == Qt::Key_S)
        barco->moveDown();
    else if (event->key() == Qt::Key_A)
        barco->moveLeft();
    else if (event->key() == Qt::Key_D)
        barco->moveRight();
}

void Widget::bgMove()
{
    bg1->setX(bg1->x() - 1);
    bg2->setX(bg2->x() - 1);

    if (bg1->x() <= -bg1->pixmap().width())
        bg1->setX(bg2->x() + bg2->pixmap().width());

    if (bg2->x() <= -bg2->pixmap().width())
        bg2->setX(bg1->x() + bg1->pixmap().width());
}
