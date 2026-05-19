#ifndef NIVEL2OSCURIDAD_H
#define NIVEL2OSCURIDAD_H

#include "nivel.h"
#include "barco.h"
#include "capitanbarbarroja.h"
#include "kraken.h"
#include "agentekraken.h"
#include "olagigante.h"
#include "objetovolador.h"
#include <QKeyEvent>
#include <QRadialGradient>

// ── Nivel 2: El Mar de la Oscuridad ─────────────────────────────────────────
// Vista lateral con scroll horizontal. El barco avanza automáticamente.
// Físicas: olas sinusoidales, caída parabólica, tentáculos.

class Nivel2Oscuridad : public Nivel {
    Q_OBJECT
public:
    explicit Nivel2Oscuridad(Barco* barco,
                             CapitanBarbarroja* capitan,
                             QObject* parent = nullptr);

    void configurarEscena()    override;
    void procesarEntrada()     override;
    void verificarCondiciones() override;

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    // ── Física 1: Movimiento de olas ─────────────────────────────────────────
    void aplicarMovimientoOlas();

    // ── Física 2: Spawn de objetos voladores ─────────────────────────────────
    void spawnObjetoVolador();

    // ── Scroll automático ────────────────────────────────────────────────────
    void avanzarBarco();

    // ── Efecto de linterna (visibilidad limitada) ─────────────────────────────
    void actualizarLinterna();

    // ── Generación de contenido ──────────────────────────────────────────────
    void generarOlas();
    void colocarFaro();

    // ── Entidades ────────────────────────────────────────────────────────────
    Barco*             m_barco;
    CapitanBarbarroja* m_capitan;
    Kraken*            m_kraken;
    AgenteKraken*      m_agente;
    Item*              m_faro;

    // ── Estado ──────────────────────────────────────────────────────────────
    float m_distanciaRecorrida;
    float m_tiempoSpawnObjeto;
    float m_tiempoOla;
    float m_amplitudOla;   // aumenta con el tiempo
    float m_frecuenciaOla;

    bool m_arribaPresionada;
    bool m_abajoPresionada;

    // ── Constantes ───────────────────────────────────────────────────────────
    static constexpr float DISTANCIA_FARO      = 8000.0f;  // px para aparecer faro
    static constexpr float VEL_AVANCE          = 150.0f;   // px/s avance auto
    static constexpr float INTERVALO_SPAWN     = 2.5f;     // segundos entre objetos
    static constexpr float A_OLA_INICIAL       = 30.0f;
    static constexpr float F_OLA_INICIAL       = 0.5f;
    static constexpr float RADIO_LINTERNA      = 200.0f;
    static constexpr float TIEMPO_NIVEL2       = 999.0f;   // sin límite real
};

#endif // NIVEL2OSCURIDAD_H
