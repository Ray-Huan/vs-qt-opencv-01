#include "vsqtopencv01.h"

vsqtopencv01::vsqtopencv01(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    
    init();
    ui.open_image->setText("open_image");
    connect(ui.open_image, &QPushButton::clicked, this, &vsqtopencv01::open_img_clicked);
    ui.open_video->setText("open_video");
    connect(ui.open_video, &QPushButton::clicked, this, &vsqtopencv01::open_video_clicked);
    ui.open_camera->setText("open_camera");
    connect(ui.open_camera, &QPushButton::clicked, this, &vsqtopencv01::open_camera_clicked);
    ui.identify->setText("identify");
    connect(ui.identify, &QPushButton::clicked, this, &vsqtopencv01::identify_clicked);
    ui.takepic->setText("takepic");
}

vsqtopencv01::~vsqtopencv01()
{}

void vsqtopencv01::open_img_clicked()
{
    //============================================================
    QString face_cascade_name = "C:\\Download\\OfficeSoftware\\OpenCV\\opencv-4.9.0-windows\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
    QString eyes_cascade_name = "C:\\Download\\OfficeSoftware\\OpenCV\\opencv-4.9.0-windows\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml";
    QString mouth_cascade_name = "C:\\Download\\OfficeSoftware\\OpenCV\\opencv-4.9.0-windows\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_mouth.xml";

    //加载脸、眼、嘴分类器
    cv::CascadeClassifier faceCC;
    cv::CascadeClassifier eyesCC;
    cv::CascadeClassifier mouthCC;
    if (!faceCC.load(face_cascade_name.toStdString())) return;
    if (!eyesCC.load(eyes_cascade_name.toStdString())) return;
    if (!mouthCC.load(mouth_cascade_name.toStdString())) return;
    //============================================================

    QString imgfilepath = QFileDialog::getOpenFileName(this);
    int len = imgfilepath.size();
    QString str;
    for(int i = 0; i < len; i++)
    {
        if (imgfilepath[i] == '\\')
        {
            str += '\\';
        }
        str += imgfilepath[i];
    }
    if (!imgfilepath.isEmpty())
    {
        cv::Mat img = cv::imread(str.toStdString(), 1);
        //cv::imshow("1", img);

        faceRecongize(faceCC, eyesCC, mouthCC, img);

        QImage image = Mat2QImage(img);
        QImage result = image.scaled(ui.video_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui.video_label->setPixmap(QPixmap::fromImage(result));
        qDebug() << "图片打开成功！";
    }
    else
    {
        qDebug() << "图片打开失败！";
    }
}

void vsqtopencv01::init()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &vsqtopencv01::readFarme);
}


QImage vsqtopencv01::Mat2QImage(const cv::Mat& mat)
{
    if (mat.type() == CV_8UC1)
    {
        QVector<QRgb> colorTable;
        for (int i = 0; i <= 255; i++)
            colorTable.push_back(qRgb(i, i, i));
        const uchar* qImageBuffer = (const uchar*)mat.data;
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    if (mat.type() == CV_8UC3)
    {
        const uchar* qImageBuffer = (const uchar*)mat.data;
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return  img.rgbSwapped();
    }
    else
    {
        return QImage();
    }
}

void vsqtopencv01::faceRecongize(cv::CascadeClassifier faceCC, cv::CascadeClassifier eyesCC, cv::CascadeClassifier mouthCC, cv::Mat frame)
{

    std::vector<cv::Rect> peo;

    //检测人脸
    faceCC.detectMultiScale(frame, peo, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(32, 32));
    for (int i = 0; i < peo.size(); i++)
    {
        // 用椭圆画出人脸部分
        cv::Point center(peo[i].x + peo[i].width / 2, peo[i].y + peo[i].height / 2);
        ellipse(frame, center, cv::Size(peo[i].width / 2, peo[i].height / 2), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);

        cv::Mat faceROI = frame(peo[i]);
        std::vector<cv::Rect> eyes;

        // 检测眼睛
        eyesCC.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        for (int j = 0; j < eyes.size(); j++)
        {
            // 用圆画出眼睛部分
            cv::Point eye_center(peo[i].x + eyes[j].x + eyes[j].width / 2, peo[i].y + eyes[j].y + eyes[j].height / 2);
            int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
            circle(frame, eye_center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
        }

        cv::Mat mouthROI = frame(peo[i]);
        std::vector<cv::Rect> mouth;

        // 检测嘴部
        mouthCC.detectMultiScale(mouthROI, mouth, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        for (int k = 0; k < mouth.size(); k++)
        {
            //用长方形画出嘴部
            cv::Rect rect(peo[i].x + mouth[k].x, peo[i].y + mouth[k].y, mouth[k].width, mouth[k].height);
            rectangle(frame, rect, cv::Scalar(0, 255, 0), 2, 8, 0);
        }

        // 检测到两个眼睛和一个嘴巴, 可认为检测到有效人脸
        if (eyes.size() >= 2 && mouth.size() >= 1) {

            // 人脸上方区域写字进行标识
            cv::Point centerText(peo[i].x + peo[i].width / 2 - 40, peo[i].y - 20);
            cv::putText(frame, "face", centerText, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        }
    }
}

void vsqtopencv01::open_video_clicked()
{
    QString imgfilepath = QFileDialog::getOpenFileName(this);
    int len = imgfilepath.size();
    QString str;
    for (int i = 0; i < len; i++)
    {
        if (imgfilepath[i] == '\\')
        {
            str += '\\';
        }
        str += imgfilepath[i];
    }
    if (!imgfilepath.isEmpty())
    {
        cv::VideoCapture cap;
        cap.open(str.toStdString());
        if (!cap.isOpened())
        {
            qDebug() << "视频加载失败！";
            return;
        }
        //int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        //int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        //int totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);
        //int frameRate = cap.get(cv::CAP_PROP_FPS);

        cv::Mat frame;
        while (1)
        {
            cap >> frame;
            if (frame.empty())
                break;
            //cv::imshow("video", frame);

            //===============================

            // 1. 定义HOG对象
            cv::HOGDescriptor hog; // 采用默认参数


            // 2. 设置SVM分类器
            hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());   // 采用已经训练好的行人检测分类器

            // 3. 在测试图像上检测行人区域
            std::vector<cv::Rect> regions;
            hog.detectMultiScale(frame, regions, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 1);

            // 显示
            for (size_t i = 0; i < regions.size(); i++)
            {
                cv::rectangle(frame, regions[i], cv::Scalar(0, 0, 255), 2);
            }

            //===============================

            QImage image = Mat2QImage(frame);
            QImage result = image.scaled(ui.video_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui.video_label->setPixmap(QPixmap::fromImage(result));
            cv::waitKey(0);
        }
        cap.release();
        qDebug() << "视频加载成功！";
    }
    else
    {
        qDebug() << "视频加载失败！";
    }
}

void vsqtopencv01::open_camera_clicked()
{
    if (!Switch)
    {
        cam.open(0);//打开摄像头
        ui.open_camera->setText("camera_off");
        Switch = true;
        timer->start(10);//开始计时

        connect(ui.takepic, &QPushButton::clicked, this, &vsqtopencv01::takepic);
    }
    else
    {
        cam.release();//关闭摄像头
        ui.open_camera->setText("camera_on");
        Switch = false;
    }
}

void vsqtopencv01::readFarme()
{
    cv::Mat frame;
    cam >> frame;

    //===============================

        // 1. 定义HOG对象
        cv::HOGDescriptor hog; // 采用默认参数


        // 2. 设置SVM分类器
        hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());   // 采用已经训练好的行人检测分类器

        // 3. 在测试图像上检测行人区域
        std::vector<cv::Rect> regions;
        hog.detectMultiScale(frame, regions, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 1);

        // 显示
        for (size_t i = 0; i < regions.size(); i++)
        {
            cv::rectangle(frame, regions[i], cv::Scalar(0, 0, 255), 2);
        }

        IMG = Mat2QImage(frame);
        cv::waitKey(20);

        //===============================

    ui.video_label->setPixmap(QPixmap::fromImage(IMG));
}


void vsqtopencv01::takepic()
{
    IMG.save("C:\\MYPROJECT\\image_lib\\1.jpg");
}


void vsqtopencv01::identify_clicked()//识别图片
{
    QString imgfilepath = QFileDialog::getOpenFileName(this);
    int len = imgfilepath.size();
    QString str;
    for (int i = 0; i < len; i++)
    {
        if (imgfilepath[i] == '\\')
        {
            str += '\\';
        }
        str += imgfilepath[i];
    }
    if (!imgfilepath.isEmpty())
    {
        qDebug() << "图片打开成功！";
    }
    else
    {
        qDebug() << "图片打开失败！";
    }

    cv::Mat image = cv::imread(str.toStdString(), 1);
    if (image.empty())
    {
        std::cout << "read image failed" << std::endl;
    }


    //// 1. 定义HOG对象  
    //cv::HOGDescriptor hog; // 采用默认参数  


    //// 2. 设置SVM分类器  
    //hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());   // 采用已经训练好的行人检测分类器  

    //// 3. 在测试图像上检测行人区域  
    //std::vector<cv::Rect> regions;
    //hog.detectMultiScale(image, regions, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 1);

    //// 显示  
    //for (size_t i = 0; i < regions.size(); i++)
    //{
    //    cv::rectangle(image, regions[i], cv::Scalar(0, 0, 255), 2);
    //}

    ////cv::imshow("hog", image);
    //QImage imag = Mat2QImage(image);
    //QImage result = imag.scaled(ui.video_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //ui.video_label->setPixmap(QPixmap::fromImage(result));
    //cv::waitKey(0);

    //=================================================
    cv::Mat img;
    cv::cvtColor(image, img, cv::COLOR_BGRA2BGR);
    cv::HOGDescriptor hdesc;
    hdesc.setSVMDetector(hdesc.getDefaultPeopleDetector());
    std::vector<cv::Rect> peo;
    hdesc.detectMultiScale(img, peo, 0, cv::Size(8, 8));
    for (size_t i = 0; i < peo.size(); i++)
    {
        cv::rectangle(image, peo[i], cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }
    QImage imag = Mat2QImage(image);
    QImage result = imag.scaled(ui.video_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.video_label->setPixmap(QPixmap::fromImage(result));
    cv::waitKey(0);
}
