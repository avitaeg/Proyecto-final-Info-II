#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QGraphicsItem>
#include <QPainter>

// ── Clase base abstracta para todos los objetos del mundo ─────────────────────
// Barco, Obstáculo, Item y AgenteKraken heredan de esta clase.
// Hereda de QGraphicsItem para poder dibujarse en QGraphicsScene.

class Entidad : public QGraphicsItem {
public:
    explicit Entidad(float x, float y, QGraphicsItem* parent = nullptr);
    virtual ~Entidad() = default;

    // ── Interfaz obligatoria de QGraphicsItem ──
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override = 0;

    // ── Lógica de juego ──
    virtual void actualizar(float deltaTiempo) = 0;  // game loop
    virtual void alColisionar(Entidad* otro);         // respuesta a colisión

    // ── Getters / Setters ──
    float getX() const;
    float getY() const;
    float getVX() const;
    float getVY() const;
    bool  isActivo() const;

    void setVelocidad(float vx, float vy);
    void setActivo(bool estado);

protected:
    float m_vx;      // velocidad en X
    float m_vy;      // velocidad en Y
    bool  m_activo;

    // Dimensiones del bounding rect (cada subclase las ajusta)
    float m_ancho;
    float m_alto;
};

#endif // ENTIDAD_H
