#include "dialog.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    numInput = new QLineEdit();
    saveBtn = new QPushButton("save");
    saveBtn->setFixedSize(50, 30);
    imgLbl = new QLabel();
    imgLbl->setFixedSize(COLUMNS, ROWS);
    this->setFixedSize(300, 150);

    lblDraw(cv::Mat(ROWS, COLUMNS, CV_8UC3, BLACK));

    connect(numInput, SIGNAL(textChanged(const QString&)), this, SLOT(numInputChanged(const QString&)));
    connect(saveBtn, SIGNAL(clicked(bool)), this, SLOT(saveBtnClicked()));
    mainLay = new QVBoxLayout(this);
        mainLay->addWidget(numInput);
        mainLay->addWidget(imgLbl);
        mainLay->addWidget(saveBtn);
}

void Dialog::numInputChanged(const QString& inp)
{
    const int len = inp.size();
    QVector<int> digits;
    digits.reserve(len);
    for (int i = 0; i < len; ++i)
        digits.append(QString(inp[i]).toInt());
    img = cv::Mat(ROWS, COLUMNS, CV_8UC3, BLACK);

    if (digits.empty() || digits.size() > DIGITS_NUMBER - 1) {
        numInput->clear();
        lblDraw(img);
        return;
    }

    for (int i = 0; i < digits.size(); ++i) {
        int ofs = DIGIT_OFFSET * i + DIGIT_SPACING;
        switch (digits[i]) {
        case 0:
            draw0(img, ofs);
            break;
        case 1:
            draw1(img, ofs);
            break;
        case 2:
            draw2(img, ofs);
            break;
        case 3:
            draw3(img, ofs);
            break;
        case 4:
            draw4(img, ofs);
            break;
        case 5:
            draw5(img, ofs);
            break;
        case 6:
            draw6(img, ofs);
            break;
        case 7:
            draw7(img, ofs);
            break;
        case 8:
            draw8(img, ofs);
            break;
        case 9:
            draw9(img, ofs);
            break;
        default:
            return;
        }
    }
    lblDraw(img);

}

void Dialog::lblDraw(const cv::Mat &source)
{
    QImage img = QImage(static_cast<const unsigned char*>(source.data),
                         source.cols,
                         source.rows,
                         static_cast<int>(source.step),
                         QImage::Format_RGB888);
    QPixmap pixmap(COLUMNS, ROWS);
    pixmap.convertFromImage(img.rgbSwapped());
    imgLbl->setPixmap(pixmap);
}

void Dialog::saveBtnClicked()
{
    const QString initialPath = QDir::currentPath() + "/new.jpg";
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save"),
                                                          initialPath,
                                                          tr("Images (*.png *.jpg)"));
    if (fileName.isEmpty())
        return;

    QByteArray ba = fileName.toLocal8Bit();
    char *charFileName = static_cast<char *>(strdup(ba.constData()));

    if(!this->img.empty())
        cv::imwrite(charFileName, this->img);
}

void Dialog::draw0(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(77,22,188);
    drawUpper(dest, offset, color);
    drawRightUpper(dest, offset, color);
    drawLeftUpper(dest, offset, color);
    drawRightBottom(dest, offset, color);
    drawLeftBottom(dest, offset, color);
    drawBottom(dest, offset, color);
}

void Dialog::draw1(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(22,22,220);
    drawRightUpper(dest, offset, color);
    drawRightBottom(dest, offset, color);
}

void Dialog::draw2(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(22,220,22);
    drawCentral(dest, offset, color);
    drawUpper(dest, offset, color);
    drawRightUpper(dest, offset, color);
    drawLeftBottom(dest, offset, color);
    drawBottom(dest, offset, color);
}

void Dialog::draw3(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(22,220,220);
    drawCentral(dest, offset, color);
    drawUpper(dest, offset, color);
    drawRightUpper(dest, offset, color);
    drawRightBottom(dest, offset, color);
    drawBottom(dest, offset, color);
}

void Dialog::draw4(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(220,22,22);
    drawCentral(dest, offset, color);
    drawRightUpper(dest, offset, color);
    drawLeftUpper(dest, offset, color);
    drawRightBottom(dest, offset, color);
}

void Dialog::draw5(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(220,22,220);
    drawCentral(dest, offset, color);
    drawUpper(dest, offset, color);
    drawLeftUpper(dest, offset, color);
    drawRightBottom(dest, offset, color);
    drawBottom(dest, offset, color);
}

void Dialog::draw6(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(220,220,22);
    drawCentral(dest, offset, color);
    drawUpper(dest, offset, color);
    drawLeftUpper(dest, offset, color);
    drawRightBottom(dest, offset, color);
    drawLeftBottom(dest, offset, color);
    drawBottom(dest, offset, color);
}

void Dialog::draw7(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(220,220,220);
    drawUpper(dest, offset, color);
    drawRightUpper(dest, offset, color);
    drawRightBottom(dest, offset, color);
}

void Dialog::draw8(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(35,156,220);
    drawCentral(dest, offset, color);
    drawUpper(dest, offset, color);
    drawRightUpper(dest, offset, color);
    drawLeftUpper(dest, offset, color);
    drawRightBottom(dest, offset, color);
    drawLeftBottom(dest, offset, color);
    drawBottom(dest, offset, color);
}

void Dialog::draw9(cv::Mat &dest, int offset)
{
    const cv::Vec3b color(156,56,220);
    drawCentral(dest, offset, color);
    drawUpper(dest, offset, color);
    drawRightUpper(dest, offset, color);
    drawLeftUpper(dest, offset, color);
    drawRightBottom(dest, offset, color);
    drawBottom(dest, offset, color);
}

Dialog::~Dialog()
{

}
