#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <vector>
#include "entidad.h"

// ── Nivel: clase base abstracta para cada nivel del juego ─────────────────────
// Gestiona la QGraphicsScene, el game loop, las entidades y el timer.

class Nivel : public QObject {
    Q_OBJECT
public:
    explicit Nivel(const QString& nombre, float tiempoLimite,
                   QObject* parent = nullptr);
    virtual ~Nivel();

    // ── Ciclo de vida ──
    virtual void iniciar();
    virtual void pausar();
    virtual void reanudar();
    virtual void reiniciar();

    // ── Interfaz que cada nivel debe implementar ──
    virtual void configurarEscena()   = 0;   // colocar entidades
    virtual void procesarEntrada()    = 0;   // manejar input
    virtual void verificarCondiciones() = 0; // victoria / derrota

    // ── Acceso ──
    QGraphicsScene* getEscena()        const;
    float           getTiempoRestante() const;
    QString         getNombre()        const;
    bool            estaActivo()       const;

signals:
    void nivelCompletado();
    void nivelFallido();
    void tiempoActualizado(float tiempoRestante);

protected slots:
    void tick();   // llamado cada 16ms (~60fps)

protected:
    void agregarEntidad(Entidad* entidad);
    void eliminarEntidadesInactivas();
    void detectarColisiones();

    QGraphicsScene*        m_escena;
    QTimer*                m_timer;
    std::vector<Entidad*>  m_entidades;

    QString m_nombre;
    float   m_tiempoRestante;
    float   m_tiempoLimite;
    bool    m_activo;

    static constexpr int   INTERVALO_MS  = 16;           // ~60 fps
    static constexpr float DELTA_TIEMPO  = 16.0f / 1000; // segundos
};

#endif // NIVEL_H
