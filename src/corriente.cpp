// ── corriente.cpp ────────────────────────────────────────────────────────────
#include "corriente.h"
#include <QPainter>

Corriente::Corriente(float x, float y, float ancho, float alto,
                     float dirX, float dirY, QGraphicsItem* parent)
    : Entidad(x, y, parent), m_dirX(dirX), m_dirY(dirY)
{
    m_ancho = ancho;
    m_alto  = alto;
}

void Corriente::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    p->setBrush(QColor(0, 100, 200, 40));
    p->setPen(QPen(QColor(0, 150, 255, 80), 1));
    p->drawRect(boundingRect());
    // Flecha de dirección
    p->setPen(QPen(Qt::cyan, 2));
    QPointF center = boundingRect().center();
    p->drawLine(center, center + QPointF(m_dirX * 30, m_dirY * 30));
}

void Corriente::actualizar(float) { /* zona estática */ }

bool Corriente::contiene(float px, float py) const {
    QRectF area(pos().x(), pos().y(), m_ancho, m_alto);
    return area.contains(px, py);
}

float Corriente::getDirX() const { return m_dirX; }
float Corriente::getDirY() const { return m_dirY; }
