#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QKeyEvent>
#include "juego.h"

// ── MainWindow: ventana principal con stack de pantallas ──────────────────────
// Pantallas: menú → nivel1 → nivel2 → game_over / victoria

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event)   override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void onEstadoCambiado(EstadoJuego estado);
    void onPuntuacionActualizada(int puntuacion);

private:
    void configurarUI();
    void mostrarMenu();
    void mostrarNivel(int numero);
    void mostrarGameOver();
    void mostrarVictoria();

    Juego*          m_juego;
    QStackedWidget* m_stack;
    QGraphicsView*  m_vista;       // vista del nivel activo
    QLabel*         m_labelHUD;    // tiempo y salud
    QLabel*         m_labelPuntos;

    // Índices en el stack
    static constexpr int IDX_MENU     = 0;
    static constexpr int IDX_JUEGO    = 1;
    static constexpr int IDX_GAMEOVER = 2;
    static constexpr int IDX_VICTORIA = 3;
};

#endif // MAINWINDOW_H
