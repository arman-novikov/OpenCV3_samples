#include "include/Chapter5.h"

Chapter5::Chapter5()
{
    //ctor
}

/*!
This exercise will accustom you to the idea of many functions taking matrix
types. Create a two-dimensional matrix with three channels of type byte with
data size 100 × 100. Set all the values to 0 .
    a. Draw a circle in the matrix using void cv::circle(InputOutputArray img,
        cv::point center, intradius, cv::Scalar color, int thickness=1,
        int line_type=8, int shift=0) .
    b. Display this image using methods described in Chapter 2.
*/
int Chapter5::exer_1()
{
    const char winName[] = "displayWin";
    const cv::Size SIZE(100,100);
    cv::Mat img(SIZE, CV_8UC3, cv::Scalar(0,0,0));
    cv::circle(img, cv::Point(50,50), 15, cv::Scalar(200,200,200), 1, 8, 0);

    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, img);

    while (cv::waitKey() == 255);

    return 0;
}

/*!
Create a two-dimensional matrix with three channels of type byte with data size
100 × 100, and set all the values to 0 . Use the cv::Mat element access functions to
modify the pixels. Draw a green rectangle between (20, 5) and (40, 20).
*/
int Chapter5::exer_2()
{
    const char winName[] = "displayWin";
    const cv::Size SIZE(100,100);
    const cv::Point BEGINIG(5, 20), ENDING(20,40);
    //const cv::Scalar greenColor(0,250,0);
    cv::Mat_<cv::Scalar_<unsigned char>> img(SIZE, cv::Scalar_<unsigned char>(0,0,0));

    if (1)// element access
    for (int r = BEGINIG.y; r < ENDING.y; ++r) // element access
        for(int c = BEGINIG.x; c < ENDING.x; ++c)
            img(r,c) = GREEN;

    if (0)//iterators arithmetic
    for (int heigth = 0; heigth < ENDING.y - BEGINIG.y; ++heigth) {
        auto it = img.begin() + (BEGINIG.y + heigth) * img.cols + BEGINIG.x;
        const int width = ENDING.x - BEGINIG.x;
        const auto finish = it + width;
        for (; it < finish; ++it)
            *it = GREEN;
    }
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, img);

    while (cv::waitKey(33) == 255);
    return 0;
}

/*!
Practice using region of interest (ROI). Create a 210 × 210 single-channel byte
image and zero it. Within the image, build a pyramid of increasing values using
ROI and cv::Mat::setTo() . That is: the outer border should be 0 , the next inner
border should be 20 , the next inner border should be 40 , and so on until the final
innermost square is set to value 200 ; all borders should be 10 pixels wide. Display
the image.
*/
int Chapter5::exer_4()
{
    const int IMG_SIZE = 210;
    const int BORDER_STEP = 20;
    const int BORDER_WIDTH = 10;
    const char winName[] = "Pyramid";
    cv::Mat_<unsigned char> img(IMG_SIZE, IMG_SIZE);

    for (int initPoint = 0, intense = 0; initPoint < IMG_SIZE/2; initPoint += BORDER_WIDTH, intense += BORDER_STEP) {
        const int sideLen = IMG_SIZE - 2 * initPoint;
        cv::Mat roi(img, cv::Rect(initPoint, initPoint, sideLen, sideLen));
        roi.setTo(intense);
    }

    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, img);
    while (cv::waitKey(33) == 255);
    return 0;
}

/*!
Use multiple headers for one image. Load an image that is at least 100 × 100.
Create two additional headers that are ROIs where width = 20 and the height =
30 . Their origins should be at (5, 10) and (50, 60), respectively. Pass these new
image subheaders to cv::bitwise_not() . Display the loaded image, which
should have two inverted rectangles within the larger image.
*/
int Chapter5::exer_5()
{
    const cv::Point roiPoint1(10, 5), roiPoint2(360, 350);
    const cv::Size SIZE(220, 230);
    const char winName[] = "bitwise_not";

    cv::Mat img = cv::imread(PHOTO_PATH, -1);
    cv::Mat roi1(img, cv::Rect(roiPoint1, SIZE));
    cv::Mat roi2(img, cv::Rect(roiPoint2, SIZE));
    cv::bitwise_not(roi1, roi2);

    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, img);
    while (cv::waitKey(33) == 255);
    return 0;
}

/*!
Create a mask using cv::compare() . Load a real image. Use cv::split() to split
the image into red, green, and blue images.
    a. Find and display the green image.
    b. Clone this green plane image twice (call these clone1 and clone2 ).
    c. Find the green plane’s minimum and maximum value.
    d. Set clone1 ’s values to thresh = (unsigned char)((maximum - minimum)/
        2.0) .
    e. Set clone2 to 0 and use cv::compare (green_image, clone1, clone2,
        cv::CMP_GE) . Now clone2 will have a mask of where the value exceeds
        thresh in the green image.
    f. Finally, use cv::subtract (green_image,thresh/2,
        green_image, clone2) and display the results.
*/
int Chapter5::exer_6()
{
    typedef cv::Mat MAT_TYPE;
    typedef uchar ELEMENTS_TYPE;
    const int GREEN_CHANNEL = 1;
    const char greenChannelWinName[] = "green channel",
               resWinName[] = "result img";
    const MAT_TYPE img = cv::imread(PHOTO_PATH, -1); //const MAT_TYPE img = cv::imread(PHOTO_PATH, cv::IMREAD_REDUCED_COLOR_2);
    std::vector<MAT_TYPE> channels;
    ELEMENTS_TYPE green_min, green_max;

    cv::split(img, channels);

    MAT_TYPE clone1 = channels[GREEN_CHANNEL].clone(),
             clone2;
    channels[GREEN_CHANNEL].copyTo(clone2); // the same:
/* why begin() == end()?
    cv::MatIterator_<ELEMENTS_TYPE> it = channels[GREEN_CHANNEL].begin<ELEMENTS_TYPE>();
    green_min = green_max = *it;
    for (; it < channels[GREEN_CHANNEL].end<ELEMENTS_TYPE>(); ++it) {
        green_min = std::min(green_min, *it);
        green_max = std::max(green_max, *it);
        std::cout<<static_cast<int>(*it)<<"\t"<<static_cast<int>(*it)<<std::endl;
    }
*/
    green_min = green_max = clone1.at<uchar>(0,0);
    for (int r = 0; r < clone1.rows; ++r)
        for (int c = 0; c < clone1.cols; ++c) {
            ELEMENTS_TYPE val = clone1.at<ELEMENTS_TYPE>(r,c);
            green_min = std::min(green_min, val);
            green_max = std::max(green_max, val);
        }

    cv::namedWindow(greenChannelWinName, cv::WINDOW_AUTOSIZE);
    cv::imshow(greenChannelWinName, channels[GREEN_CHANNEL]);

    const ELEMENTS_TYPE thresh = static_cast<ELEMENTS_TYPE>((green_max - green_min) / 2.0);
    clone2.setTo(static_cast<ELEMENTS_TYPE>(thresh));
    clone2.setTo(static_cast<ELEMENTS_TYPE>(0));
    cv::compare(channels[GREEN_CHANNEL], clone1, clone2, cv::CMP_GE);

    cv::subtract(channels[GREEN_CHANNEL], thresh/2, channels[GREEN_CHANNEL], clone2);

    cv::namedWindow(resWinName, cv::WINDOW_AUTOSIZE);
    cv::imshow(resWinName, channels[GREEN_CHANNEL]);
    while (cv::waitKey(33) == 255);
    return 0;
}
