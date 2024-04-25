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
    QTimer* timer;//��ʱ��
    QImage IMG;//���ص�ͼƬ
    QCamera* camera;
    cv::VideoCapture cam;
    bool Switch;//����ͷ����
    bool videoSwitch;//��Ƶ���ſ���

    void init();

public:
    QImage Mat2QImage(const cv::Mat& mat);
    void faceRecongize(cv::CascadeClassifier faceCC, cv::CascadeClassifier eyesCC, cv::CascadeClassifier mouthCC, cv::Mat frame);
    

private slots:
    void open_img_clicked();//��ͼƬ
    void open_video_clicked();//����Ƶ
    void open_camera_clicked();//��/�ر�����ͷ
    void readFarme();//��ȡ��ǰ֡��Ϣ
    void takepic();//����
    void identify_clicked();//����ͼƬʶ��

private:
    Ui::vsqtopencv01Class ui;
};
