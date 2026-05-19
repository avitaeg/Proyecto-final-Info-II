#ifndef NIVEL1ARCHIPIELAGO_H
#define NIVEL1ARCHIPIELAGO_H

#include "nivel.h"
#include "barco.h"
#include "capitanbarbarroja.h"
#include "corriente.h"
#include "arrecife.h"
#include "item.h"
#include <QKeyEvent>
#include <QtMath>

// ── Nivel 1: El Archipiélago de las Tortugas ──────────────────────────────────
// Vista cenital con scroll. Físicas: viento, borrachera, corrientes.

class Nivel1Archipielago : public Nivel {
    Q_OBJECT
public:
    explicit Nivel1Archipielago(Barco* barco,
                                CapitanBarbarroja* capitan,
                                QObject* parent = nullptr);

    void configurarEscena()    override;
    void procesarEntrada()     override;
    void verificarCondiciones() override;

    // Llamado por MainWindow al recibir QKeyEvent
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    // ── Física 1: Propulsión Eólica ──────────────────────────────────────────
    // V_barco = V_viento * cos(theta) * k
    // W(t) = A * sin(w*t + phi)
    void actualizarViento();
    void aplicarPropulsionEolica();

    // ── Física 3: Corrientes ─────────────────────────────────────────────────
    void aplicarCorrientes();

    // ── Generación del mapa ──────────────────────────────────────────────────
    void generarArrecifes();
    void generarCorrientes();
    void colocarMeta();

    // ── Estado del viento ────────────────────────────────────────────────────
    float m_dirViento;      // ángulo actual del viento en radianes
    float m_velViento;      // magnitud del viento
    float m_tiempoViento;   // acumulado para W(t)

    // ── Entidades específicas ────────────────────────────────────────────────
    Barco*             m_barco;
    CapitanBarbarroja* m_capitan;
    Item*              m_meta;             // isla del tesoro

    std::vector<Corriente*> m_corrientes;

    // ── Teclas presionadas ───────────────────────────────────────────────────
    bool m_izqPresionada;
    bool m_derPresionada;

    // ── Constantes ───────────────────────────────────────────────────────────
    static constexpr float TIEMPO_NIVEL1   = 180.0f; // 3 minutos
    static constexpr float A_VIENTO        = 1.0f;   // amplitud cambio dirección
    static constexpr float W_VIENTO        = 0.3f;   // frecuencia angular viento
    static constexpr float K_EFICIENCIA    = 0.8f;   // coef. eficiencia vela
    static constexpr float VEL_VIENTO_MAX  = 200.0f; // px/s
    static constexpr float ESCENA_ANCHO    = 3000.0f;
    static constexpr float ESCENA_ALTO     = 2000.0f;
};

#endif // NIVEL1ARCHIPIELAGO_H
