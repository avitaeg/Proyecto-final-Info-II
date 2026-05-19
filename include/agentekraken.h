#ifndef AGENTEKRAKEN_H
#define AGENTEKRAKEN_H

#include "entidad.h"
#include "kraken.h"
#include "barco.h"
#include "tipos.h"
#include <map>
#include <vector>
#include <QString>

// ── AgenteKraken: agente inteligente con percepción, razonamiento y aprendizaje
// Ciclo: Percibir → Razonar → Actuar → Aprender

class AgenteKraken : public Entidad {
public:
    explicit AgenteKraken(Kraken* kraken, Barco* barco,
                          QGraphicsItem* parent = nullptr);

    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void actualizar(float deltaTiempo) override;

    float getAgresividad() const;

private:
    // ── Percepción ──────────────────────────────────────────────────────────
    void percibir();          // registra posición y dirección de evasión del barco

    // ── Razonamiento ────────────────────────────────────────────────────────
    // Decide dónde lanzar el próximo tentáculo
    void razonar();

    // ── Acción ──────────────────────────────────────────────────────────────
    void actuar();

    // ── Aprendizaje ─────────────────────────────────────────────────────────
    // Registra si el ataque impactó o no y ajusta probabilidades
    void aprender(bool impacto);

    // ── Estado interno ──────────────────────────────────────────────────────
    Kraken* m_kraken;
    Barco*  m_barco;

    float m_agresividad;           // sube con el tiempo
    float m_tiempoEntreAtaques;    // segundos entre ataques
    float m_tiempoDesdeUltimoAtaque;

    // Historial de evasiones: "arriba" o "abajo" → conteo
    std::map<QString, int> m_historial;

    // Posición anterior del barco para detectar dirección de evasión
    float m_yBarcoAnterior;

    // Decisión actual
    bool  m_atacarArriba;

    static constexpr float AGRESIVIDAD_INICIAL  = 0.3f;
    static constexpr float INCREMENTO_AGRESIVIDAD = 0.01f; // por ataque
    static constexpr float TIEMPO_BASE_ATAQUE   = 3.0f;   // segundos
};

#endif // AGENTEKRAKEN_H
