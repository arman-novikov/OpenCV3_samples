#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <thread>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "facewidgets.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
private:
    void setImg(const cv::Mat &source);
    void drawImg();
    cv::Mat img;
    QLabel *imgLbl;
    FaceWidget *face;
    IFaceFeathure *eyes, *nose, *lips;
    QVBoxLayout *settingsLay;
    QHBoxLayout *mainLay;
    int faceHeight, faceWidth;
    int eyesVPOS, eyeHeight, eyeWidth;
    int noseVPOS, noseHeight, noseWidth;
    int lipsVPOS, lipsHeight, lipsWidth;
private slots:
    void setNewValues();
};

#endif // DIALOG_H
