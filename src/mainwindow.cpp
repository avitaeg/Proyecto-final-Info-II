#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_juego(new Juego(this))
    , m_stack(new QStackedWidget(this))
    , m_vista(new QGraphicsView(this))
    , m_labelHUD(new QLabel(this))
    , m_labelPuntos(new QLabel(this))
{
    configurarUI();

    connect(m_juego, &Juego::estadoCambiado,
            this, &MainWindow::onEstadoCambiado);
    connect(m_juego, &Juego::puntuacionActualizada,
            this, &MainWindow::onPuntuacionActualizada);
}

MainWindow::~MainWindow() {}

void MainWindow::configurarUI() {
    setCentralWidget(m_stack);

    // ── Pantalla Menú ──────────────────────────────────────────────────────
    QWidget* menuWidget = new QWidget;
    QVBoxLayout* menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setAlignment(Qt::AlignCenter);

    QLabel* titulo = new QLabel("⚓ El Tormenta Escarlata ⚓");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 32px; font-weight: bold; color: #8B4513;");

    QPushButton* btnJugar = new QPushButton("Zarpar al Nivel 1");
    btnJugar->setFixedSize(220, 50);
    btnJugar->setStyleSheet("font-size: 18px;");
    connect(btnJugar, &QPushButton::clicked, [this]() { m_juego->iniciar(); });

    menuLayout->addWidget(titulo);
    menuLayout->addSpacing(40);
    menuLayout->addWidget(btnJugar, 0, Qt::AlignCenter);

    // ── Pantalla de juego ──────────────────────────────────────────────────
    QWidget* juegoWidget = new QWidget;
    QVBoxLayout* juegoLayout = new QVBoxLayout(juegoWidget);
    juegoLayout->setContentsMargins(0, 0, 0, 0);
    juegoLayout->setSpacing(0);

    // HUD
    QWidget* hud = new QWidget;
    hud->setFixedHeight(40);
    hud->setStyleSheet("background: rgba(0,0,0,180);");
    QHBoxLayout* hudLayout = new QHBoxLayout(hud);
    m_labelHUD->setStyleSheet("color: white; font-size: 14px;");
    m_labelPuntos->setStyleSheet("color: gold; font-size: 14px;");
    hudLayout->addWidget(m_labelHUD);
    hudLayout->addStretch();
    hudLayout->addWidget(m_labelPuntos);

    juegoLayout->addWidget(hud);
    juegoLayout->addWidget(m_vista);

    // ── Pantalla Game Over ─────────────────────────────────────────────────
    QWidget* gameoverWidget = new QWidget;
    QVBoxLayout* goLayout = new QVBoxLayout(gameoverWidget);
    goLayout->setAlignment(Qt::AlignCenter);
    QLabel* goLabel = new QLabel("💀 El barco se hundió...");
    goLabel->setStyleSheet("font-size: 28px; color: red;");
    goLabel->setAlignment(Qt::AlignCenter);
    QPushButton* btnReintentar = new QPushButton("Reintentar");
    btnReintentar->setFixedSize(180, 44);
    connect(btnReintentar, &QPushButton::clicked, [this]() { mostrarMenu(); });
    goLayout->addWidget(goLabel);
    goLayout->addSpacing(20);
    goLayout->addWidget(btnReintentar, 0, Qt::AlignCenter);

    // ── Pantalla Victoria ──────────────────────────────────────────────────
    QWidget* victoriaWidget = new QWidget;
    QVBoxLayout* vLayout = new QVBoxLayout(victoriaWidget);
    vLayout->setAlignment(Qt::AlignCenter);
    QLabel* vLabel = new QLabel("🏴‍☠️ ¡Tesoro conquistado!");
    vLabel->setStyleSheet("font-size: 28px; color: gold;");
    vLabel->setAlignment(Qt::AlignCenter);
    QPushButton* btnMenu = new QPushButton("Volver al Menú");
    btnMenu->setFixedSize(180, 44);
    connect(btnMenu, &QPushButton::clicked, [this]() { mostrarMenu(); });
    vLayout->addWidget(vLabel);
    vLayout->addSpacing(20);
    vLayout->addWidget(btnMenu, 0, Qt::AlignCenter);

    m_stack->addWidget(menuWidget);    // IDX_MENU     = 0
    m_stack->addWidget(juegoWidget);   // IDX_JUEGO    = 1
    m_stack->addWidget(gameoverWidget);// IDX_GAMEOVER = 2
    m_stack->addWidget(victoriaWidget);// IDX_VICTORIA = 3

    mostrarMenu();
}

void MainWindow::mostrarMenu() {
    m_stack->setCurrentIndex(IDX_MENU);
}

void MainWindow::mostrarNivel(int numero) {
    Q_UNUSED(numero);
    Nivel* nivel = m_juego->getNivelActual();
    if (nivel) {
        m_vista->setScene(nivel->getEscena());
        m_vista->setRenderHint(QPainter::Antialiasing);
        m_vista->setDragMode(QGraphicsView::NoDrag);
    }
    m_stack->setCurrentIndex(IDX_JUEGO);
}

void MainWindow::mostrarGameOver() {
    m_stack->setCurrentIndex(IDX_GAMEOVER);
}

void MainWindow::mostrarVictoria() {
    m_stack->setCurrentIndex(IDX_VICTORIA);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    // Reenviar al nivel activo
    Nivel* nivel = m_juego->getNivelActual();
    if (!nivel) return;

    // TODO: castear al nivel concreto para procesar entrada
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::onEstadoCambiado(EstadoJuego estado) {
    switch (estado) {
        case EstadoJuego::NIVEL1:    mostrarNivel(1); break;
        case EstadoJuego::NIVEL2:    mostrarNivel(2); break;
        case EstadoJuego::GAME_OVER: mostrarGameOver(); break;
        case EstadoJuego::VICTORIA:  mostrarVictoria(); break;
        case EstadoJuego::MENU:      mostrarMenu(); break;
    }
}

void MainWindow::onPuntuacionActualizada(int puntuacion) {
    m_labelPuntos->setText(QString("Monedas: %1").arg(puntuacion));
}
