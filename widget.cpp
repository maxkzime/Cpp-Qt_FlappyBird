#include "widget.h"
#include "ui_widget.h"

#include <QGraphicsPixmapItem>

#include "pillaritem.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene = new Scene(this);
    scene->setSceneRect(-250,-250,500,500);

    QGraphicsPixmapItem * pixItem = new QGraphicsPixmapItem(QPixmap(":/imgs/backgroundElongated.png").scaled(QSize(432, 1536)));

    scene->addItem(pixItem);
    pixItem->setPos(QPointF(0,0) - QPointF(pixItem->boundingRect().width()/2,
                                           pixItem->boundingRect().height()/2));

    //    for( int i = 0; i<= 256;)
    //    {
    //        scene->addLine(-400,-128+i,400,-128+i,QPen(Qt::red));
    //        scene->addLine(-128+i,-400,-128+i,400,QPen(Qt::blue));
    //        i+=16;
    //    }

    scene->addLine(-400,0,400,0,QPen(Qt::red));
    scene->addLine(0,-400,0,400,QPen(Qt::blue));

    scene->addBird();

    ui->graphicsView->setScene(scene);
}


Widget::~Widget()
{delete ui;}


void Widget::on_pb_start_clicked()
{scene->startGame();}

