#include "include/Chapter6.h"

Chapter6::Chapter6()
{
    //ctor
}

/*
Drawing practice: load or create and display a color image. Draw one example of
every shape and line that OpenCV can draw.
*/
int Chapter6::exer_1()
{
    const char winName[] = "res";
    cv::Mat img = cv::imread(PHOTO_PATH, -1);

    cv::circle(img, cv::Point(30,15), 10, RED, 5, 4, 0);
    cv::circle(img, cv::Point(30,150), 180, GREEN, 10, 4, 0);
    cv::circle(img, cv::Point(30,150), 10, BLUE, 5, 8, 0);

    cv::ellipse(img, cv::Point(500, 500), cv::Size(100, 50), 45, 0, 360, RED, 3, 4, 0);
    cv::ellipse(img, cv::Point(500, 500), cv::Size(100, 50), 135, 0, 360, GREEN, 3, 4, 1);
    const cv::RotatedRect rotatedRect(cv::Point2f(600.0, 200.0), cv::Size2f(50.0, 150.0), 60);
    cv::ellipse(img, rotatedRect, BLUE, 3, 4);

    std::vector<cv::Point> pts;
    cv::ellipse2Poly(cv::Point(350, 350), cv::Size(50, 150), 30, 0, 360, 3, pts);
    cv::fillConvexPoly(img, pts.data(), pts.size(), RED, 4, 0);

    cv::line(img, cv::Point(30,30), cv::Point(600,300), GREEN, 8, 0);
    cv::line(img, cv::Point(85,85), cv::Point(650,350), BLUE, 2, 1);

    cv::rectangle(img, cv::Point(400, 40), cv::Point(0,0), RED, 4, 0);

    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, img);
    while (cv::waitKey(33) == 255);
    return 0;
}

/*
Grayscale: load and display a color image.
    a. Turn it into three-channel grayscale (it is still an BGR image, but it looks gray
        to the user).
    b. Draw color text onto the image.
*/
int Chapter6::exer_2()
{
    const char winName[] = "res";
    cv::Mat img = cv::imread(PHOTO_PATH, -1);
    /*
    typedef cv::Vec<uchar, 3> pixType;
    for (int r = 0; r < img.rows; ++r)
        for (int c = 0; c < img.cols; ++c) {
            pixType &pix = img.at<pixType>(r,c);
            uchar avr = (pix[0] + pix[1] + pix[2])/3;
            pix = pixType(avr, avr, avr);
        }
    */
    cv::cvtColor(img, img, CV_RGB2GRAY);
    cv::putText(img, "hello", cv::Point(100, 100), cv::FONT_HERSHEY_PLAIN, 4, GREEN, 2, 4, true);
    cv::putText(img, "hello", cv::Point(500, 100), cv::FONT_HERSHEY_DUPLEX, 4, GREEN, 2, 4, false);

    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, img);
    while (cv::waitKey(33) == 255);
    return 0;
}

/*
Dynamic text: load and display video from a video or from a camera.
    a. Draw a frame per second (FPS) counter somewhere on the image.
*/
int Chapter6::exer_3()
{
    const char winName[] = "res";
    cv::VideoCapture video(VIDEO_PATH);
    cv::Mat frame;
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    const int FPS = static_cast<int>(video.get(CV_CAP_PROP_FPS));
    const cv::String FPS_str(std::to_string(FPS).data());

    for (;;) {
        video >> frame;

        if (frame.empty())
            break;

        cv::putText(frame, FPS_str, cv::Point(30, 30),
                    cv::FONT_HERSHEY_COMPLEX, 1, GREEN, 3, 4, false);
        cv::imshow(winName, frame);

        if (cv::waitKey(FPS) == 27)
            break;
    }

    return 0;
}


int Chapter6::exer_4()
{
    std::cout << "see cv_ch6_ex4" << std::endl;
    return 0;
}

/*
Use cv::LineIterator to count pixels on different line segments in, say, a 300 ×
300 image.
    a. At what angles do you get the same number of pixels for 4-connected and
        8-connected lines?
    b. For line segment angles other than the above, which counts more pixels:
        4-connected or 8-connected lines?
    c. For a given line segment, explain the difference in the length of the line com‐
        pared to the number of pixels you count iterating along the line for
        both 4-connected and 8-connected? Which connectedness is closer to the true
        line length?
*/
int Chapter6::exer_5()
{
    cv::Mat img(300, 300, CV_8UC3, RED);
    const cv::Point Point0(0, 0), Point0_10(0, 10), Point0_20(0, 20), Point0_30(0, 30);
    const cv::Point Point20_20(20, 20), Point40_20(40, 20), Point30_20(30, 20), Point40_30(40, 30);
    const cv::Point Point90_0(90, 0), Point0_90(0, 90), Point10_20(10, 20);

    cv::LineIterator iter1_4(img, Point0, Point20_20, 4);
    cv::LineIterator iter2_8(img, Point0, Point20_20, 8);
    std::cout<<"45 deg: "<<iter1_4.count<<'\t'<<iter2_8.count<<std::endl;

    cv::LineIterator iter3_4(img, Point0, Point0_20, 4);
    cv::LineIterator iter4_8(img, Point0, Point0_20, 8);
    std::cout<<"90 deg: "<<iter3_4.count<<'\t'<<iter4_8.count<<std::endl;

    cv::LineIterator iter5_4(img, Point0, Point10_20, 4);
    cv::LineIterator iter6_8(img, Point0, Point10_20, 8);
    std::cout<<"30 deg: "<<iter5_4.count<<'\t'<<iter6_8.count<<std::endl;

    return 0;
}
