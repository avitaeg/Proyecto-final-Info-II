#include "juego.h"
#include "nivel1archipielago.h"
#include "nivel2oscuridad.h"

Juego::Juego(QObject* parent)
    : QObject(parent)
    , m_nivelActual(nullptr)
    , m_barco(nullptr)
    , m_jugador(nullptr)
    , m_puntuacion(0)
    , m_estado(EstadoJuego::MENU)
{}

Juego::~Juego() {
    delete m_nivelActual;
    delete m_jugador;
    delete m_barco;
}

void Juego::iniciar() {
    crearJugador();
    irANivel(1);
}

void Juego::crearJugador() {
    delete m_jugador;
    delete m_barco;

    m_barco  = new Barco(400, 300);
    m_jugador = new CapitanBarbarroja(m_barco, this);
}

void Juego::irANivel(int numeroNivel) {
    delete m_nivelActual;
    m_nivelActual = nullptr;

    if (numeroNivel == 1) {
        auto* n1 = new Nivel1Archipielago(m_barco, m_jugador, this);
        connect(n1, &Nivel::nivelCompletado, this, &Juego::onNivel1Completado);
        connect(n1, &Nivel::nivelFallido,    this, &Juego::onNivel1Fallido);
        m_nivelActual = n1;
        m_estado = EstadoJuego::NIVEL1;

    } else if (numeroNivel == 2) {
        auto* n2 = new Nivel2Oscuridad(m_barco, m_jugador, this);
        connect(n2, &Nivel::nivelCompletado, this, &Juego::onNivel2Completado);
        connect(n2, &Nivel::nivelFallido,    this, &Juego::onNivel2Fallido);
        m_nivelActual = n2;
        m_estado = EstadoJuego::NIVEL2;
    }

    if (m_nivelActual) m_nivelActual->iniciar();
    emit estadoCambiado(m_estado);
}

void Juego::onNivel1Completado() {
    sumarPuntuacion(500);
    irANivel(2);
}

void Juego::onNivel1Fallido() {
    m_estado = EstadoJuego::GAME_OVER;
    emit estadoCambiado(m_estado);
}

void Juego::onNivel2Completado() {
    sumarPuntuacion(1000);
    m_estado = EstadoJuego::VICTORIA;
    emit estadoCambiado(m_estado);
}

void Juego::onNivel2Fallido() {
    m_estado = EstadoJuego::GAME_OVER;
    emit estadoCambiado(m_estado);
}

void Juego::sumarPuntuacion(int cantidad) {
    m_puntuacion += cantidad;
    emit puntuacionActualizada(m_puntuacion);
}

Nivel*             Juego::getNivelActual() const { return m_nivelActual; }
CapitanBarbarroja* Juego::getJugador()     const { return m_jugador; }
int                Juego::getPuntuacion()  const { return m_puntuacion; }
EstadoJuego        Juego::getEstado()      const { return m_estado; }
