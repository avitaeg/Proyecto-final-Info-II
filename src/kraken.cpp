#include "kraken.h"
#include <QPainter>
#include <QtMath>

Kraken::Kraken(float x, float y, QGraphicsItem* parent)
    : Obstaculo(x, y, DANIO_KRAKEN, parent)
    , m_estado(EstadoKraken::RETROCEDER)
    , m_tiempo(0.0f)
    , m_amplitudTentaculo(60.0f)
    , m_frecuenciaTentaculo(1.0f)
    , m_yBase(y)
{ m_ancho = 120.0f; m_alto = 120.0f; }

void Kraken::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    QColor col = (m_estado == EstadoKraken::ATACAR)
                 ? QColor(180, 20, 20, 200)
                 : QColor(80, 20, 120, 150);
    p->setBrush(col);
    p->setPen(Qt::NoPen);
    p->drawEllipse(boundingRect());
}

void Kraken::actualizar(float dt) {
    if (!m_activo) return;
    m_tiempo += dt;
    if (m_estado == EstadoKraken::ATACAR) {
        // Física 3 nivel 2: y_tentaculo(t) = y_base + A_t * sin(w_t * t)
        float y = m_yBase + m_amplitudTentaculo
                  * static_cast<float>(qSin(m_frecuenciaTentaculo * m_tiempo));
        setPos(pos().x(), y);
    }
}

void Kraken::atacar(float amplitud, float frecuencia) {
    m_estado              = EstadoKraken::ATACAR;
    m_amplitudTentaculo   = amplitud;
    m_frecuenciaTentaculo = frecuencia;
}

void Kraken::retroceder() {
    m_estado = EstadoKraken::RETROCEDER;
}

EstadoKraken Kraken::getEstado() const { return m_estado; }
