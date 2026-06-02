#include "nivel.h"
#include <algorithm>

Nivel::Nivel(const QString& nombre, float tiempoLimite, QObject* parent)
    : QObject(parent)
    , m_escena(new QGraphicsScene(this))
    , m_timer(new QTimer(this))
    , m_nombre(nombre)
    , m_tiempoRestante(tiempoLimite)
    , m_tiempoLimite(tiempoLimite)
    , m_activo(false)
{
    connect(m_timer, &QTimer::timeout, this, &Nivel::tick);
}

Nivel::~Nivel() {
    m_timer->stop();
    // m_escena se elimina como hijo de QObject
}

void Nivel::iniciar() {
    m_activo = true;
    m_tiempoRestante = m_tiempoLimite;
    configurarEscena();
    m_timer->start(INTERVALO_MS);
}

void Nivel::pausar() {
    m_timer->stop();
}

void Nivel::reanudar() {
    if (m_activo) m_timer->start(INTERVALO_MS);
}

void Nivel::reiniciar() {
    m_timer->stop();
    m_escena->clear();
    m_entidades.clear();
    m_tiempoRestante = m_tiempoLimite;
    iniciar();
}

void Nivel::tick() {
    if (!m_activo) return;

    // Actualizar tiempo
    m_tiempoRestante -= DELTA_TIEMPO;
    emit tiempoActualizado(m_tiempoRestante);

    // Actualizar todas las entidades
    for (Entidad* e : m_entidades) {
        if (e && e->isActivo()) {
            e->actualizar(DELTA_TIEMPO);
        }
    }

    detectarColisiones();
    eliminarEntidadesInactivas();
    verificarCondiciones();
}

void Nivel::agregarEntidad(Entidad* entidad) {
    if (!entidad) return;
    m_entidades.push_back(entidad);
    m_escena->addItem(entidad);
}

void Nivel::eliminarEntidadesInactivas() {
    for (auto it = m_entidades.begin(); it != m_entidades.end(); ) {
        Entidad* e = *it;
        if (e && !e->isActivo()) {
            m_escena->removeItem(e);
            delete e;
            it = m_entidades.erase(it);
        } else {
            ++it;
        }
    }
}

void Nivel::detectarColisiones() {
    // Colisiones simples por bounding rect entre todas las entidades
    for (size_t i = 0; i < m_entidades.size(); ++i) {
        for (size_t j = i + 1; j < m_entidades.size(); ++j) {
            Entidad* a = m_entidades[i];
            Entidad* b = m_entidades[j];
            if (!a || !b || !a->isActivo() || !b->isActivo()) continue;

            if (a->collidesWithItem(b)) {
                a->alColisionar(b);
                b->alColisionar(a);
            }
        }
    }
}

QGraphicsScene* Nivel::getEscena()         const { return m_escena; }
float           Nivel::getTiempoRestante()  const { return m_tiempoRestante; }
QString         Nivel::getNombre()         const { return m_nombre; }
bool            Nivel::estaActivo()        const { return m_activo; }
