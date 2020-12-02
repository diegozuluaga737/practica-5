#include "personajes.h"

Personajes::Personajes(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    timerD = new QTimer();
    filas = 0;
    columnas = 0;
    pixmap = new QPixmap(":/imagenes/PACMAN (2).png");

    posy = 865;
    posx = 370;
    velocidad =5;

    //dimensiones de la imagen
     ancho = 31.25;
     alto = 28;
     timer->start(120);
     connect(timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
     connect(timerD,SIGNAL(timeout()),this,SLOT(Morir()));

}

int Personajes::getPosx() const
{
    return posx;
}

void Personajes::setPosx(int value)
{
    posx = value;
}

int Personajes::getPosy() const
{
    return posy;
}

void Personajes::setPosy(int value)
{
    posy = value;
}

void Personajes::DeadPacman()
{
    timer->stop();
    delete pixmap;
    pixmap = new QPixmap(":/imagenes/dead_pacman.png");
    ancho = 31;
    timerD->start(200);
}

void Personajes::AlivePacman()
{
    timerD->stop();
    delete pixmap;
    pixmap = new QPixmap(":/imagenes/PACMAN (2).png");
    ancho = 31.25;
    timer->start(120);
    posx = 370;
    posy = 865;
    this->setPos(370,865);
}

QRectF Personajes::boundingRect() const
{
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void Personajes::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    option = nullptr;
    widget = nullptr;
    painter->drawPixmap(-ancho/2,-alto/2,*pixmap,columnas,0,ancho,alto);
}

void Personajes::Up()
{
    posy -= velocidad;
    setPos(posx,posy);
}

void Personajes::Down()
{
    posy += velocidad;
    setPos(posx,posy);
}

void Personajes::Left()
{
    posx -= velocidad;
    setPos(posx,posy);
}

void Personajes::Right()
{
    posx += velocidad;
    setPos(posx,posy);
}

void Personajes::Actualizacion()
{
    columnas += 31.25;
    if(columnas >= 125){
        columnas = 0;
    }
    this->update(-ancho/2,-alto/2,ancho,alto);
}

void Personajes::Morir()
{
    columnas += 31;
    if(columnas > 310){
        AlivePacman();
    }

    this->update(-ancho/2,-alto/2,ancho,alto);
}
