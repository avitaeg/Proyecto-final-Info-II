// ── nivel1archipielago.cpp ───────────────────────────────────────────────────
#include "nivel1archipielago.h"
#include <QRandomGenerator>
#include <QtMath>

Nivel1Archipielago::Nivel1Archipielago(Barco* barco,
                                       CapitanBarbarroja* capitan,
                                       QObject* parent)
    : Nivel("El Archipiélago de las Tortugas", TIEMPO_NIVEL1, parent)
    , m_barco(barco), m_capitan(capitan), m_meta(nullptr)
    , m_dirViento(0.0f), m_velViento(100.0f), m_tiempoViento(0.0f)
    , m_izqPresionada(false), m_derPresionada(false)
{
    m_escena->setSceneRect(0, 0, ESCENA_ANCHO, ESCENA_ALTO);
    m_escena->setBackgroundBrush(QColor(15, 80, 140));
}

void Nivel1Archipielago::configurarEscena() {
    // Colocar barco en el puerto de salida
    m_barco->setPos(200, ESCENA_ALTO / 2);
    m_escena->addItem(m_barco);

    generarArrecifes();
    generarCorrientes();
    colocarMeta();
}

void Nivel1Archipielago::generarArrecifes() {
    // Distribuir arrecifes aleatoriamente por el mapa
    for (int i = 0; i < 20; ++i) {
        float rx = 400.0f + QRandomGenerator::global()->bounded(static_cast<int>(ESCENA_ANCHO - 600));
        float ry = QRandomGenerator::global()->bounded(static_cast<int>(ESCENA_ALTO));
        auto* a = new Arrecife(rx, ry);
        agregarEntidad(a);
    }
}

void Nivel1Archipielago::generarCorrientes() {
    // Dos corrientes favorables y una desfavorable
    auto* c1 = new Corriente(800, 400, 300, 200, 1.0f, 0.2f);
    auto* c2 = new Corriente(1500, 800, 300, 200, 1.0f, -0.3f);
    auto* c3 = new Corriente(2200, 300, 250, 200, -0.5f, 0.5f);
    agregarEntidad(c1);
    agregarEntidad(c2);
    agregarEntidad(c3);
    m_corrientes = {c1, c2, c3};
}

void Nivel1Archipielago::colocarMeta() {
    m_meta = new Item(ESCENA_ANCHO - 200, ESCENA_ALTO / 2, "Isla del Tesoro");
    agregarEntidad(m_meta);
}

void Nivel1Archipielago::procesarEntrada() {
    if (m_izqPresionada) m_capitan->girarVelasIzquierda();
    if (m_derPresionada) m_capitan->girarVelasDerecha();
}

void Nivel1Archipielago::verificarCondiciones() {
    // Derrota: tiempo agotado o barco destruido
    if (m_tiempoRestante <= 0 || !m_barco->isActivo()) {
        m_activo = false;
        m_timer->stop();
        emit nivelFallido();
        return;
    }

    // Victoria: llegó a la isla del tesoro
    if (m_meta && m_meta->fueRecogido()) {
        m_activo = false;
        m_timer->stop();
        emit nivelCompletado();
    }
}

void Nivel1Archipielago::actualizarViento() {
    m_tiempoViento += DELTA_TIEMPO;
    // W(t) = A * sin(w*t + phi)
    m_dirViento = A_VIENTO * static_cast<float>(qSin(W_VIENTO * m_tiempoViento));
}

void Nivel1Archipielago::aplicarPropulsionEolica() {
    if (!m_barco) return;
    // V_barco = V_viento * cos(theta) * k
    float theta = m_barco->getAnguloVela() - m_dirViento;
    float speed = VEL_VIENTO_MAX * static_cast<float>(qCos(theta)) * K_EFICIENCIA;
    speed = qMax(0.0f, speed); // No va hacia atrás

    float vx = speed * static_cast<float>(qCos(m_barco->getAnguloVela()));
    float vy = speed * static_cast<float>(qSin(m_barco->getAnguloVela()));
    m_barco->setVelocidad(vx, vy);
}

void Nivel1Archipielago::aplicarCorrientes() {
    if (!m_barco) return;
    float bx = m_barco->getX();
    float by = m_barco->getY();

    for (Corriente* c : m_corrientes) {
        if (c && c->contiene(bx, by)) {
            // V_total = V_barco + V_corriente
            m_barco->setVelocidad(
                m_barco->getVX() + c->getDirX() * 50.0f,
                m_barco->getVY() + c->getDirY() * 50.0f
            );
        }
    }
}

void Nivel1Archipielago::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Left:  m_izqPresionada = true; break;
        case Qt::Key_Right: m_derPresionada = true; break;
        default: break;
    }
}

void Nivel1Archipielago::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Left:  m_izqPresionada = false; break;
        case Qt::Key_Right: m_derPresionada = false; break;
        default: break;
    }
}
