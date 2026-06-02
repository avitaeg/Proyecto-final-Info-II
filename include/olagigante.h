#ifndef OLAGIGANTE_H
#define OLAGIGANTE_H

#include "obstaculo.h"

// ── OlaGigante: nivel 2, sigue función sinusoidal ─────────────────────────────
// Física 1 del nivel 2: y_ola(t) = A * sin(2π·f·t + φ)

class OlaGigante : public Obstaculo {
public:
    explicit OlaGigante(float x, float yBase,
                        float amplitud, float frecuencia,
                        QGraphicsItem* parent = nullptr);

    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void actualizar(float deltaTiempo) override;

    float getAmplitud() const;
    void  setAmplitud(float a);

private:
    float m_amplitud;
    float m_frecuencia;
    float m_fase;
    float m_yBase;
    float m_tiempo;

    static constexpr int DANIO_OLA = 15;
};

#endif // OLAGIGANTE_H
