#ifndef DRAWLINES_H
#define DRAWLINES_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const char PHOTO_PATH[] = "/home/arman/1.jpg";

const cv::Scalar BLACK(0, 0, 0);
const cv::Scalar WHITE(255, 255, 255);
const char WINNAME[] = "res";
const int DIGIT_WIDTH = 10;
const int DIGIT_SPACING = 5;
const int DIGIT_OFFSET = DIGIT_WIDTH + DIGIT_SPACING;
const int DIGITS_NUMBER = 10;
const int COLUMNS = DIGIT_OFFSET * DIGITS_NUMBER;
const int ROWS = 20;
const int PADDING = 2;
const int LINE_WIDTH = 2;

void drawUpper(cv::Mat &dest, int offset, const cv::Vec3b color);
void drawCentral(cv::Mat &dest, int offset, const cv::Vec3b color);
void drawRightUpper(cv::Mat &dest, int offset, const cv::Vec3b color);
void drawLeftUpper(cv::Mat &dest, int offset, const cv::Vec3b color);
void drawRightBottom(cv::Mat &dest, int offset, const cv::Vec3b color);
void drawLeftBottom(cv::Mat &dest, int offset, const cv::Vec3b color);
void drawBottom(cv::Mat &dest, int offset, const cv::Vec3b color);

#endif // DRAWLINES_H
