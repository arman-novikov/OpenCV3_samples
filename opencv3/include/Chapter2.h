#ifndef CHAPTER2_H
#define CHAPTER2_H

#include <include/MetaChapter.h>


class Chapter2 : public MetaChapter
{
public:
    Chapter2();
    int e2_2_showImg();
    int e2_3_showVid();
    int e2_4_showVidTackSlide_not_refactored();
    int e2_4_showVidTackSlide();
    int e2_5_GaussianBlurUsage();
    int e2_6_PyrDownUsage();
    int e2_7_CannyUsage();
    int e2_10_showCameraInput();
    int e2_11_videoToLog_Polar();
    int exer();
};

#endif // CHAPTER2_H
