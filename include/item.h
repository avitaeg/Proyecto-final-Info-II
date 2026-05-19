#ifndef ITEM_H
#define ITEM_H

#include "entidad.h"
#include <QString>

// ── Item: objeto coleccionable del mapa ───────────────────────────────────────

class Item : public Entidad {
public:
    explicit Item(float x, float y,
                  const QString& nombre,
                  QGraphicsItem* parent = nullptr);

    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void actualizar(float deltaTiempo) override;
    void alColisionar(Entidad* otro) override;

    QString getNombre() const;
    bool    fueRecogido() const;

private:
    QString m_nombre;
    bool    m_recogido;

    float m_tiempoFlotacion; // animación de flotado suave
};

#endif // ITEM_H
