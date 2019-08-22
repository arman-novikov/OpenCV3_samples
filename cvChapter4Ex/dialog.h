#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <thread>
#include "drawlines.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
private:
    QLineEdit *numInput;
    QLabel *imgLbl;
    QVBoxLayout *mainLay;
    QPushButton *saveBtn;
    cv::Mat img;
    void lblDraw(const cv::Mat &source);
    void draw0(cv::Mat &dest, int offset);
    void draw1(cv::Mat &dest, int offset);
    void draw2(cv::Mat &dest, int offset);
    void draw3(cv::Mat &dest, int offset);
    void draw4(cv::Mat &dest, int offset);
    void draw5(cv::Mat &dest, int offset);
    void draw6(cv::Mat &dest, int offset);
    void draw7(cv::Mat &dest, int offset);
    void draw8(cv::Mat &dest, int offset);
    void draw9(cv::Mat &dest, int offset);
private slots:
    void numInputChanged(const QString& inp);
    void saveBtnClicked();
};

#endif // DIALOG_H
