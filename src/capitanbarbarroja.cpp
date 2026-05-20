#include "capitanbarbarroja.h"
#include <QtMath>

CapitanBarbarroja::CapitanBarbarroja(Barco* barco, QObject* parent)
    : QObject(parent)
    , m_barco(barco)
    , m_nivelBorrachera(1.0f)  // siempre borracho por diseño
    , m_monedas(0)
    , m_tiempoAcumulado(0.0f)
{}

void CapitanBarbarroja::actualizar(float deltaTiempo) {
    if (!m_barco || !m_barco->isActivo()) return;

    m_tiempoAcumulado += deltaTiempo;

    // ── Física 2: Oscilación lateral por ebriedad ──────────────────────────
    // desplazamiento_lateral(t) = A_ebriedad * sin(f * t)
    // Se aplica como offset sobre la posición Y del barco en nivel 1,
    // o sobre X en nivel 2 (lo selecciona el Nivel al llamar).
    if (m_barco->esBorracho()) {
        float oscilacion = A_EBRIEDAD * m_nivelBorrachera
                           * static_cast<float>(qSin(F_EBRIEDAD * m_tiempoAcumulado));
        // El barco expone el offset; el nivel decide si es X o Y
        m_barco->setPos(m_barco->pos().x(),
                        m_barco->pos().y() + oscilacion * deltaTiempo);
    }
}

void CapitanBarbarroja::girarVelasIzquierda() {
    if (!m_barco) return;
    float angulo = m_barco->getAnguloVela() - GIRO_VELAS;
    m_barco->setAnguloVela(angulo);
}

void CapitanBarbarroja::girarVelasDerecha() {
    if (!m_barco) return;
    float angulo = m_barco->getAnguloVela() + GIRO_VELAS;
    m_barco->setAnguloVela(angulo);
}

void CapitanBarbarroja::subirBarco() {
    if (m_barco) m_barco->subirBarco();
}

void CapitanBarbarroja::bajarBarco() {
    if (m_barco) m_barco->bajarBarco();
}

Barco*  CapitanBarbarroja::getBarco()           const { return m_barco; }
float   CapitanBarbarroja::getNivelBorrachera()  const { return m_nivelBorrachera; }
int     CapitanBarbarroja::getMonedas()          const { return m_monedas; }

void CapitanBarbarroja::agregarMonedas(int cantidad) {
    m_monedas += cantidad;
}

void CapitanBarbarroja::setNivelBorrachera(float nivel) {
    m_nivelBorrachera = qBound(0.0f, nivel, 1.0f);
    m_barco->setEsBorracho(m_nivelBorrachera > 0.0f);
}
