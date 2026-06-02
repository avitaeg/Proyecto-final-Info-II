#include "entidad.h"

Entidad::Entidad(float x, float y, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_vx(0.0f)
    , m_vy(0.0f)
    , m_activo(true)
    , m_ancho(64.0f)
    , m_alto(64.0f)
{
    setPos(x, y);
}

QRectF Entidad::boundingRect() const {
    return QRectF(0, 0, m_ancho, m_alto);
}

void Entidad::alColisionar(Entidad* otro) {
    Q_UNUSED(otro);
    // Comportamiento por defecto: nada. Las subclases lo sobreescriben.
}

float Entidad::getX()  const { return static_cast<float>(pos().x()); }
float Entidad::getY()  const { return static_cast<float>(pos().y()); }
float Entidad::getVX() const { return m_vx; }
float Entidad::getVY() const { return m_vy; }
bool  Entidad::isActivo() const { return m_activo; }

void Entidad::setVelocidad(float vx, float vy) {
    m_vx = vx;
    m_vy = vy;
}

void Entidad::setActivo(bool estado) {
    m_activo = estado;
    setVisible(estado);
}
