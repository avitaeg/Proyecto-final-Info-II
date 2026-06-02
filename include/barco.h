#ifndef BARCO_H
#define BARCO_H

#include "entidad.h"
#include <QPixmap>

// ── Barco: entidad principal controlada por el jugador ────────────────────────
// Nivel 1: se mueve en 2D cenital con físicas eólicas.
// Nivel 2: se mueve verticalmente en vista lateral.

class Barco : public Entidad {
public:
    explicit Barco(float x, float y, QGraphicsItem* parent = nullptr);

    // ── QGraphicsItem ──
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    // ── Game loop ──
    void actualizar(float deltaTiempo) override;
    void alColisionar(Entidad* otro) override;

    // ── Control del jugador ──
    void setAnguloVela(float angulo);   // en radianes
    void subirBarco();                  // nivel 2
    void bajarBarco();                  // nivel 2

    // ── Estado ──
    int   getSalud()      const;
    float getVelocidad()  const;
    float getAnguloVela() const;
    bool  esBorracho()    const;

    void recibirDanio(int cantidad);
    void setSalud(int salud);
    void setEsBorracho(bool borracho);

    // ── Sprite ──
    void setSprite(const QPixmap& pixmap);

private:
    int   m_salud;
    float m_velocidad;
    float m_anguloVela;    // ángulo de velas respecto al viento
    bool  m_esBorracho;    // activa oscilación lateral

    QPixmap m_sprite;

    static constexpr int   SALUD_MAX      = 100;
    static constexpr float VEL_SUBIR_BAJAR = 120.0f;  // px/seg nivel 2
};

#endif // BARCO_H
