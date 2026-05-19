#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "entidad.h"

// ── Obstáculo: base de todos los elementos dañinos ────────────────────────────

class Obstaculo : public Entidad {
public:
    explicit Obstaculo(float x, float y, int danio, QGraphicsItem* parent = nullptr);

    void actualizar(float deltaTiempo) override = 0;
    void alColisionar(Entidad* otro) override;

    int  getDanio()   const;
    bool isVisible()  const;
    void setDanioObstaculo(int danio);

protected:
    int  m_danio;
    bool m_visible;
};

#endif // OBSTACULO_H
