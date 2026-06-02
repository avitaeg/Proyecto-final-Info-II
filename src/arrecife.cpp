// ── arrecife.cpp ──────────────────────────────────────────────────────────────
#include "arrecife.h"
#include <QPainter>

Arrecife::Arrecife(float x, float y, QGraphicsItem* parent)
    : Obstaculo(x, y, DANIO_ARRECIFE, parent)
    , m_resistencia(RESISTENCIA_MAX)
{
    m_ancho = 96.0f;
    m_alto  = 96.0f;
}

void Arrecife::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    p->setBrush(QColor(80, 60, 40));
    p->setPen(Qt::NoPen);
    p->drawEllipse(boundingRect());
    p->setPen(Qt::white);
    p->drawText(boundingRect(), Qt::AlignCenter, "⛰");
}

void Arrecife::actualizar(float) { /* estático */ }

void Arrecife::alColisionar(Entidad* otro) {
    Obstaculo::alColisionar(otro);
    m_resistencia--;
    if (m_resistencia <= 0) setActivo(false);
}

int Arrecife::getResistencia() const { return m_resistencia; }
