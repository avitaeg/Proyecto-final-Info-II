// ── item.cpp ─────────────────────────────────────────────────────────────────
#include "item.h"
#include <QPainter>
#include <QtMath>

Item::Item(float x, float y, const QString& nombre, QGraphicsItem* parent)
    : Entidad(x, y, parent), m_nombre(nombre)
    , m_recogido(false), m_tiempoFlotacion(0.0f)
{ m_ancho = 48.0f; m_alto = 48.0f; }

void Item::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    if (m_recogido) return;
    p->setBrush(QColor(255, 215, 0));
    p->setPen(Qt::NoPen);
    p->drawEllipse(boundingRect());
    p->setPen(Qt::black);
    p->drawText(boundingRect(), Qt::AlignCenter, "★");
}

void Item::actualizar(float dt) {
    m_tiempoFlotacion += dt;
    float offsetY = 4.0f * static_cast<float>(qSin(3.0 * m_tiempoFlotacion));
    setPos(pos().x(), pos().y() + offsetY * dt);
}

void Item::alColisionar(Entidad* otro) {
    Q_UNUSED(otro);
    if (!m_recogido) {
        m_recogido = true;
        setActivo(false);
    }
}

QString Item::getNombre()   const { return m_nombre; }
bool    Item::fueRecogido() const { return m_recogido; }
