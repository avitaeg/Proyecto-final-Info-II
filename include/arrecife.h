#ifndef ARRECIFE_H
#define ARRECIFE_H

#include "obstaculo.h"

// ── Arrecife: obstáculo estático del nivel 1 ─────────────────────────────────

class Arrecife : public Obstaculo {
public:
    explicit Arrecife(float x, float y, QGraphicsItem* parent = nullptr);

    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void actualizar(float deltaTiempo) override;
    void alColisionar(Entidad* otro) override;

    int getResistencia() const;

private:
    int m_resistencia;   // cuántos golpes aguanta antes de romperse

    static constexpr int RESISTENCIA_MAX = 3;
    static constexpr int DANIO_ARRECIFE  = 20;
};

#endif // ARRECIFE_H
