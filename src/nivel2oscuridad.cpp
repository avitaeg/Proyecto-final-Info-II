#include "nivel2oscuridad.h"
#include <QRandomGenerator>
#include <QtMath>

Nivel2Oscuridad::Nivel2Oscuridad(Barco* barco,
                                 CapitanBarbarroja* capitan,
                                 QObject* parent)
    : Nivel("El Mar de la Oscuridad", TIEMPO_NIVEL2, parent)
    , m_barco(barco), m_capitan(capitan)
    , m_kraken(nullptr), m_agente(nullptr), m_faro(nullptr)
    , m_distanciaRecorrida(0.0f)
    , m_tiempoSpawnObjeto(0.0f)
    , m_tiempoOla(0.0f)
    , m_amplitudOla(A_OLA_INICIAL)
    , m_frecuenciaOla(F_OLA_INICIAL)
    , m_arribaPresionada(false), m_abajoPresionada(false)
{
    m_escena->setSceneRect(0, 0, 20000, 720);
    m_escena->setBackgroundBrush(QColor(5, 5, 20)); // noche casi negra
}

void Nivel2Oscuridad::configurarEscena() {
    // Barco en el extremo izquierdo, centrado verticalmente
    m_barco->setPos(150, 300);
    m_escena->addItem(m_barco);

    // Kraken y su agente
    m_kraken = new Kraken(600, 400);
    m_agente  = new AgenteKraken(m_kraken, m_barco);
    agregarEntidad(m_kraken);
    // AgenteKraken no se dibuja, pero necesita actualizarse
    m_entidades.push_back(m_agente);

    generarOlas();
    colocarFaro();
}

void Nivel2Oscuridad::generarOlas() {
    for (int i = 0; i < 8; ++i) {
        float x = 300.0f + i * 400.0f;
        float y = 350.0f;
        float amp  = 30.0f + QRandomGenerator::global()->bounded(40);
        float freq = 0.3f  + QRandomGenerator::global()->bounded(10) * 0.05f;
        auto* ola = new OlaGigante(x, y, amp, freq);
        agregarEntidad(ola);
    }
}

void Nivel2Oscuridad::colocarFaro() {
    m_faro = new Item(DISTANCIA_FARO, 350, "Faro");
    m_faro->setActivo(false); // aparece cuando se supera la distancia umbral
    agregarEntidad(m_faro);
}

void Nivel2Oscuridad::procesarEntrada() {
    if (m_arribaPresionada) m_capitan->subirBarco();
    if (m_abajoPresionada)  m_capitan->bajarBarco();
    if (!m_arribaPresionada && !m_abajoPresionada) {
        m_barco->setVelocidad(VEL_AVANCE, 0.0f); // solo avance horizontal
    }
}

void Nivel2Oscuridad::verificarCondiciones() {
    // Derrota: barco destruido
    if (!m_barco->isActivo()) {
        m_activo = false;
        m_timer->stop();
        emit nivelFallido();
        return;
    }

    // Mostrar faro al superar distancia umbral
    if (m_distanciaRecorrida >= DISTANCIA_FARO && m_faro && !m_faro->isActivo()) {
        m_faro->setActivo(true);
    }

    // Victoria: llegó al faro
    if (m_faro && m_faro->fueRecogido()) {
        m_activo = false;
        m_timer->stop();
        emit nivelCompletado();
    }

    // La tormenta se intensifica con el tiempo
    m_amplitudOla   = A_OLA_INICIAL + (TIEMPO_NIVEL2 - m_tiempoRestante) * 0.05f;
    m_frecuenciaOla = F_OLA_INICIAL + (TIEMPO_NIVEL2 - m_tiempoRestante) * 0.002f;

    // Spawn de objetos voladores
    m_tiempoSpawnObjeto += DELTA_TIEMPO;
    if (m_tiempoSpawnObjeto >= INTERVALO_SPAWN) {
        spawnObjetoVolador();
        m_tiempoSpawnObjeto = 0.0f;
    }

    // Avance del barco
    m_distanciaRecorrida += VEL_AVANCE * DELTA_TIEMPO;
}

void Nivel2Oscuridad::spawnObjetoVolador() {
    float bx = m_barco->getX() + RADIO_LINTERNA * 1.1f; // fuera del radio de visión
    float by = -50.0f;
    float vx = -30.0f + QRandomGenerator::global()->bounded(60) - 30.0f;
    float vy = 50.0f;

    TipoObjeto tipo = static_cast<TipoObjeto>(QRandomGenerator::global()->bounded(3));
    auto* obj = new ObjetoVolador(bx, by, vx, vy, tipo);
    agregarEntidad(obj);
}

void Nivel2Oscuridad::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Up:   m_arribaPresionada = true;  break;
        case Qt::Key_Down: m_abajoPresionada  = true;  break;
        default: break;
    }
}

void Nivel2Oscuridad::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Up:   m_arribaPresionada = false; break;
        case Qt::Key_Down: m_abajoPresionada  = false; break;
        default: break;
    }
}
