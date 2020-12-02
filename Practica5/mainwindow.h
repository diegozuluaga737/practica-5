#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRectF>
#include <QDebug>
#include <QGraphicsScene>
#include <QKeyEvent>//recibir teclas
#include "personajes.h"
#include "monedas.h"
#include "laberinto.h"
#include "score.h"
#include "enemy.h"
#include <QList>
#include <QMediaPlayer>
#include "health.h"
#include <fstream>
#include <string>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_playButton_clicked();
    void on_closeButton_clicked();
    void moveGhosts();
    void move();
    void ChocarFantasma();
    void Music();
    void startTimerG();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    QString key;
    Personajes *pacman;
    Monedas *moneda;
    QList<Monedas *> coins;
    QList< laberinto *> muros;
    laberinto *muro1;
    Score *score;
    Health *health;
    QList<Enemy *> ghosts;
    QTimer *timerG;
    QTimer *timerM;
    QTimer *timer;

    //escenas
    void scene1();
    void scene2();
    void scene3();
    void scene4();

    //movimiento
    void keyPressEvent(QKeyEvent *evento);

    //paredes
    void ConstruirMuro();
    bool Chocar();

    //monedas
    void ConstruirMonedas();
    void ComerMonedas();
    bool ComprobarMuro(float posx,float posy);

    //fantasmas
    void CrearFantasmas();
    //sonido
    QMediaPlayer *sound;

};
#endif // MAINWINDOW_H
