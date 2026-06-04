#include "widget.h"
#include "ui_widget.h"
#include "barco.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , vidas(3)
{
    ui->setupUi(this);

    // Configuración escena
    scene = new QGraphicsScene(ui->graphicsView->rect(), this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(scene);

    // Fondo
    QPixmap originalImagen(":/imagenes/fondo_juego.png");
    QPixmap mirroredImagen = originalImagen.transformed(QTransform().scale(-1,1));
    bg1 = new QGraphicsPixmapItem(originalImagen);
    bg2 = new QGraphicsPixmapItem(mirroredImagen);
    bg1->setPos(0, 0);
    bg2->setPos(originalImagen.width(), 0.0);
    scene->addItem(bg1);
    scene->addItem(bg2);

    // Tiempo del fondo fondo
    bgTimer = new QTimer(this);
    connect(bgTimer, &QTimer::timeout, this, &Widget::bgMove);
    bgTimer->start(10);

    // Barco
    QPixmap pixmapPlayer(":/imagenes/Barco.png");
    barco = new Barco(ui->graphicsView, pixmapPlayer, scene->sceneRect(), this);
    barco->setScale(0.06);
    barco->setPos(50, 250);
    barco->setZValue(1);
    scene->addItem(barco);

    // Olas iniciales
    QPixmap spriteOlas(":/imagenes/Olas.png");
    int posicionesY[4] = {350, 400, 300, 320};
    int configs[4][2] = {{0,0},{0,1},{1,0},{1,1}};
    for (int i = 0; i < 4; i++)
    {
        Ola *ola = new Ola(spriteOlas, configs[i][0], configs[i][1], this);
        ola->setPos(200 + i * 150, posicionesY[i]);
        scene->addItem(ola);
        olas.append(ola);
    }

    // Tiempo para crear nuevas olas
    olaTimer = new QTimer(this);
    connect(olaTimer, &QTimer::timeout, this, &Widget::crearOla);
    olaTimer->start(5000);

    // Viento
    viento = new Viento(scene->sceneRect(), this);
    scene->addItem(viento);

    // Vidas
    vidasTexto = new QGraphicsTextItem();
    vidasTexto->setDefaultTextColor(Qt::white);
    vidasTexto->setFont(QFont("Arial", 16, QFont::Bold));
    vidasTexto->setPlainText("Vidas: 3");
    vidasTexto->setPos(10, 10);
    vidasTexto->setZValue(10);
    scene->addItem(vidasTexto);

    // Timer colisiones
    colisionTimer = new QTimer(this);
    connect(colisionTimer, &QTimer::timeout, this, &Widget::verificarColisiones);
    colisionTimer->start(50);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    int velocidad = viento->estaActivo() ? 5 : 10;
    qreal bw = barco->boundingRect().width() * barco->scale();
    qreal bh = barco->boundingRect().height() * barco->scale();
    qreal newX = barco->x();
    qreal newY = barco->y();

    if (event->key() == Qt::Key_W)
        newY -= velocidad;
    else if (event->key() == Qt::Key_S)
        newY += velocidad;
    else if (event->key() == Qt::Key_A)
        newX -= velocidad;
    else if (event->key() == Qt::Key_D)
        newX += velocidad;

    // Limitar dentro de la escena
    newX = qBound(scene->sceneRect().left(), newX, scene->sceneRect().right() - bw);
    newY = qBound(scene->sceneRect().top(), newY, scene->sceneRect().bottom() - bh);

    barco->setPos(newX, newY);
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

void Widget::verificarColisiones()
{
    // Limpiar olas que ya no están en la escena
    for (int i = olas.size() - 1; i >= 0; i--)
    {
        if (olas[i]->scene() == nullptr)
            olas.removeAt(i);
    }

    for (Ola *ola : olas)
    {
        if (barco->collidesWithItem(ola))
        {
            vidas--;
            vidasTexto->setPlainText("Vidas: " + QString::number(vidas));
            ola->setPos(scene->sceneRect().width() + 100, ola->y());

            if (vidas <= 0)
            {
                vidasTexto->setPlainText("¡Game Over!");
                bgTimer->stop();
                colisionTimer->stop();
                olaTimer->stop();
            }
            break;
        }
    }
}
void Widget::crearOla()
{
    QPixmap spriteOlas(":/imagenes/Olas.png");
    int configs[4][2] = {{0,0},{0,1},{1,0},{1,1}};
    int randomConfig = rand() % 4;

    Ola *ola = new Ola(spriteOlas, configs[randomConfig][0], configs[randomConfig][1], this);
    ola->setPos(scene->sceneRect().width(), 310);
    scene->addItem(ola);
    olas.append(ola);
}
