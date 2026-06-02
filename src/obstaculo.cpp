#include "obstaculo.h"
#include "barco.h"

Obstaculo::Obstaculo(float x, float y, int danio, QGraphicsItem* parent)
    : Entidad(x, y, parent)
    , m_danio(danio)
    , m_visible(true)
{}

void Obstaculo::alColisionar(Entidad* otro) {
    // Si colisiona con el barco, le aplica daño
    Barco* barco = dynamic_cast<Barco*>(otro);
    if (barco) {
        barco->recibirDanio(m_danio);
    }
}

int  Obstaculo::getDanio()  const { return m_danio; }
bool Obstaculo::isVisible() const { return m_visible; }

void Obstaculo::setDanioObstaculo(int danio) {
    m_danio = danio;
}
