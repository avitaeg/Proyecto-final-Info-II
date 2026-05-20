#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //Configuración
    scene = new QGraphicsScene (ui->graphicsView-> rect(), this);
    ui ->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui ->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(scene);

    //Fondo de pantalla
    QPixmap originalImagen("://imagenes/fondo_juego.png");
    bg1 = new QGraphicsPixmapItem(originalImagen);
    bg1-> setPos (0, 0);
    scene -> addItem(bg1);

    //Timer para mover el fondo

}

Widget::~Widget()
{
    delete ui;
}
