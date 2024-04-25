#pragma once

#include <QtWidgets/QWidget>
#include "ui_vsqtopencv01.h"

#include <qtimer.h>
#include <qcamera.h>
#include <qcameraviewfinder.h>
#include <videoio.hpp>
#include <qfile.h>
#include <qfiledialog.h>
#include <qdebug.h>
#include <qpixmap.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <cstdio>
#include <qevent.h>

class vsqtopencv01 : public QWidget
{
    Q_OBJECT

public:
    vsqtopencv01(QWidget *parent = nullptr);
    ~vsqtopencv01();

private:
    QTimer* timer;//计时器
    QImage IMG;//返回的图片
    QCamera* camera;
    cv::VideoCapture cam;
    bool Switch;//摄像头开关
    bool videoSwitch;//视频播放开关

    void init();

public:
    QImage Mat2QImage(const cv::Mat& mat);
    void faceRecongize(cv::CascadeClassifier faceCC, cv::CascadeClassifier eyesCC, cv::CascadeClassifier mouthCC, cv::Mat frame);
    

private slots:
    void open_img_clicked();//打开图片
    void open_video_clicked();//打开视频
    void open_camera_clicked();//打开/关闭摄像头
    void readFarme();//读取当前帧信息
    void takepic();//拍照
    void identify_clicked();//进行图片识别

private:
    Ui::vsqtopencv01Class ui;
};
