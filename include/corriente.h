#ifndef CORRIENTE_H
#define CORRIENTE_H

#include "entidad.h"

// ── Corriente: zona del mapa con vector de flujo constante ────────────────────
// Física 3 nivel 1: V_total = V_barco + V_corriente
// No daña, pero empuja al barco en su dirección.

class Corriente : public Entidad {
public:
    explicit Corriente(float x, float y,
                       float ancho, float alto,
                       float dirX, float dirY,
                       QGraphicsItem* parent = nullptr);

    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void actualizar(float deltaTiempo) override;

    float getDirX() const;
    float getDirY() const;

    // Devuelve true si el punto (px, py) está dentro del área de la corriente
    bool contiene(float px, float py) const;

private:
    float m_dirX;   // componente X del vector de flujo
    float m_dirY;   // componente Y del vector de flujo
};

#endif // CORRIENTE_H
