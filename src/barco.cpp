#include "barco.h"
#include <QPainter>
#include <QtMath>

Barco::Barco(float x, float y, QGraphicsItem* parent)
    : Entidad(x, y, parent)
    , m_salud(SALUD_MAX)
    , m_velocidad(0.0f)
    , m_anguloVela(0.0f)
    , m_esBorracho(true)   // siempre activo según el diseño
{
    m_ancho = 80.0f;
    m_alto  = 80.0f;
}

void Barco::paint(QPainter* painter,
                  const QStyleOptionGraphicsItem*,
                  QWidget*)
{
    if (!m_sprite.isNull()) {
        painter->drawPixmap(0, 0,
                            static_cast<int>(m_ancho),
                            static_cast<int>(m_alto),
                            m_sprite);
    } else {
        // Placeholder hasta tener sprites
        painter->setBrush(QColor(139, 90, 43));
        painter->setPen(Qt::NoPen);
        painter->drawRect(boundingRect());
        painter->setPen(Qt::white);
        painter->drawText(boundingRect(), Qt::AlignCenter, "BARCO");
    }
}

void Barco::actualizar(float deltaTiempo) {
    if (!m_activo) return;

    float nx = static_cast<float>(pos().x()) + m_vx * deltaTiempo;
    float ny = static_cast<float>(pos().y()) + m_vy * deltaTiempo;
    setPos(nx, ny);
}

void Barco::alColisionar(Entidad* otro) {
    Q_UNUSED(otro);
    // La lógica de daño la delega el Nivel; aquí solo se puede
    // agregar feedback visual (parpadeo, etc.)
}

void Barco::setAnguloVela(float angulo) {
    m_anguloVela = angulo;
}

void Barco::subirBarco() {
    m_vy = -VEL_SUBIR_BAJAR;
}

void Barco::bajarBarco() {
    m_vy = VEL_SUBIR_BAJAR;
}

int   Barco::getSalud()      const { return m_salud; }
float Barco::getVelocidad()  const { return m_velocidad; }
float Barco::getAnguloVela() const { return m_anguloVela; }
bool  Barco::esBorracho()    const { return m_esBorracho; }

void Barco::recibirDanio(int cantidad) {
    m_salud = qMax(0, m_salud - cantidad);
    if (m_salud == 0) setActivo(false);
}

void Barco::setSalud(int salud) {
    m_salud = qBound(0, salud, SALUD_MAX);
}

void Barco::setEsBorracho(bool borracho) {
    m_esBorracho = borracho;
}

void Barco::setSprite(const QPixmap& pixmap) {
    m_sprite = pixmap;
    update();
}
