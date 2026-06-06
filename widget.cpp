#include "widget.h"
#include "ui_widget.h"
#include "nivel2oscuridad.h"
#include <QtMath>
#include <QGraphicsRectItem>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , vidas(3)
    , m_nivel2(nullptr)
{
    ui->setupUi(this);

    // Configuración escena
    scene = new QGraphicsScene(ui->graphicsView->rect(), this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(scene);

    // Fondo
    QPixmap originalImagen(":/imagenes/fondo_juego.png");
    QPixmap mirroredImagen = originalImagen.transformed(QTransform().scale(-1, 1));
    bg1 = new QGraphicsPixmapItem(originalImagen);
    bg2 = new QGraphicsPixmapItem(mirroredImagen);
    bg1->setPos(0, 0);
    bg2->setPos(originalImagen.width(), 0.0);
    scene->addItem(bg1);
    scene->addItem(bg2);

    // Timer fondo
    bgTimer = new QTimer(this);
    connect(bgTimer, &QTimer::timeout, this, &Widget::bgMove);
    bgTimer->start(10);

    // Barco
    QPixmap pixmapPlayer(":/imagenes/Barco.png");
    barco = new Barco(ui->graphicsView, pixmapPlayer, scene->sceneRect(), this);
    barco->setScale(0.05);
    barco->setPos(310, 210);
    barco->setZValue(1);
    scene->addItem(barco);

    // Timer para crear olas
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
    // Tiempo
    tiempoRestante = 60;
    txtTiempo = new QGraphicsTextItem();
    txtTiempo->setDefaultTextColor(Qt::white);
    txtTiempo->setFont(QFont("Arial", 16, QFont::Bold));
    txtTiempo->setPlainText("⏱ 60s");
    txtTiempo->setPos(10, 40);
    txtTiempo->setZValue(10);
    scene->addItem(txtTiempo);

    // Puntos
    txtPuntos = new QGraphicsTextItem();
    txtPuntos->setDefaultTextColor(QColor(255, 215, 0));
    txtPuntos->setFont(QFont("Arial", 16, QFont::Bold));
    txtPuntos->setPlainText("⭐ 0 pts");
    txtPuntos->setPos(10, 70);
    txtPuntos->setZValue(10);
    scene->addItem(txtPuntos);

    // Timer HUD — actualiza cada segundo
    timerHUD = new QTimer(this);
    connect(timerHUD, &QTimer::timeout, this, [this]() {
        tiempoRestante--;
        txtTiempo->setPlainText(QString("⏱ %1s").arg(tiempoRestante));
        txtPuntos->setPlainText(QString("⭐ %1 pts").arg((60 - tiempoRestante) * 10));
        if (tiempoRestante <= 0) timerHUD->stop();
    });
    timerHUD->start(1000);
    // Efecto borracho — oscilación sinusoidal lateral
    QTimer *timerBorracho = new QTimer(this);
    float *tiempoBorracho = new float(0.0f);
    connect(timerBorracho, &QTimer::timeout, this, [this, tiempoBorracho]() {
        *tiempoBorracho += 0.05f;
        // desplazamiento_lateral(t) = A_ebriedad * sin(f * t)
        float offsetX = 8.0f * static_cast<float>(qSin(*tiempoBorracho));
        float newX = qBound(scene->sceneRect().left(),
                            barco->x() + offsetX,
                            scene->sceneRect().right() - barco->boundingRect().width() * barco->scale());
        barco->setX(newX);
    });
    timerBorracho->start(50);

    // Timer colisiones
    colisionTimer = new QTimer(this);
    connect(colisionTimer, &QTimer::timeout, this, &Widget::verificarColisiones);
    colisionTimer->start(50);

    // Timer meta - 60 segundos
    timerMeta = new QTimer(this);
    connect(timerMeta, &QTimer::timeout, this, &Widget::juegoGanado);
    timerMeta->setSingleShot(true);
    timerMeta->start(60000);
}


Widget::~Widget()
{
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (m_nivel2) {
        m_nivel2->keyPressEvent(event);
        return;
    }

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
    QList<Ola*> olasValidas;
    for (Ola *ola : olas)
        if (ola && ola->scene() != nullptr)
            olasValidas.append(ola);
    olas = olasValidas;

    for (Ola *ola : olas) {
        if (barco->collidesWithItem(ola)) {
            vidas--;
            vidasTexto->setPlainText("Vidas: " + QString::number(vidas));
            ola->setPos(scene->sceneRect().width() + 100, ola->y());
            if (vidas <= 0) {
                bgTimer->stop();
                colisionTimer->stop();
                olaTimer->stop();
                timerMeta->stop();
                timerHUD->stop();

                QGraphicsRectItem* overlay = new QGraphicsRectItem(scene->sceneRect());
                overlay->setBrush(QColor(0, 0, 0, 160));
                overlay->setPen(Qt::NoPen);
                overlay->setZValue(20);
                scene->addItem(overlay);

                QGraphicsTextItem* txtGO = new QGraphicsTextItem("💀 ¡Game Over!");
                txtGO->setDefaultTextColor(Qt::red);
                txtGO->setFont(QFont("Arial", 32, QFont::Bold));
                txtGO->setPos(scene->sceneRect().width()/2 - 160,
                              scene->sceneRect().height()/2 - 60);
                txtGO->setZValue(21);
                scene->addItem(txtGO);

                QGraphicsTextItem* txtRein = new QGraphicsTextItem("Cierra y abre para reintentar");
                txtRein->setDefaultTextColor(Qt::white);
                txtRein->setFont(QFont("Arial", 16));
                txtRein->setPos(scene->sceneRect().width()/2 - 160,
                                scene->sceneRect().height()/2 + 10);
                txtRein->setZValue(21);
                scene->addItem(txtRein);
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

    // Posición Y aleatoria en toda la pantalla
    int randomY = rand() % static_cast<int>(scene->sceneRect().height());

    Ola *ola = new Ola(spriteOlas, configs[randomConfig][0],
                       configs[randomConfig][1], this);
    ola->setPos(scene->sceneRect().width(), randomY);
    scene->addItem(ola);
    olas.append(ola);
}

void Widget::juegoGanado()
{
    bgTimer->stop();
    colisionTimer->stop();
    olaTimer->stop();
    timerMeta->stop();
    timerHUD->stop();

    iniciarNivel2();
}

void Widget::iniciarNivel2()
{
    // Detener Nivel 1
    bgTimer->stop();
    colisionTimer->stop();
    olaTimer->stop();
    timerMeta->stop();

    // Crear Nivel 2
    if (m_nivel2) { m_nivel2->pausar(); delete m_nivel2; }
    m_nivel2 = new Nivel2Oscuridad(ui->graphicsView, this);

    connect(m_nivel2, &Nivel2Oscuridad::nivelCompletado, this, [this]() {
        auto* txt = new QGraphicsTextItem("🏴‍☠️ ¡Misión cumplida! El tesoro es tuyo.");
        txt->setDefaultTextColor(QColor(255,215,0));
        txt->setFont(QFont("Arial", 22, QFont::Bold));
        txt->setPos(80, 300);
        m_nivel2->getEscena()->addItem(txt);
    });

    connect(m_nivel2, &Nivel2Oscuridad::nivelFallido, this, [this](int pts) {
        auto* txt = new QGraphicsTextItem(
            QString("💀 El barco se hundió... Puntos: %1").arg(pts));
        txt->setDefaultTextColor(Qt::red);
        txt->setFont(QFont("Arial", 20, QFont::Bold));
        txt->setPos(80, 320);
        m_nivel2->getEscena()->addItem(txt);
    });

    m_nivel2->iniciar();
    ui->graphicsView->setFocus();
    connect(m_nivel2, &Nivel2Oscuridad::nivelFallido, this, [this](int pts) {
        auto* txt = new QGraphicsTextItem(
            QString("💀 El barco se hundió... Puntos: %1").arg(pts));
        txt->setDefaultTextColor(Qt::red);
        txt->setFont(QFont("Arial", 20, QFont::Bold));
        txt->setPos(80, 320);
        m_nivel2->getEscena()->addItem(txt);
    });

    m_nivel2->iniciar();
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    if (m_nivel2) {
        m_nivel2->keyReleaseEvent(event);
        return;
    }
}

