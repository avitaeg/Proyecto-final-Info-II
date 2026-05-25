#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "barco.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //Funciones propias

public slots:
    void bgMove();

private:
    Ui::Widget *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *bg1, *bg2;
    QTimer *bgTimer;
    Barco *barco;
};
#endif // WIDGET_H
