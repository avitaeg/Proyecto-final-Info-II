#ifndef JUEGO_H
#define JUEGO_H

#include <QObject>
#include "tipos.h"
#include "nivel.h"
#include "barco.h"
#include "capitanbarbarroja.h"

// ── Juego: coordinador de alto nivel ──────────────────────────────────────────
// Crea y gestiona los niveles, el capitán y lleva la puntuación global.

class Juego : public QObject {
    Q_OBJECT
public:
    explicit Juego(QObject* parent = nullptr);
    ~Juego();

    void iniciar();
    void irANivel(int numeroNivel);

    Nivel*             getNivelActual() const;
    CapitanBarbarroja* getJugador()     const;
    int                getPuntuacion()  const;
    EstadoJuego        getEstado()      const;

signals:
    void estadoCambiado(EstadoJuego nuevoEstado);
    void puntuacionActualizada(int puntuacion);

private slots:
    void onNivel1Completado();
    void onNivel1Fallido();
    void onNivel2Completado();
    void onNivel2Fallido();

private:
    void crearJugador();
    void sumarPuntuacion(int cantidad);

    Nivel*             m_nivelActual;
    Barco*             m_barco;
    CapitanBarbarroja* m_jugador;

    int         m_puntuacion;
    EstadoJuego m_estado;
};

#endif // JUEGO_H
