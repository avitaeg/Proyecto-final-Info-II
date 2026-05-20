#ifndef TIPOS_H
#define TIPOS_H

// ── Enumeraciones globales del juego ──────────────────────────────────────────

enum class TipoObjeto {
    BARRIL,
    MADERO,
    RAYO
};

enum class EstadoKraken {
    ATACAR,
    RETROCEDER
};

enum class EstadoJuego {
    MENU,
    NIVEL1,
    NIVEL2,
    GAME_OVER,
    VICTORIA
};

#endif // TIPOS_H
