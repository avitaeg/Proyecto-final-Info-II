#include "agentekraken.h"
#include <QtMath>
#include <QRandomGenerator>
#include <algorithm>

AgenteKraken::AgenteKraken(Kraken* kraken, Barco* barco, QGraphicsItem* parent)
    : Entidad(0, 0, parent)
    , m_kraken(kraken)
    , m_barco(barco)
    , m_agresividad(AGRESIVIDAD_INICIAL)
    , m_tiempoEntreAtaques(TIEMPO_BASE_ATAQUE)
    , m_tiempoDesdeUltimoAtaque(0.0f)
    , m_yBarcoAnterior(barco ? barco->getY() : 0.0f)
    , m_atacarArriba(false)
{
    m_historial["arriba"] = 0;
    m_historial["abajo"]  = 0;
    m_activo = true;
}

void AgenteKraken::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) {}

void AgenteKraken::actualizar(float deltaTiempo) {
    if (!m_activo || !m_kraken || !m_barco) return;

    m_tiempoDesdeUltimoAtaque += deltaTiempo;
    percibir();

    if (m_tiempoDesdeUltimoAtaque >= m_tiempoEntreAtaques) {
        razonar();
        actuar();
        m_tiempoDesdeUltimoAtaque = 0.0f;
        m_agresividad = qMin(1.0f, m_agresividad + INCREMENTO_AGRESIVIDAD);
        m_tiempoEntreAtaques = TIEMPO_BASE_ATAQUE * (1.0f - m_agresividad * 0.5f);
    }

    m_kraken->actualizar(deltaTiempo);
}

void AgenteKraken::percibir() {
    if (!m_barco) return;
    float yActual = m_barco->getY();
    float delta   = yActual - m_yBarcoAnterior;
    if (delta < -2.0f)      m_historial["arriba"]++;
    else if (delta > 2.0f)  m_historial["abajo"]++;
    m_yBarcoAnterior = yActual;
}

void AgenteKraken::razonar() {
    int arriba = m_historial["arriba"];
    int abajo  = m_historial["abajo"];

    float probAtacarArriba = (arriba > 0 || abajo > 0)
        ? static_cast<float>(arriba) / (arriba + abajo)
        : 0.5f;

    float ruido = (1.0f - m_agresividad) * 0.4f;
    float aleatorio = static_cast<float>(QRandomGenerator::global()->generateDouble());
    probAtacarArriba += ruido * (aleatorio - 0.5f);

    m_atacarArriba = probAtacarArriba > 0.5f;
}

void AgenteKraken::actuar() {
    if (!m_kraken) return;
    float amplitud   = 60.0f + m_agresividad * 80.0f;
    float frecuencia = 1.0f  + m_agresividad * 2.0f;
    m_kraken->atacar(amplitud, frecuencia);
    bool impacto = m_barco->getSalud() < 100;
    aprender(impacto);
}

void AgenteKraken::aprender(bool impacto) {
    if (impacto) {
        if (m_atacarArriba) m_historial["arriba"] += 2;
        else                m_historial["abajo"]  += 2;
    }
}

float AgenteKraken::getAgresividad() const { return m_agresividad; }
