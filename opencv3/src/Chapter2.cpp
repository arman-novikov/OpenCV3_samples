#include "include/Chapter2.h"

Chapter2::Chapter2()
{
    //ctor
}

int Chapter2::e2_2_showImg()
{
    cv::Mat img = cv::imread(PHOTO_PATH, -1);

    if (img.empty())
        return -1;

    cv::namedWindow("Example2", cv::WINDOW_AUTOSIZE);
    cv::imshow("Example2",img);
    cv::waitKey(0);

    cv::destroyWindow("Example2");
    return 0;
}

int Chapter2::e2_3_showVid()
{
    cv::namedWindow("Example3", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap;

    cap.open(VIDEO_PATH);

    cv::Mat frame;

    for(;;) {
        cap >> frame;
        if(frame.empty()) {
            std::cout<<"empty\n";
            break;
        }
        cv::imshow("Example3", frame);
        if (cv::waitKey(33) != 255) {
            std::cout<<"pressed "<<"\n";
            break;
        }
    }

    return 0;
}

int Chapter2::e2_4_showVidTackSlide_not_refactored()
{
    int g_slider_position = 0;
    int g_run = 1, g_dontset = 0;
    cv::VideoCapture g_cap;
    struct param_struct{
        int *g_run, *g_dontset;
         cv::VideoCapture *g_cap;
    } params = {&g_run, &g_dontset, &g_cap};

    auto onTrackbarSlide = [](int pos, void* _params){
        struct param_struct *par_struct = (param_struct*)(_params);
        par_struct->g_cap->set(cv::CAP_PROP_POS_FRAMES, pos);
        if (!*(par_struct->g_dontset))
            *(par_struct->g_run) = 1;
        *(par_struct->g_dontset) = 0;
    };
    const char windowsName[] = "Example2_4";
    cv::namedWindow(windowsName, cv::WINDOW_AUTOSIZE);
    g_cap.open(VIDEO_PATH);
    int frames = static_cast<int>(g_cap.get(cv::CAP_PROP_FRAME_COUNT));
    int tmpw   = static_cast<int>(g_cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int tmph   = static_cast<int>(g_cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    std::cout<<"Video has "<<frames<<"frames of dimensions("
             <<tmpw<<", "<<tmph<<").\n";

    cv::createTrackbar("Position", windowsName, &g_slider_position, frames, onTrackbarSlide, &params);

    cv::Mat frame;

    for(;;) {
        if (g_run != 0) {
            g_cap >> frame;
            if (frame.empty())
                break;
            int current_pos = (int)(g_cap.get(cv::CAP_PROP_POS_FRAMES));
            g_dontset = 1;

            cv::setTrackbarPos("Position", windowsName, current_pos);
            cv::imshow(windowsName, frame);

            g_run -= 1;
            std::cout<<"out of if g_run != 0\n";
        }

        switch (static_cast<char>(cv::waitKey(10))) {
        case 's':
            g_run = 1;
            std::cout<<"Single step, run = "<<g_run<<std::endl;
            break;
        case 'r':
            g_run = -1;
            std::cout<<"Run mode, run = "<<g_run<<std::endl;
            break;
        case 27:
            return 0;
        }
    std::cout<<"run = "<<g_run<<std::endl;
    }

    return 0;
}

int Chapter2::e2_4_showVidTackSlide()
{
#define windowsName "Example2_4"
#define TrackBarName "Position"
    struct shared_data {
        cv::VideoCapture cap;
        cv::Mat frame;
        bool is_stopped;
    } sh_data;
    sh_data.cap.open(VIDEO_PATH);
    sh_data.is_stopped = false;

    const int frames_count = static_cast<int>(sh_data.cap.get(cv::CAP_PROP_FRAME_COUNT));
    std::cout<<"Video has "<<frames_count<<" frames of dimensions("
             <<static_cast<int>(sh_data.cap.get(cv::CAP_PROP_FRAME_WIDTH))
             <<"x"
             <<static_cast<int>(sh_data.cap.get(cv::CAP_PROP_FRAME_HEIGHT))
             <<").\n";

    auto onTrackbarSlide = [](int _pos, void *params) {
        struct shared_data *_sh_data = static_cast<shared_data*>(params);
        _sh_data->cap.set(cv::CAP_PROP_POS_FRAMES, _pos);

        if (_sh_data->is_stopped) {
            const int cur_pos = cv::getTrackbarPos(TrackBarName, windowsName);
            _sh_data->cap.set(cv::CAP_PROP_POS_FRAMES, cur_pos);
            _sh_data->cap >> _sh_data->frame;

            if (_sh_data->frame.empty())
                return;

            cv::imshow(windowsName, _sh_data->frame);
        }
    };

    cv::namedWindow(windowsName, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar(TrackBarName, windowsName, nullptr, frames_count, onTrackbarSlide, &sh_data);

    for(;;) {
        if (!sh_data.is_stopped) {
            sh_data.cap >> sh_data.frame;
            if (sh_data.frame.empty())
                break;
            const int cur_pos = static_cast<const int>(sh_data.cap.get(cv::CAP_PROP_POS_FRAMES));
            cv::setTrackbarPos(TrackBarName, windowsName, cur_pos);
            cv::imshow(windowsName, sh_data.frame);
        }

        switch (static_cast<const char>(cv::waitKey(10))) {
        case 's':
            sh_data.is_stopped = true;
            std::cout<<"Single step"<<std::endl;
            break;
        case 'r':
            sh_data.is_stopped = false;
            std::cout<<"Run mode"<<std::endl;
            break;
        case 27:
            return 0;
        }
    }
#undef windowsName
#undef TrackBarName
    return 0;
}


int Chapter2::e2_5_GaussianBlurUsage()
{
    const char winnameIN[] = "Example2_5-in";
    const char winnameOUT[] = "Example2_5-out";
    const cv::Mat imgIN = cv::imread(PHOTO_PATH, -1);
    cv::Mat imgOUT;

    cv::imshow(winnameIN, imgIN);
    cv::GaussianBlur(imgIN, imgOUT, cv::Size(5,5), 3, 3);
    cv::GaussianBlur(imgOUT, imgOUT, cv::Size(5,5), 3, 3);

    cv::imshow(winnameOUT, imgOUT);

    cv::waitKey(0);
    return 0;
}

int Chapter2::e2_6_PyrDownUsage()
{
    const char winnameIN[] = "Example2_6-in";
    const char winnameOUT[] = "Example2_6-out";
    const cv::Mat imgIN = cv::imread(PHOTO_PATH);
    cv::Mat imgOUT;

    cv::namedWindow(winnameIN, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(winnameOUT, cv::WINDOW_AUTOSIZE);

    cv::imshow(winnameIN, imgIN);

    cv::pyrDown(imgIN, imgOUT);
    cv::imshow(winnameOUT, imgOUT);

    cv::waitKey(0);

    return 0;
}

int Chapter2::e2_7_CannyUsage()
{
    const char winnameGray[] = "Example Gray";
    const char winnameCanny[] = "Example Canny";
    const cv::Mat img_rgb = cv::imread(PHOTO_PATH);
    cv::Mat img_gry, img_cny;

    cv::namedWindow(winnameGray, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(winnameCanny, cv::WINDOW_AUTOSIZE);

    cv::cvtColor(img_rgb, img_gry, cv::COLOR_BGR2GRAY);
    cv::imshow(winnameGray, img_gry);

    cv::Canny(img_gry, img_cny, 10, 100, 3, true);
    cv::imshow(winnameCanny, img_cny);

    cv::waitKey(0);

    return 0;
}

int Chapter2::e2_10_showCameraInput()
{
    const char winName[] = "Example2_10";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap;

    if (!cap.open(0)) {
        std::cerr<<"coudn't open capture"<<std::endl;
        return -1;
    }

    for(cv::Mat frame;;) {
        cap >> frame;
        if(frame.empty()) {
            std::cout<<"empty\n";
            break;
        }
        cv::imshow(winName, frame);
        if (cv::waitKey(33) != 255) {
            std::cout<<"pressed "<<"\n";
            break;
        }
    }

    return 0;
}

int Chapter2::e2_11_videoToLog_Polar()
{
    const char resFile[] = "res.avi";
    const char winNameIn[] = "Example 2_11";
    const char winNameOut[] = "log_polar";
    cv::VideoCapture capture(VIDEO_PATH);                // cv::VideoCapture capture(1) to use camera input
    const double fps = capture.get(cv::CAP_PROP_FPS);
    const int frame_width = static_cast<const int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    const int frame_height = static_cast<const int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    const cv::Size frame_size(frame_width, frame_height);

    cv::VideoWriter writer(resFile, CV_FOURCC('M', 'J','P','G'), fps, frame_size);

    for(cv::Mat logpolar_frame, bgr_frame;;) {
        capture >> bgr_frame;
        if (bgr_frame.empty())
            break;
        const float point_X = bgr_frame.cols/2;         // x
        const float point_Y = bgr_frame.rows/2;         // y
        const cv::Point2f point(point_X, point_Y);
        cv::logPolar(bgr_frame, logpolar_frame, point,
        40,                                             // Magnitude (scale parameter)
        cv::WARP_FILL_OUTLIERS);                        // Fill ouliers with zero

        cv::imshow(winNameIn, bgr_frame);
        cv::imshow(winNameOut, logpolar_frame);
        writer << logpolar_frame;

        if (cv::waitKey(24) == 27)
            break;
    }
    capture.release();

    return 0;
}
/*!
3. Use the capture and store code in Example 2-11 together with the PyrDown()
    code of Example 2-6 to create a program that reads from a camera and stores
    downsampled color images to disk.
4. Modify the code in Exercise 3 and combine it with the window display code in
    Example 2-2 to display the frames as they are processed.
5. Modify the program of Exercise 4 with a trackbar slider control from
    Example 2-4 so that the user can dynamically vary the pyramid downsampling
    reduction level by factors of between 2 and 8. You may skip writing this to disk,
    but you should display the results.
*/
int Chapter2::exer()
{
    //const char resFile[] = "res.avi";
    const char winNameIn[] = "Example 2_11";
    const char winNameOut[] = "log_polar";
    const char trackbarName[] = "size of frame";
    cv::VideoCapture capture(0);                // cv::VideoCapture capture(1) to use camera input
    //const double fps = capture.get(cv::CAP_PROP_FPS);
    const int frame_width = static_cast<const int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    const int frame_height = static_cast<const int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    int pyrDown_frame_factor = 2;
    cv::Size pyrDown_frame_size(frame_width/2, frame_height/2); // for writing afte
    //cv::VideoWriter writer(resFile, CV_FOURCC('M', 'J','P','G'), fps, pyrDown_frame_size);

    auto onTrackbarSlide = [] (int _pos, void *_size) {
    };
    cv::namedWindow(winNameIn, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar(trackbarName, winNameIn, &pyrDown_frame_factor, 8, onTrackbarSlide, nullptr);
    for(cv::Mat bgr_frame, pyrDown_frame;;) {
        capture >> bgr_frame;
        if (bgr_frame.empty())
            break;

        pyrDown_frame_size = cv::Size(frame_width/pyrDown_frame_factor, frame_height/pyrDown_frame_factor);
cv::Size s = pyrDown_frame.size();
std::cout<<s.width<<" "<<s.height<<std::endl;
std::cout<<bgr_frame.cols<<" "<<bgr_frame.rows<<std::endl<<std::endl;
        cv::pyrDown(bgr_frame, pyrDown_frame);
        if (pyrDown_frame.empty())
            continue;
        for (int i = 0; i < pyrDown_frame_factor; ++i)
            cv::pyrDown(pyrDown_frame, pyrDown_frame);

        cv::imshow(winNameIn, bgr_frame);
        cv::imshow(winNameOut, pyrDown_frame);
        //writer << pyrDown_frame;

        if (cv::waitKey(24) == 27)
            break;
    }
    capture.release();

    return 0;
}
