#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QThread>

int fd;
int dx1;
int dy1;
int dx2;
int dy2;
int pc=0;
int snakex[10];
int snakey[10];
int snakelength = 4;
int direction = 1;
int fruitx = 2;
int fruity = 2;
QTimer *timer;
QTimer *snake_timer;
QTimer *fruit_timer;
QTimer *snake_print;
class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fd = open("/dev/LED_Matrix", 0);

    //ui->setupUi(this);
    timer = new QTimer(this);
    snake_timer=new QTimer(this);
    fruit_timer=new QTimer(this);
    snake_print=new QTimer(this);


    connect(ui->pushButton, SIGNAL (clicked()), this, SLOT (on_pushButton_clicked()));
    connect(ui->pushButton_2, SIGNAL (clicked()), this, SLOT (on_pushButton_2_clicked()));
    connect(ui->pushButton_3, SIGNAL (clicked()), this, SLOT (on_pushButton_3_clicked()));
    connect(ui->pushButton_4, SIGNAL (clicked()), this, SLOT (on_pushButton_4_clicked()));
    snakex[0]=3;
    snakex[1]=2;
    snakey[0]=0;
    snakey[1]=0;
    snakex[2]=1;
    snakex[3]=0;
    snakey[2]=0;
    snakey[3]=0;


    connect(snake_timer, SIGNAL(timeout()), this, SLOT(snake_timer_expiry()));
    int period = 200;//rand()%2000;
    snake_timer->setInterval(period);
    snake_timer->start();

    connect(fruit_timer, SIGNAL(timeout()), this, SLOT(fruit_timer_expiry()));
    int period_fruit = 100;//rand()%2000;
    fruit_timer->setInterval(period_fruit);
    fruit_timer->start();

    connect(snake_print, SIGNAL(timeout()), this, SLOT(snake_print_expiry()));
    int period_snake_print = 10;//rand()%2000;
    snake_print->setInterval(period_snake_print);
    snake_print->start();

    for(int dc=0;dc<snakelength;dc++)
    {
        ioctl(fd,snakex[dc],snakey[dc]);Sleeper::usleep(1);
    }


}

MainWindow::~MainWindow()
{


    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    direction = 1;
    snake_timer_expiry();
}

void MainWindow::on_pushButton_2_clicked()
{
    direction = 2;
    snake_timer_expiry();
}

void MainWindow::on_pushButton_4_clicked()
{
    direction = 3;
    snake_timer_expiry();
}

void MainWindow::on_pushButton_3_clicked()
{
    direction = 4;
    snake_timer_expiry();
}

void MainWindow::fruit_timer_expiry()
{
    ioctl(fd,fruitx,fruity);
    Sleeper::usleep(1);
    if(snakex[0]==fruitx && snakey[0]==fruity)
    {
        snakelength++;
        fruitx=rand()%7;
        fruity=rand()%15;
        int cc=1;
        while(cc ==1)
        {
            for(int c=0;c<snakelength;c++)
            {
                if(snakex[c]==fruitx && snakey[c]==fruity)
                {
                    fruitx=rand()%7;
                    fruity=rand()%15;
                }
            }
            cc=2;
            for(int c=0;c<snakelength;c++)
            {
                if(snakex[c]==fruitx || snakey[c]==fruity)
                {
                    cc=1;
                }
            }
        }
    }
}

void MainWindow::snake_print_expiry()
{
    //for(int dc=0;dc<snakelength;dc++)
    if (pc<snakelength)
    {
        ioctl(fd,snakex[pc],snakey[pc]);Sleeper::usleep(1);
        pc++;
    }
    else
    {
        pc=0;
    }

}

void MainWindow::snake_timer_expiry()
{

        /*for(int x=0;x<8;x++)
        {
            for(int y=0;y<16;y++)
            {

                ioctl(fd,x,y);Sleeper::usleep(10);
                //ioctl(fd,x,y);Sleeper::usleep(1);
                //ioctl(fd,x,y);Sleeper::usleep(100);
            }
        }*/
        if(direction==1)
        {
            dx1=snakex[0];
            dy1=snakey[0];
            for(int dc=1;dc<snakelength;dc++)
            {
                dx2=snakex[dc];
                dy2=snakey[dc];
                snakex[dc]=dx1;
                snakey[dc]=dy1;
                dx1=dx2;
                dy1=dy2;

            }
            snakex[0]++;
            if(snakex[0]==8)
            {
                snakex[0]=0;
            }

        }
        if(direction==2)
        {

            dx1=snakex[0];
            dy1=snakey[0];
            for(int dc=1;dc<snakelength;dc++)
            {
                dx2=snakex[dc];
                dy2=snakey[dc];
                snakex[dc]=dx1;
                snakey[dc]=dy1;
                dx1=dx2;
                dy1=dy2;

            }
            snakey[0]++;
            if(snakey[0]==16)
            {
                snakey[0]=0;
            }

        }
        if(direction==3)
            {
                dx1=snakex[0];
                dy1=snakey[0];
                for(int dc=1;dc<snakelength;dc++)
                {
                    dx2=snakex[dc];
                    dy2=snakey[dc];
                    snakex[dc]=dx1;
                    snakey[dc]=dy1;
                    dx1=dx2;
                    dy1=dy2;

                }
                snakex[0]--;
                if(snakex[0]==-1)
                {
                    snakex[0]=7;
                }

            }


        if(direction==4)
        {
            dx1=snakex[0];
            dy1=snakey[0];
            for(int dc=1;dc<snakelength;dc++)
            {
                dx2=snakex[dc];
                dy2=snakey[dc];
                snakex[dc]=dx1;
                snakey[dc]=dy1;
                dx1=dx2;
                dy1=dy2;

            }
            snakey[0]--;
            if(snakey[0]==-1)
            {
                snakey[0]=15;
            }

        }
}
