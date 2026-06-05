#ifndef NIVEL2OSCURIDAD_H
#define NIVEL2OSCURIDAD_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QKeyEvent>
#include <QList>
#include <QPixmap>
#include <QtMath>
#include <QRandomGenerator>
#include <QPainter>
#include <QRadialGradient>

// ── Nivel 2: El Mar de la Oscuridad ──────────────────────────────────────────
// Vista CENITAL nocturna. W/A/S/D mueven el barco en 4 direcciones.
// Kraken fijo a la derecha con movimiento sinusoidal (Física 3).
// Barriles caen con trayectoria parabólica (Física 2).
// AgenteKraken aprende la dirección de evasión del jugador.

class Nivel2Oscuridad : public QObject
{
    Q_OBJECT
public:
    explicit Nivel2Oscuridad(QGraphicsView* view, QObject* parent = nullptr);
    ~Nivel2Oscuridad();

    void iniciar();
    void pausar();

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    QGraphicsScene* getEscena() const { return m_scene; }

signals:
    void nivelCompletado();
    void nivelFallido(int puntuacion);
    void hudActualizado(int salud, int puntuacion, int tiempo);

private slots:
    void tick();
    void animarKraken();
    void spawnObstaculo();

private:
    // Setup
    void setupFondo();
    void setupBarco();
    void setupKraken();
    void setupHUD();
    void reconstruirLinterna();

    // Físicas
    void moverBarco();
    void actualizarKraken();   // sinusoidal + AgenteKraken
    void actualizarObjetos();  // caída parabólica
    void razonarKraken();      // IA

    // Colisiones y HUD
    void detectarColisiones();
    void refrescarHUD();

    // Helper
    QPixmap quitarFondo(const QPixmap& src, const QColor& bg, int tol = 45);

    // ── Escena ───────────────────────────────────────────────────────────
    QGraphicsScene* m_scene;
    QGraphicsView*  m_view;

    // ── Fondo ────────────────────────────────────────────────────────────
    QGraphicsPixmapItem* m_fondo;

    // ── Barco ────────────────────────────────────────────────────────────
    QGraphicsPixmapItem* m_barcoItem;
    float m_bx, m_by;
    static constexpr float VEL_BARCO = 160.0f;

    // ── Kraken ───────────────────────────────────────────────────────────
    QGraphicsPixmapItem* m_krakenItem;
    QPixmap  m_krakenSheet;
    int      m_kFrame, m_kFrameW, m_kFrameH;
    float    m_kxBase, m_kyBase;
    float    m_kTiempo, m_kAmplitud, m_kFrecuencia;

    // ── AgenteKraken ─────────────────────────────────────────────────────
    float m_agresividad;
    float m_tAtaque, m_intervaloAtaque;
    int   m_evasionesArriba, m_evasionesAbajo;
    int   m_evasionesIzq, m_evasionsDer;
    float m_byAnt, m_bxAnt;

    // ── Obstáculos (caída parabólica) ─────────────────────────────────────
    struct Objeto {
        QGraphicsPixmapItem* item = nullptr;
        float x0, y0, vx, vy, t;
        bool activo = true;
    };
    QList<Objeto> m_objetos;
    QPixmap m_pixBarril;

    // ── Linterna overlay ──────────────────────────────────────────────────
    QGraphicsPixmapItem* m_overlayItem;

    // ── HUD ──────────────────────────────────────────────────────────────
    QGraphicsTextItem* m_hudSalud;
    QGraphicsTextItem* m_hudPuntos;
    QGraphicsTextItem* m_hudTiempo;

    // ── Estado ───────────────────────────────────────────────────────────
    int   m_salud, m_puntuacion;
    float m_tiempoJuego;
    bool  m_activo;
    bool  m_wPress, m_sPress, m_aPress, m_dPress;

    // ── Timers ───────────────────────────────────────────────────────────
    QTimer* m_timerLoop;
    QTimer* m_timerKAnim;
    QTimer* m_timerSpawn;

    // ── Constantes ───────────────────────────────────────────────────────
    static constexpr float DT              = 0.016f;
    static constexpr int   W               = 1280;
    static constexpr int   H               = 720;
    static constexpr int   SALUD_MAX       = 100;
    static constexpr float RADIO_LUZ       = 230.0f;
    static constexpr float GRAVEDAD        = 550.0f;
    static constexpr float TIEMPO_VICTORIA = 90.0f;   // segundos
    static constexpr int   DANIO_OBJ       = 12;
    static constexpr int   DANIO_KRAKEN    = 20;
    static constexpr float AGR_INICIAL     = 0.25f;
    static constexpr float INTERVALO_BASE  = 4.5f;
};

#endif // NIVEL2OSCURIDAD_H
