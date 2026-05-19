#ifndef CAPITANBARBARROJA_H
#define CAPITANBARBARROJA_H

#include <QObject>
#include "barco.h"

// ── CapitanBarbarroja: controlador del jugador ────────────────────────────────
// Gestiona la entrada del jugador, el estado del capitán y aplica
// las físicas de borrachera al barco que posee.

class CapitanBarbarroja : public QObject {
    Q_OBJECT
public:
    explicit CapitanBarbarroja(Barco* barco, QObject* parent = nullptr);
    ~CapitanBarbarroja() = default;

    // ── Game loop ──
    // Aplica la oscilación de borrachera y delega al barco
    void actualizar(float deltaTiempo);

    // ── Control ──
    void girarVelasIzquierda();
    void girarVelasDerecha();
    void subirBarco();    // nivel 2
    void bajarBarco();    // nivel 2

    // ── Estado ──
    Barco*  getBarco()          const;
    float   getNivelBorrachera() const;
    int     getMonedas()         const;

    void agregarMonedas(int cantidad);
    void setNivelBorrachera(float nivel);

private:
    Barco* m_barco;           // puntero al barco (no es dueño)
    float  m_nivelBorrachera; // 0.0 = sobrio, 1.0 = muy borracho
    int    m_monedas;
    float  m_tiempoAcumulado; // para la función sinusoidal de borrachera

    // Constantes de borrachera (Física 2 del nivel 1)
    static constexpr float A_EBRIEDAD    = 15.0f;  // amplitud en px
    static constexpr float F_EBRIEDAD    = 0.8f;   // frecuencia (rad/s)
    static constexpr float GIRO_VELAS    = 0.05f;  // radianes por pulsación
};

#endif // CAPITANBARBARROJA_H
