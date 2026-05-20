#ifndef OBJETOVOLADOR_H
#define OBJETOVOLADOR_H

#include "obstaculo.h"
#include "tipos.h"

// ── ObjetoVolador: cae con trayectoria parabólica en nivel 2 ─────────────────
// Física 2 nivel 2: x(t) = x0 + Vx·t,  y(t) = y0 + Vy·t + ½·g·t²

class ObjetoVolador : public Obstaculo {
public:
    explicit ObjetoVolador(float x, float y,
                           float vx, float vy,
                           TipoObjeto tipo,
                           QGraphicsItem* parent = nullptr);

    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void actualizar(float deltaTiempo) override;

    TipoObjeto getTipo() const;

private:
    TipoObjeto m_tipo;
    float m_tiempo;           // tiempo acumulado para la parábola
    float m_vxInicial;
    float m_vyInicial;

    static constexpr float GRAVEDAD    = 980.0f;  // px/s² (≈ 9.8 m/s² escalado)
    static constexpr int   DANIO_OBJETO = 25;
};

#endif // OBJETOVOLADOR_H
