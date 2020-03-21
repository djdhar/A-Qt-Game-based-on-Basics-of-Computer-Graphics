#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
int pixelsize = 9;
QVector<QPair<int,int> >PolygonPoints;
QVector<QPair<int,int> >Base;
QVector<QPair<int,int> >PolygonPoints_for_reflection;
QVector<QPair<int,int> >Window;
QColor edgeColor(255,255,255);
#define size_of_the_frame 900
#define PI 3.14159
QImage GlobalImg;
QString msg("You Win!!!");
void plotLineLow( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1);
void plotLineHigh( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1);
void Bresenham( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1);

bool Checker(QImage &sourceImage){
    bool checker=false;
    for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
        int a=PolygonPoints_for_reflection[i].first;
        int b=PolygonPoints_for_reflection[i].second;
        int x=PolygonPoints_for_reflection[i+1].first;
        int y=PolygonPoints_for_reflection[i+1].second;
        bool dect=true;
        QVector<QPair<int,int> >qV;
        Bresenham(qV,a,b,x,y);

        for(int j=0;j<qV.size();j=j+pixelsize){
            int lx,ly,rx,ry,tx,ty,bx,by,c1x,c1y,c2x,c2y,c3x,c3y,c4x,c4y;
            int xx=qV[j].first;
            int yy=qV[j].second;
            xx=(xx/pixelsize)*pixelsize+(pixelsize/2);
            yy=(yy/pixelsize)*pixelsize+(pixelsize/2);
            lx=xx-pixelsize;
            ly=yy;
            rx=xx+pixelsize;
            ry=yy;
            tx=xx;
            ty=yy-pixelsize;
            bx=xx;
            by=yy+pixelsize;
            c1x=xx+pixelsize;
            c1y=yy+pixelsize;
            c2x=xx+pixelsize;
            c2y=yy-pixelsize;
            c3x=xx-pixelsize;
            c3y=yy+pixelsize;
            c4x=xx-pixelsize;
            c4y=yy-pixelsize;

            if(sourceImage.pixelColor(lx,ly)==QColor(0,255,255) || sourceImage.pixelColor(rx,ry)==QColor(0,255,255) || sourceImage.pixelColor(tx,ty)==QColor(0,255,255) || sourceImage.pixelColor(bx,by)==QColor(0,255,255) /* || sourceImage.pixelColor(c1x,c1y)==QColor(0,255,255) || sourceImage.pixelColor(c2x,c2y)==QColor(0,255,255) || sourceImage.pixelColor(c3x,c3y)==QColor(0,255,255) || sourceImage.pixelColor(c4x,c4y)==QColor(0,255,255)*/){
                dect=dect&true;
            }
            else{
                dect=dect&false;
            }
        }
        checker=checker|dect;
    }
    return checker;
}
using namespace std;
double rad(int deg){
    return (PI/180)*deg;
}

void plotLineLow( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1){
    int dx=x1-x0;
    int dy=y1-y0;
    int yi=1;
    if(dy<0){
        yi=-1;
        dy=-dy;
    }
    int D=2*dy-dx;
    int y=y0;
    for(int x=x0;x<=x1;x=x+1){
        v.push_back(qMakePair(x,y));
        if(D>0){
            y=y+yi;
            D=D-2*dx;
        }
        D=D+2*dy;
    }

}
 void plotLineHigh( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1){
     int dx=x1-x0;
     int dy=y1-y0;
     int xi=1;
     if(dx<0){
         xi=-1;
         dx=-dx;
     }
     int D=2*dx-dy;
     int x=x0;
     for(int y=y0;y<=y1;y=y+1){
         v.push_back(qMakePair(x,y));
         if(D>0){
             x=x+xi;
             D=D-2*dy;
         }
         D=D+2*dx;
     }
 }

 void Bresenham( QVector<QPair<int,int> > &v,int x0,int y0, int x1,int y1){
     if(abs(y1-y0)<abs(x1-x0)){
         if(x0>x1)
             plotLineLow(v,x1,y1,x0,y0);
         else {
             plotLineLow(v,x0,y0,x1,y1);
         }
     }
     else{
         if(y0>y1)
             plotLineHigh(v,x1,y1,x0,y0);
         else {
             plotLineHigh(v,x0,y0,x1,y1);
         }
     }
 }

 int RotX(int x, int y,int x1, int y1, int deg=45){
     return x1+(x-x1)*cos(rad(deg))-(y-y1)*sin(rad(deg));
 }
 int RotY(int y, int x,int x1, int y1, int deg=45){
     return y1+(x-x1)*sin(rad(deg))+(y-y1)*cos((rad(deg)));
 }

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
    ui->base->adjustSize();
    //ui->label->adjustSize();
    ui->poly->adjustSize();
    ui->pushButton->adjustSize();
    ui->pushButton_2->adjustSize();
    ui->pushButton_3->adjustSize();
    ui->pushButton_4->adjustSize();
    ui->pushButton_5->adjustSize();
    ui->pushButton_6->adjustSize();
    ui->pushButton_7->adjustSize();
    ui->pushButton_8->adjustSize();
    ui->pushButton_9->adjustSize();
    ui->pushButton_10->adjustSize();

    ui->up->adjustSize();
    ui->up_2->adjustSize();

    ClearScreen();
}
void MainWindow::ClearScreen(void){
    QImage img=QImage(size_of_the_frame,size_of_the_frame,QImage::Format_RGB888);
        img.fill(QColor(Qt::black).rgb());

        QPainter painter( &img );
        QPen pen(Qt::red, 1);
        painter.setPen(pen);
        for(int i=0;i<size_of_the_frame;i=i+pixelsize)
            painter.drawLine(0,i,size_of_the_frame-1,i);
        for(int i=0;i<size_of_the_frame;i=i+pixelsize)
            painter.drawLine(i,0,i,size_of_the_frame-1);

        painter.end();
         ui->frame->setPixmap(QPixmap::fromImage(img));
         sourceImage=img;
}
void MainWindow::keyPressEvent(QKeyEvent *ev){
    //ARROW DOWN
    if(ev->key()==16777237){
        on_pushButton_3_clicked();
    }

       //ARROW RIGHT
    else if(ev->key()==16777236){
        on_pushButton_4_clicked();
    }
    //ARROW UP
    else if(ev->key()==16777235){
        on_up_clicked();
    }
    //ARROW LEFT
    else if(ev->key()==16777234){
        on_up_2_clicked();
    }
    else {

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::point(QImage&img,int x,int y,int r, int red=255, int green=255, int blue=255)
{

    int left=x/pixelsize;
    int right=y/pixelsize;
    for(int i=left*pixelsize+1;i<(left+1)*pixelsize;i++){
        for(int j=right*pixelsize+1;j<(right+1)*pixelsize;j++){
            img.setPixel(i,j,qRgb(red,green,blue));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
    sourceImage=img;
}

void MainWindow::showMousePosition(QPoint &pos)
{
    //ui->mouse_movement->setText(" X : "+QString::number(pos.x())+", Y : "+QString::number(pos.y()));
}
void MainWindow::Mouse_Pressed()
{
   // ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x)+", Y : "+QString::number(ui->frame->y));
    point(sourceImage,ui->frame->x,ui->frame->y,3,0,255,0);
    p1.setX((((int)ui->frame->x)/pixelsize)*pixelsize+pixelsize/2);
    p1.setY((((int)ui->frame->y)/pixelsize)*pixelsize+pixelsize/2);
    if(ui->poly->isChecked())
    PolygonPoints.push_back(qMakePair(p1.x(),p1.y()));
    if(ui->base->isChecked())
           Base.push_back(qMakePair(p1.x(),p1.y()));
}






void MainWindow::on_pushButton_clicked()
{
    ClearScreen();
    if(ui->poly->isChecked()){

        for(int i=0;i<Base.size()-1;i++){
            QVector<QPair<int,int>> qP;
            Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
            for(int j=0;j<qP.size();j=j+pixelsize)
                point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
        }

    PolygonPoints.push_back(PolygonPoints[0]);

    for(int i=0;i<PolygonPoints.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,PolygonPoints[i].first,PolygonPoints[i].second,PolygonPoints[i+1].first,PolygonPoints[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,0,255);
    }
    PolygonPoints_for_reflection=PolygonPoints;
    PolygonPoints.clear();
   // ui->poly->setAutoExclusive(false);
    ui->poly->setChecked(0);
    }
    if(ui->base->isChecked()){
        Base.push_back(Base[0]);

        for(int i=0;i<Base.size()-1;i++){
            QVector<QPair<int,int>> qP;
            Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
            for(int j=0;j<qP.size();j=j+pixelsize)
                point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
        }
      //  ui->base->setAutoExclusive(false);
        ui->base->setChecked(0);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ClearScreen();
    Base.clear();
    PolygonPoints.clear();
    PolygonPoints_for_reflection.clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,PolygonPoints_for_reflection[i].first+pixelsize,PolygonPoints_for_reflection[i].second,PolygonPoints_for_reflection[i+1].first+pixelsize,PolygonPoints_for_reflection[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,0,255);
        PolygonPoints_for_reflection[i].first=PolygonPoints_for_reflection[i].first+pixelsize;
    }
     if(PolygonPoints_for_reflection.size()>=1)
    PolygonPoints_for_reflection[PolygonPoints_for_reflection.size()-1].first+=pixelsize;
     if(Checker(sourceImage)==true){
         ui->label->setText(msg);
     }
     else {
         ui->label->setText("");
     }
}

void MainWindow::on_up_2_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,PolygonPoints_for_reflection[i].first-pixelsize,PolygonPoints_for_reflection[i].second,PolygonPoints_for_reflection[i+1].first-pixelsize,PolygonPoints_for_reflection[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,0,255);
        PolygonPoints_for_reflection[i].first=PolygonPoints_for_reflection[i].first-pixelsize;
    }
     if(PolygonPoints_for_reflection.size()>=1)
    PolygonPoints_for_reflection[PolygonPoints_for_reflection.size()-1].first-=pixelsize;
     if(Checker(sourceImage)==true){
         ui->label->setText(msg);
     }
     else {
         ui->label->setText("");
     }
}

void MainWindow::on_up_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,PolygonPoints_for_reflection[i].first,PolygonPoints_for_reflection[i].second-pixelsize,PolygonPoints_for_reflection[i+1].first,PolygonPoints_for_reflection[i+1].second-pixelsize);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,0,255);
        PolygonPoints_for_reflection[i].second-=pixelsize;
    }
     if(PolygonPoints_for_reflection.size()>=1)
    PolygonPoints_for_reflection[PolygonPoints_for_reflection.size()-1].second-=pixelsize;
     if(Checker(sourceImage)==true){
         ui->label->setText(msg);
     }
     else {
         ui->label->setText("");
     }
}

void MainWindow::on_pushButton_3_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,PolygonPoints_for_reflection[i].first,PolygonPoints_for_reflection[i].second+pixelsize,PolygonPoints_for_reflection[i+1].first,PolygonPoints_for_reflection[i+1].second+pixelsize);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,0,255);
        PolygonPoints_for_reflection[i].second+=pixelsize;
    }
    if(PolygonPoints_for_reflection.size()>=1)
    PolygonPoints_for_reflection[PolygonPoints_for_reflection.size()-1].second+=pixelsize;
    if(Checker(sourceImage)==true){
        ui->label->setText(msg);
    }
    else {
        ui->label->setText("");
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    QVector<QPair<int,int> > temp;
    int deg=5;
    int totx=0;
    int toty=0;
    int x1=0,y1=0;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        totx=totx+PolygonPoints_for_reflection[i].first;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        toty=toty+PolygonPoints_for_reflection[i].second;
    try{
         x1=totx/PolygonPoints_for_reflection.size();
        y1=toty/PolygonPoints_for_reflection.size();
    }
    catch(exception e){

    }

    if(PolygonPoints_for_reflection.size()!=0){
            int a=PolygonPoints_for_reflection[0].first;
            int b=PolygonPoints_for_reflection[0].second;
            temp.push_back(qMakePair(RotX(a,b, x1, y1,deg),RotY(b,a,x1,  y1,deg)));
    }

            for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
            int a=PolygonPoints_for_reflection[i].first;
            int b=PolygonPoints_for_reflection[i].second;

            int _a=PolygonPoints_for_reflection[i+1].first;
            int _b=PolygonPoints_for_reflection[i+1].second;
             QVector<QPair<int,int> > v;
            Bresenham(v, RotX(a,b, x1, y1,deg),RotY(b,a,x1,  y1,deg),RotX(_a,_b, x1, y1,deg),RotY(_b,_a, x1,  y1,deg));
            temp.push_back(qMakePair(RotX(_a,_b, x1, y1,deg),RotY(_b,_a,x1,  y1,deg)));
            for(int i=0;i<v.size();i=i+pixelsize){
                int a_=v[i].first;
                int b_=v[i].second;

                //if(Window[Window.size()-2].first<a && Window[Window.size()-2].second<b && Window[Window.size()-1].first>a && Window[Window.size()-1].second>b)
                point(sourceImage,a_,b_,3,0,0,255);
                cout<<a_<<" "<<b_<<endl;
            }
            }
            PolygonPoints_for_reflection=temp;
            if(Checker(sourceImage)==true){
                ui->label->setText(msg);
            }
            else {
                ui->label->setText("");
            }
}

void MainWindow::on_pushButton_6_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    QVector<QPair<int,int> > temp;
    int deg=-5;
    int totx=0;
    int toty=0;
    int x1=0,y1=0;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        totx=totx+PolygonPoints_for_reflection[i].first;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        toty=toty+PolygonPoints_for_reflection[i].second;
    try{
         x1=totx/PolygonPoints_for_reflection.size();
        y1=toty/PolygonPoints_for_reflection.size();
    }
    catch(exception e){

    }

    if(PolygonPoints_for_reflection.size()!=0){
            int a=PolygonPoints_for_reflection[0].first;
            int b=PolygonPoints_for_reflection[0].second;
            temp.push_back(qMakePair(RotX(a,b, x1, y1,deg),RotY(b,a,x1,  y1,deg)));
    }

            for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
            int a=PolygonPoints_for_reflection[i].first;
            int b=PolygonPoints_for_reflection[i].second;

            int _a=PolygonPoints_for_reflection[i+1].first;
            int _b=PolygonPoints_for_reflection[i+1].second;
             QVector<QPair<int,int> > v;
            Bresenham(v, RotX(a,b, x1, y1,deg),RotY(b,a,x1,  y1,deg),RotX(_a,_b, x1, y1,deg),RotY(_b,_a, x1,  y1,deg));
            temp.push_back(qMakePair(RotX(_a,_b, x1, y1,deg),RotY(_b,_a,x1,  y1,deg)));
            for(int i=0;i<v.size();i=i+pixelsize){
                int a_=v[i].first;
                int b_=v[i].second;

                //if(Window[Window.size()-2].first<a && Window[Window.size()-2].second<b && Window[Window.size()-1].first>a && Window[Window.size()-1].second>b)
                point(sourceImage,a_,b_,3,0,0,255);
                cout<<a_<<" "<<b_<<endl;
            }
            }
            PolygonPoints_for_reflection=temp;
            if(Checker(sourceImage)==true){
                ui->label->setText(msg);
            }
            else {
                ui->label->setText("");
            }
}

void MainWindow::on_pushButton_7_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    QVector<QPair<int,int> > temp;
    int deg=5;
    int totx=0;
    int toty=0;
    int x1=0,y1=0;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        totx=totx+PolygonPoints_for_reflection[i].first;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        toty=toty+PolygonPoints_for_reflection[i].second;
    try{
         x1=totx/PolygonPoints_for_reflection.size();
        y1=toty/PolygonPoints_for_reflection.size();
    }
    catch(exception e){

    }
    for(int i=0;i<PolygonPoints_for_reflection.size();i++){
        if(PolygonPoints_for_reflection[i].first>=x1){
            PolygonPoints_for_reflection[i].first+=pixelsize;
        }
        else {
            PolygonPoints_for_reflection[i].first-=pixelsize;
        }
    }
    for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,PolygonPoints_for_reflection[i].first,PolygonPoints_for_reflection[i].second,PolygonPoints_for_reflection[i+1].first,PolygonPoints_for_reflection[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,0,255);
    }
            if(Checker(sourceImage)==true){
                ui->label->setText(msg);
            }
            else {
                ui->label->setText("");
            }
}

void MainWindow::on_pushButton_10_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    QVector<QPair<int,int> > temp;
    int deg=5;
    int totx=0;
    int toty=0;
    int x1=0,y1=0;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        totx=totx+PolygonPoints_for_reflection[i].first;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        toty=toty+PolygonPoints_for_reflection[i].second;
    try{
         x1=totx/PolygonPoints_for_reflection.size();
        y1=toty/PolygonPoints_for_reflection.size();
    }
    catch(exception e){

    }
    for(int i=0;i<PolygonPoints_for_reflection.size();i++){
        if(PolygonPoints_for_reflection[i].first>=x1){
            PolygonPoints_for_reflection[i].first-=pixelsize;
        }
        else {
            PolygonPoints_for_reflection[i].first+=pixelsize;
        }
    }
    for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,PolygonPoints_for_reflection[i].first,PolygonPoints_for_reflection[i].second,PolygonPoints_for_reflection[i+1].first,PolygonPoints_for_reflection[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,0,255);
    }
            if(Checker(sourceImage)==true){
                ui->label->setText(msg);
            }
            else {
                ui->label->setText("");
            }
}

void MainWindow::on_pushButton_8_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    QVector<QPair<int,int> > temp;
    int deg=5;
    int totx=0;
    int toty=0;
    int x1=0,y1=0;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        totx=totx+PolygonPoints_for_reflection[i].first;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        toty=toty+PolygonPoints_for_reflection[i].second;
    try{
         x1=totx/PolygonPoints_for_reflection.size();
        y1=toty/PolygonPoints_for_reflection.size();
    }
    catch(exception e){

    }
    for(int i=0;i<PolygonPoints_for_reflection.size();i++){
        if(PolygonPoints_for_reflection[i].second>=y1){
            PolygonPoints_for_reflection[i].second+=pixelsize;
        }
        else {
            PolygonPoints_for_reflection[i].second-=pixelsize;
        }
    }
    for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,PolygonPoints_for_reflection[i].first,PolygonPoints_for_reflection[i].second,PolygonPoints_for_reflection[i+1].first,PolygonPoints_for_reflection[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,0,255);
    }
            if(Checker(sourceImage)==true){
                ui->label->setText(msg);
            }
            else {
                ui->label->setText("");
            }
}

void MainWindow::on_pushButton_9_clicked()
{
    ClearScreen();
    for(int i=0;i<Base.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,Base[i].first,Base[i].second,Base[i+1].first,Base[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,255,255);
    }
    QVector<QPair<int,int> > temp;
    int deg=5;
    int totx=0;
    int toty=0;
    int x1=0,y1=0;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        totx=totx+PolygonPoints_for_reflection[i].first;
    for(int i=0;i<PolygonPoints_for_reflection.size();i++)
        toty=toty+PolygonPoints_for_reflection[i].second;
    try{
         x1=totx/PolygonPoints_for_reflection.size();
        y1=toty/PolygonPoints_for_reflection.size();
    }
    catch(exception e){

    }
    for(int i=0;i<PolygonPoints_for_reflection.size();i++){
        if(PolygonPoints_for_reflection[i].second>=y1){
            PolygonPoints_for_reflection[i].second-=pixelsize;
        }
        else {
            PolygonPoints_for_reflection[i].second+=pixelsize;
        }
    }
    for(int i=0;i<PolygonPoints_for_reflection.size()-1;i++){
        QVector<QPair<int,int>> qP;
        Bresenham(qP,PolygonPoints_for_reflection[i].first,PolygonPoints_for_reflection[i].second,PolygonPoints_for_reflection[i+1].first,PolygonPoints_for_reflection[i+1].second);
        for(int j=0;j<qP.size();j=j+pixelsize)
            point(sourceImage,qP[j].first,qP[j].second,3,0,0,255);
    }
            if(Checker(sourceImage)==true){
                ui->label->setText(msg);
            }
            else {
                ui->label->setText("");
            }
}
