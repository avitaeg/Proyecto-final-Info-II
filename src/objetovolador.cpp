#include "objetovolador.h"
#include <QPainter>
#include <QtMath>

ObjetoVolador::ObjetoVolador(float x, float y, float vx, float vy,
                             TipoObjeto tipo, QGraphicsItem* parent)
    : Obstaculo(x, y, DANIO_OBJETO, parent)
    , m_tipo(tipo), m_tiempo(0.0f)
    , m_vxInicial(vx), m_vyInicial(vy)
{ m_ancho = 40.0f; m_alto = 40.0f; }

void ObjetoVolador::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    p->setBrush(QColor(139, 69, 19));
    p->setPen(Qt::NoPen);
    p->drawRect(boundingRect());
    p->setPen(Qt::white);
    QString icon = (m_tipo == TipoObjeto::BARRIL)  ? "🛢" :
                   (m_tipo == TipoObjeto::MADERO)  ? "🪵" : "⚡";
    p->drawText(boundingRect(), Qt::AlignCenter, icon);
}

void ObjetoVolador::actualizar(float dt) {
    m_tiempo += dt;
    // x(t) = x0 + Vx·t,  y(t) = y0 + Vy·t + ½·g·t²
    float x0 = static_cast<float>(pos().x()) - m_vxInicial * (m_tiempo - dt);
    float y0 = static_cast<float>(pos().y())
               - (m_vyInicial * (m_tiempo - dt) + 0.5f * GRAVEDAD * (m_tiempo - dt) * (m_tiempo - dt));

    float nx = x0 + m_vxInicial * m_tiempo;
    float ny = y0 + m_vyInicial * m_tiempo + 0.5f * GRAVEDAD * m_tiempo * m_tiempo;
    setPos(nx, ny);

    // Desactivar si sale de la pantalla
    if (ny > 1000.0f) setActivo(false);
}

TipoObjeto ObjetoVolador::getTipo() const { return m_tipo; }
