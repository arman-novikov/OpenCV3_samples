#include "drawlines.h"

void drawUpper(cv::Mat &dest, int offset, const cv::Vec3b color)
{
    for (int r = PADDING;r < PADDING + LINE_WIDTH; ++r)
        for (int c = offset; c < DIGIT_WIDTH  + offset; ++c)
            dest.at<cv::Vec3b>(r,c) = color;
}

void drawRightUpper(cv::Mat &dest, int offset, const cv::Vec3b color)
{
    for (int r = PADDING;r < ROWS/2; ++r)
        for (int c = offset + DIGIT_WIDTH - LINE_WIDTH; c < DIGIT_WIDTH + offset; ++c)
            dest.at<cv::Vec3b>(r,c) = color;
}

void drawLeftUpper(cv::Mat &dest, int offset, const cv::Vec3b color)
{
    for (int r = PADDING;r < ROWS/2; ++r)
        for (int c = offset; c < offset + LINE_WIDTH; ++c)
            dest.at<cv::Vec3b>(r,c) = color;
}

void drawCentral(cv::Mat &dest, int offset, const cv::Vec3b color)
{
    for (int r = ROWS/2 - 1; r < ROWS/2 + 1; ++r)
        for (int c = offset; c < DIGIT_WIDTH + offset; ++c)
            dest.at<cv::Vec3b>(r,c) = color;
}

void drawLeftBottom(cv::Mat &dest, int offset, const cv::Vec3b color)
{
    for (int r = ROWS / 2; r < ROWS - PADDING; ++r)
        for (int c = offset; c < LINE_WIDTH + offset; ++c)
            dest.at<cv::Vec3b>(r,c) = color;

}

void drawRightBottom(cv::Mat &dest, int offset, const cv::Vec3b color)
{
    for (int r = ROWS / 2; r < ROWS - PADDING; ++r)
        for (int c = offset + DIGIT_WIDTH - LINE_WIDTH; c < offset + DIGIT_WIDTH; ++c)
            dest.at<cv::Vec3b>(r,c) = color;

}
void drawBottom(cv::Mat &dest, int offset, const cv::Vec3b color)
{
    for (int r = ROWS - LINE_WIDTH - PADDING; r < ROWS - PADDING; ++r)
        for (int c = offset; c < DIGIT_WIDTH + offset; ++c)
            dest.at<cv::Vec3b>(r,c) = color;
}
