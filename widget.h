#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMouseEvent>
#include<QFileDialog>
#include<opencv2/opencv.hpp>
#include<QPainter>

using namespace std;
using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    string videoname;
    int waittime = 30000;
    int speed=1;
    Mat play_frame;
    VideoCapture cap;
    bool playorpause = false;
    int frame_position;
    int frame_length;
    int origin_x,origin_y,wid,hei;
    QPixmap grapPix;

private slots :
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void speedup_play();
    void change_play_process();
    void press_slider();
    void save_image();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *event);
//    void paintEvent(QPaintEvent *event);

};



#endif // WIDGET_H
