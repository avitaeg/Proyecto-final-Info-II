// ── olagigante.cpp ────────────────────────────────────────────────────────────
#include "olagigante.h"
#include <QPainter>
#include <QtMath>

OlaGigante::OlaGigante(float x, float yBase, float amplitud, float frecuencia,
                       QGraphicsItem* parent)
    : Obstaculo(x, yBase, DANIO_OLA, parent)
    , m_amplitud(amplitud), m_frecuencia(frecuencia)
    , m_fase(0.0f), m_yBase(yBase), m_tiempo(0.0f)
{ m_ancho = 80.0f; m_alto = 60.0f; }

void OlaGigante::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    p->setBrush(QColor(30, 100, 200, 180));
    p->setPen(Qt::NoPen);
    p->drawEllipse(boundingRect());
}

void OlaGigante::actualizar(float dt) {
    m_tiempo += dt;
    // y_ola(t) = A * sin(2π·f·t + φ)
    float y = m_yBase + m_amplitud * static_cast<float>(
                  qSin(2.0 * M_PI * m_frecuencia * m_tiempo + m_fase));
    setPos(pos().x(), y);
}

float OlaGigante::getAmplitud() const { return m_amplitud; }
void  OlaGigante::setAmplitud(float a) { m_amplitud = a; }
