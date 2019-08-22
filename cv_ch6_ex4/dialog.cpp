#include "dialog.h"
#include <QDebug>

const int imgWIDTH = 400, imgHEIGTH = 400;
const int LINES_THICKNESS = 1;
const cv::Scalar RED(0, 0, 250), BLACK(0,0,0);

const int FACE_HEIGHT_MIN = 100;
const int FACE_HEIGHT_MAX = 300;
const int FACE_WIDTH_MIN = 50;
const int FACE_WIDTH_MAX = 200;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    imgLbl = new QLabel();
        imgLbl->setFixedSize(QSize(imgWIDTH,imgHEIGTH));
    face = new FaceWidget("face", FACE_HEIGHT_MIN, FACE_HEIGHT_MAX, FACE_WIDTH_MIN, FACE_WIDTH_MAX);
    eyes = new IFaceFeathure("eyes", 10, 20, 10, 30, 130, 150);
    nose = new IFaceFeathure("nose", 10, 30, 20, 40, 130, 180);
    lips = new IFaceFeathure("lips", 20, 40, 20, 80, 160, 280);
    settingsLay = new QVBoxLayout();
        settingsLay->addWidget(face);
        settingsLay->addWidget(eyes);
        settingsLay->addWidget(nose);
        settingsLay->addWidget(lips);
    mainLay = new QHBoxLayout(this);
        mainLay->addWidget(imgLbl);
        mainLay->addLayout(settingsLay);

    setNewValues();
    connect(face, SIGNAL(valuesChangedSig()), this, SLOT(setNewValues()));
    connect(eyes, SIGNAL(valuesChangedSig()), this, SLOT(setNewValues()));
    connect(nose, SIGNAL(valuesChangedSig()), this, SLOT(setNewValues()));
    connect(lips, SIGNAL(valuesChangedSig()), this, SLOT(setNewValues()));
}

void Dialog::setNewValues()
{
//qDebug() << face->getHeight() << face->getWidth();
    faceHeight = face->getHeight();
    faceWidth = face->getWidth();
    eyesVPOS = eyes->getVPos();
    eyeHeight = eyes->getHeight();
    eyeWidth = eyes->getWidth();
    noseVPOS = nose->getVPos();
    noseHeight = nose->getHeight();
    noseWidth = nose->getWidth();
    lipsVPOS = lips->getVPos();
    lipsHeight = lips->getHeight();
    lipsWidth = lips->getWidth();

    drawImg();
}

void Dialog::setImg(const cv::Mat &source)
{
    QImage img = QImage(static_cast<const unsigned char*>(source.data),
                             source.cols,
                             source.rows,
                             static_cast<int>(source.step),
                             QImage::Format_RGB888);
        QPixmap pixmap(imgWIDTH, imgHEIGTH);
        pixmap.convertFromImage(img.rgbSwapped());
        imgLbl->setPixmap(pixmap);
}

void Dialog::drawImg()
{
    img = cv::Mat(imgHEIGTH, imgWIDTH, CV_8UC3, BLACK);
    const cv::Point2f center(faceWidth/2 + 80, faceHeight/2 + 80);
    const cv::RotatedRect faceBox(center, cv::Size2f(faceWidth, faceHeight),0);
    cv::ellipse(img, faceBox, RED, LINES_THICKNESS, 4);

    const cv::RotatedRect leftEyeBox(cv::Point2f(center.x - 20, eyesVPOS), cv::Size2f(eyeWidth, eyeHeight),0);
    const cv::RotatedRect rightEyeBox(cv::Point2f(center.x + 20, eyesVPOS), cv::Size2f(eyeWidth, eyeHeight),0);
    cv::ellipse(img, leftEyeBox, RED, LINES_THICKNESS, 4);
    cv::ellipse(img, rightEyeBox, RED, LINES_THICKNESS, 4);

    const cv::Point2f upperNose(center.x, noseVPOS);
    const cv::Point2f leftNose(upperNose.x - noseWidth/2, upperNose.y + noseHeight);
    const cv::Point2f rightNose(center.x + noseWidth/2, upperNose.y + noseHeight);
    cv::line(img, upperNose, leftNose, RED, LINES_THICKNESS, 4);
    cv::line(img, upperNose, rightNose, RED, LINES_THICKNESS, 4);
    cv::line(img, rightNose, leftNose, RED, LINES_THICKNESS, 4);

    const cv::Point2f lipsCenter(center.x, lipsVPOS);
    const cv::RotatedRect lipsBox(lipsCenter, cv::Size2f(lipsWidth, lipsHeight), 0);
    cv::ellipse(img, lipsBox, RED, LINES_THICKNESS, 4);
    cv::Point2f leftLipCorner(lipsCenter.x - lipsWidth/2, lipsCenter.y);
    cv::Point2f righttLipCorner(lipsCenter.x + lipsWidth/2, lipsCenter.y);
    cv::line(img, leftLipCorner, righttLipCorner, RED, LINES_THICKNESS, 4);

    setImg(img);
}

Dialog::~Dialog()
{

}
