#ifndef METACHAPTER_H
#define METACHAPTER_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <errno.h>

class MetaChapter
{
public:
    MetaChapter();

protected:
    static const size_t PATH_SIZE = 32U;
    static const char VIDEO_PATH[PATH_SIZE];
    static const char PHOTO_PATH[PATH_SIZE];
    static const cv::Scalar RED;
    static const cv::Scalar GREEN;
    static const cv::Scalar BLUE;
};

#endif // ICHAPTER_H
