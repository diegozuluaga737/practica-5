#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene1();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scene1()
{
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->closeButton->setVisible(false);
    ui->pushButton->setVisible(false);

    this->setGeometry(700,100,770,912);
    this->setMinimumSize(width(),height());
    this->setMaximumSize(width(),height());
    scene = new QGraphicsScene();
    scene->setBackgroundBrush(QPixmap(":/imagenes/inicio.png").scaled(width()-20,height()-30));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(width()-20,height()-30);
    ui->graphicsView->setSceneRect(0,0,width()-20,height()-30);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->playButton->setGeometry(250,500,277,107);
}

void MainWindow::scene2()
{
    //escena
    this->setGeometry(700,100,770,912);
    scene->setBackgroundBrush(QPixmap(":/imagenes/fondo_prueba.png"));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(width()-20,height()-30);
    ui->graphicsView->setSceneRect(0,0,width()-20,height()-30);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //pacman
    pacman = new Personajes();
    scene->addItem(pacman);
    pacman->setPos(pacman->getPosx(),pacman->getPosy());

    //laberinto
    ConstruirMuro();

    //monedas
    ConstruirMonedas();

    //puntaje
    score = new Score();
    scene->addItem(score);

    //salud
    health = new Health();
    scene->addItem(health);
    health->setY(health->y()+25);
    //sonido
    sound = new QMediaPlayer();
    sound->setMedia(QUrl("qrc:/musica/kart-mario.mp3"));
    sound->play();
    timerM = new QTimer();
    connect(timerM,SIGNAL(timeout()),this,SLOT(Music()));
    timerM->start(30000);

    //fantasmas
    CrearFantasmas();
    timerG = new QTimer();
    connect(timerG,SIGNAL(timeout()),this,SLOT(moveGhosts()));
    connect(timerG,SIGNAL(timeout()),this,SLOT(move()));
    connect(timerG,SIGNAL(timeout()),this,SLOT(ChocarFantasma()));
    timerG->start(40);

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(startTimerG()));
}

void MainWindow::scene3()
{
    scene->setBackgroundBrush(QPixmap(":/imagenes/end.png"));
    ui->label->setVisible(true);
    ui->label->setGeometry(150,0,500,400);
    ui->label_2->setVisible(true);
    ui->label_2->setText(QString::number(score->getScore()));
    ui->label_2->setGeometry(300,250,400,200);
    ui->pushButton->setVisible(true);
    ui->pushButton->setGeometry(100,600,250,100);
    ui->closeButton->setVisible(true);
    ui->closeButton->setGeometry(400,600,250,100);

}

void MainWindow::on_playButton_clicked()
{
    ui->playButton->setVisible(false);
    scene2();
}

void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    if(evento->key() == Qt::Key_A) key = "A";
    else if(evento->key() == Qt::Key_D) key = "D";
    else if(evento->key() == Qt::Key_W) key = "W";
    else if(evento->key() == Qt::Key_S) key = "S";
}

void MainWindow::move()
{
    if(key == "A" && pacman->x()>=20){
        pacman->setRotation(180);
        pacman->Left();
        if(Chocar()) pacman->Right();
        ComerMonedas();
    }
    if(key == "D" && pacman->x()<=730){
        pacman->setRotation(0);
        pacman->Right();
        if(Chocar()) pacman->Left();
        ComerMonedas();
    }
    if(key == "W" && pacman->y()>=20){
        pacman->setRotation(270);
        pacman->Up();
        if(Chocar()) pacman->Down();
        ComerMonedas();
    }
    if(key == "S" && pacman->y()<=860){
        pacman->setRotation(90);
        pacman->Down();
        if(Chocar()) pacman->Up();
        ComerMonedas();
    }
}

void MainWindow::CrearFantasmas()
{
    Enemy *enemy = new Enemy(":/imagenes/red_ghost.png",20,250);
    ghosts.push_back(enemy);
    enemy = new Enemy(":/imagenes/pink_ghost.png",730,250);
    ghosts.push_back(enemy);
    enemy = new Enemy(":/imagenes/blue_ghost.png",20,600);
    ghosts.push_back(enemy);
    enemy = new Enemy(":/imagenes/orange_ghost.png",730,600);
    ghosts.push_back(enemy);

    for(Enemy *i : ghosts){
        scene->addItem(i);

    }

}

void MainWindow::ChocarFantasma()
{
    for(Enemy *i : ghosts){
        if(pacman->collidesWithItem(i)){

            key = " ";
            health->decrease();
            pacman->DeadPacman();
            //pacman->setPos(pacman->getPosx(),pacman->getPosy());
            if(health->getHealth()==0){
                scene3();
            }
            timerG->stop();
            timer->start(2000);

        }
    }
}

void MainWindow::moveGhosts()
{
    for(short pos=0;pos<2;pos++){
        if(ghosts[pos]->x() < pacman->x()){
            ghosts[pos]->Right();
        }
        else if(ghosts[pos]->x() > pacman->x()){
            ghosts[pos]->Left();
        }
        else if(ghosts[pos]->y() < pacman->y()){
            ghosts[pos]->Down();
        }
        else if(ghosts[pos]->y() > pacman->y()){
            ghosts[pos]->Up();
        }
    }
}

void MainWindow::ConstruirMonedas()
{
    //total de monedas 160
    float posx=22,posy=30;
    Monedas *moneda;
    for(int i=0;i<181;){
       if(!ComprobarMuro(posx,posy)){
       moneda = new Monedas(posx,posy);
       coins.push_back(moneda);
       i++;
       }
       posx += 44.5;
       if(posx>734){
           posx=22;
           posy+=44;
       }
    }

    for(Monedas *i : coins){
        scene->addItem(i);
       i->setPos(i->getPosx(),i->getPosy());
    }
}

void MainWindow::ComerMonedas()
{
    int pos = 0;
    if(coins.size()==0){
        scene3();
    }
    for(Monedas *i : coins){
        if(pacman->collidesWithItem(i)){
            scene->removeItem(i);
            delete i;
            coins.removeAt(pos);
            score->increase();
            break;
        }
        pos ++;
    }
}

bool MainWindow::ComprobarMuro(float posx, float posy)
{
    if(posy ==30 or posy ==162 or posy==602 or posy==866){
       if(posx==378) return true;
    }
    else if(posy == 74 or posy == 118){
        if(posx==66.5 or posx==111 or posx==200 or posx==244.5 or posx==289 or posx==378
        or posx==467 or posx==511.5 or posx==556 or posx==645 or posx==689.5) return true;
    }
    else if(posy == 206){
        if(posx==66.5 or posx==111 or posx==200 or posx==289 or posx==333.5 or posx==378
        or posx==422.5 or posx==467 or posx==556 or posx==645 or posx==689.5) return true;
    }
    else if(posy == 250){
        if(posx==200 or posx==378 or posx==556) return true;
    }
    else if(posy == 294){
        if(posx==22 or posx==66.5 or posx==111 or posx==200 or posx==244.5 or posx==289
        or posx==378 or posx==467 or posx==511.5 or posx==556 or posx==645 or posx==689.5
        or posx==734)return true;
    }
    else if(posy == 338){
        if(posx==22 or posx==66.5 or posx==111 or posx==200 or posx==556 or posx==645
        or posx==689.5 or posx==734)return true;
    }
    else if(posy == 382 or posy == 470 or posy == 558){
        if(posx==22 or posx==66.5 or posx==111 or posx==200 or posx==289 or posx==333.5
        or posx==378 or posx==422.5 or posx==467 or posx==556 or posx==645 or posx==689.5
        or posx==734)return true;
    }
    else if(posy == 426){
        if(posx==22 or posx==66.5 or posx==111 or posx==289 or posx==333.5 or posx==378
        or posx==422.5 or posx==467 or posx==645 or posx==689.5 or posx==734) return true;
    }
    else if(posy == 514){
        if(posx==22 or posx==66.5 or posx==111 or posx==200 or posx==556 or posx==645
        or posx==689.5 or posx==734)return true;
    }
    else if(posy == 646){
        if(posx==66.5 or posx==111 or posx==200 or posx==244.5 or posx==289 or posx==378
        or posx==467 or posx==511.5 or posx==556 or posx==645 or posx==689.5)return true;
    }
    else if(posy == 690){
        if(posx==111 or posx==645) return true;
    }
    else if(posy == 734){
        if(posx==22 or posx==111 or posx==200  or posx==289 or posx==333.5 or posx==378
        or posx==422.5 or posx==467 or posx==556 or posx==645 or posx==734) return true;
    }
    else if(posy == 778){
        if( posx==200 or posx == 378  or posx==556) return true;
    }
    else if(posy == 822){
        if(posx==66.5 or posx==111 or posx==155.5 or posx==200 or posx==244.5 or posx==289
        or posx==378 or posx==467 or posx==511.5 or posx==556 or posx==600.5
        or posx==645 or posx==689.5)return true;
    }
    return false;
}

void MainWindow::ConstruirMuro()
{
    laberinto *pared;

        string texto="",linea="";
        fstream lectura;
        lectura.open("coordenadas.txt",fstream::in|fstream::binary);

        while(lectura.good()){
            linea=lectura.get();
            texto.append(linea);
        }
        lectura.close();
        texto.pop_back();

        int pos = 0;

       for(int num=1;pos!=-1;num++){
           string w,h,x,y;
           pos = texto.find(',');
           w = texto.substr(0,pos);
           pos = texto.find(',',pos+1);
           h = texto.substr(0,pos);
           h = h.substr(w.size()+1);
           pos=texto.find(',',pos+1);
           x = texto.substr(0,pos);
           x = x.substr(w.size()+h.size()+2);
           pos = texto.find('\r');
           y = texto.substr(0,pos);
           y = y.substr(x.size()+w.size()+h.size()+3);

           pos = texto.find('\n');
           texto = texto.substr(pos+1);

           int a = atoi(w.c_str()),b = atoi(h.c_str()),c = atoi(x.c_str()), d = atoi(y.c_str());

           pared=new laberinto(a,b,c,d);
           muros.push_back(pared);

       }
       for(laberinto *i : muros){
           scene->addItem(i);

    }

}


bool MainWindow::Chocar()
{
    for(laberinto *i : muros){ if (pacman->collidesWithItem(i)) return true;}

    return false;

}

void MainWindow::on_closeButton_clicked()
{
    close();
}

void MainWindow::Music()
{
    sound->play();
}

void MainWindow::startTimerG()
{
    timerG->start(40);
    timer->stop();
    //pacman->setPos(370,865);
}
