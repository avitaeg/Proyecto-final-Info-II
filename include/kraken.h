#ifndef KRAKEN_H
#define KRAKEN_H

#include "obstaculo.h"
#include "tipos.h"

// ── Kraken: obstáculo con estado propio, controlado por AgenteKraken ──────────
// La lógica inteligente vive en AgenteKraken; Kraken solo sabe dibujarse
// y aplicar sus físicas de tentáculos.

class Kraken : public Obstaculo {
public:
    explicit Kraken(float x, float y, QGraphicsItem* parent = nullptr);

    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void actualizar(float deltaTiempo) override;

    // ── Tentáculos ──
    // Física 3 nivel 2: y_tentaculo(t) = y_base + A_t * sin(w_t * t)
    void atacar(float amplitud, float frecuencia);
    void retroceder();

    EstadoKraken getEstado() const;

private:
    EstadoKraken m_estado;
    float m_tiempo;

    // Parámetros del tentáculo actual
    float m_amplitudTentaculo;
    float m_frecuenciaTentaculo;
    float m_yBase;

    static constexpr int DANIO_KRAKEN = 30;
};

#endif // KRAKEN_H
