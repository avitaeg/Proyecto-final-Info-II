#include "obstaculo.h"

Obstaculo::Obstaculo(float x, float y, int danio, QGraphicsItem* parent)
    : Entidad(x, y, parent)
    , m_danio(danio)
    , m_visible(true)
{}

void Obstaculo::alColisionar(Entidad* otro) {
    Q_UNUSED(otro);
    // El daño lo gestiona el nivel directamente via detectarColisiones()
}

int  Obstaculo::getDanio()  const { return m_danio; }
bool Obstaculo::isVisible() const { return m_visible; }

void Obstaculo::setDanioObstaculo(int danio) {
    m_danio = danio;
}
