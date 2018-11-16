#include "widget.h"
#include "ui_widget.h"
#include<QMouseEvent>
#include<opencv2/opencv.hpp>
#include<QFileDialog>
#include "QDebug"
#include<unistd.h>
#include<ctime>
#include <QMessageBox>
#include<QPainter>

using namespace cv;
using namespace std;


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(speedup_play()) );
    connect(ui->horizontalSlider,SIGNAL(sliderReleased()),this,SLOT(change_play_process())  );
    connect(ui->horizontalSlider,SIGNAL(sliderPressed()),this,SLOT(press_slider())  );
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(save_image()) );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    string str;
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("open video");
    fileDialog->setDirectory("/home/ansheng/mp4video");
//    fileDialog->setNameFilter(tr("(*.png *.jpg *.jpeg *.bmp)"));
    fileDialog->setNameFilter(tr("(*.mp4 *.mkv)"));
    /*fileDialog->setFileMode(QFileDialog::ExistingFiles);*/// This mode can select many files;
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileName;
    if(fileDialog->exec())
      {
         fileName = fileDialog->selectedFiles();
      }
//    for(auto tmp:fileName)
//            qDebug()<<tmp<<endl;
//    qDebug()<<fileName.at(0)<<endl;
    videoname = fileName.at(0).toStdString();
    cap = VideoCapture(videoname);
    cap>>play_frame;
    frame_length = cap.get(CV_CAP_PROP_FRAME_COUNT);
    frame_position = 1;
    ui->horizontalSlider->setMaximum(frame_length-3);
    ui->horizontalSlider->setMinimum(1);
    ui->horizontalSlider->setValue(frame_position);
}



void Widget::on_pushButton_2_clicked()
{
    playorpause = !playorpause;
    QImage img;
    QPixmap qpix;
    int height = ui->label->height();
    int width = ui->label->width();
//    ui->label->resize(1280,720);

        while(play_frame.data && playorpause)
        {
            frame_position += 1;
            ui->label_2->setText(QString::number(frame_position, 10));
            ui->horizontalSlider->setValue(frame_position);
            cap>>play_frame;
//            cap.read(play_frame);
            cvtColor(play_frame,play_frame,CV_BGR2RGB);
            img = QImage((const unsigned char*)(play_frame.data),play_frame.cols,play_frame.rows,play_frame.cols*play_frame.channels(),QImage::Format_RGB888);
            qpix = QPixmap::fromImage(img);

            /*qpix = qpix.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);*///饱满填充
            qpix = qpix.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  //按比例填充
            ui->label->setPixmap(qpix);
            usleep(waittime);
            waitKey(5);

        }

}
void Widget::speedup_play()
{
    if(speed<10)
    {
        speed = speed+1;
        waittime = waittime/speed;
    }
}
void Widget::press_slider()
{
    playorpause = !playorpause;
}

void Widget::change_play_process()
{
    frame_position = ui->horizontalSlider->value();
    ui->label_3->setText(QString::number(frame_position,10));
    cap.set(CV_CAP_PROP_POS_FRAMES,frame_position);
    ui->pushButton_2->click();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    int e_x,e_y;
    int left_x,left_y;
    int label_width,label_height;
    int right_x,right_y;
    left_x = ui->label->pos().x();
    left_y = ui->label->pos().y();
    label_width = ui->label->width();
    label_height = ui->label->height();
    right_x = left_x + label_width;
    right_y = left_y + label_height;
    if(event->button()==Qt::LeftButton)
    {
        e_x = event->x();
        e_y = event->y();
        if(e_x>left_x && e_x<right_x && e_y>left_y && e_y<right_y)
        {
            origin_x=event->x();
            origin_y=event->y();
            playorpause =! playorpause;
        }
    }

}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    wid=event->x()-origin_x;
    hei=event->y()-origin_y;
    ui->label_5->setText(QString::number(wid));
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
   QRect rect;
   rect.setRect(origin_x+1,origin_y+1,abs(wid)-1,abs(hei)-1);
   grapPix= this->grab(rect);//截取矩形区域内图像
   QPixmap grap = grapPix.scaled(wid, hei, Qt::KeepAspectRatio, Qt::SmoothTransformation);
   ui->label_4->setPixmap(grap);
}

void Widget::save_image()
{
    time_t t = time(0);
    QString qstr = QString::number(t,10);
    string str = qstr.toStdString();
    str=str+".PNG";

    str="/home/ansheng/Desktop/qt/screenfromvideo/images/"+str;
     cout<<str<<endl;
     qstr = QString::fromStdString(str);
    grapPix.save(qstr,"PNG");
    QMessageBox::information(this, QString::fromLocal8Bit("information"),QString::fromLocal8Bit("imageSaved"));
}

//void Widget::paintEvent(QPaintEvent *event)
//{
//        QPainter painter(this);
//        painter.setPen(QPen(Qt::red,5));
//        painter.drawRect(QRect(10,10,100,100));
//}

