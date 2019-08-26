#include <include/Chapter8.h>
#include <time.h>
#include <array>
#include <cmath>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <vector>

void Chapter8::get_video_codec(const std::string &addr)
{
    cv::VideoCapture cap(addr);
    uint32_t val = static_cast<uint32_t>(cap.get(cv::CAP_PROP_FOURCC));
    char fourcc[5] = {0};
    for (uint8_t i = 0; i < 4U; ++i)
        fourcc[i] = static_cast<char>(val>>8*i);
    std::cout << fourcc << std::endl;
}

int Chapter8::write_data(const std::string &file_name)
{
    cv::FileStorage fs(file_name, cv::FileStorage::WRITE);
    if (!fs.isOpened()) {
        std::cerr << "can't create a file \'"<< file_name <<"\' to write in" << std::endl;
        return -ENOENT;
    }
    fs << "frameCount" << 5;

    time_t raw_time;
    time(&raw_time);
    fs << "CalibrationDate" << asctime(localtime(&raw_time));
    cv::Mat cameraMatrix = (
      cv::Mat_<double>(3,3) << 1000, 0, 320, 0, 1000, 0, 0, 1
    );

    cv::Mat distCoeffs = (
        cv::Mat_<double>(5,1) << 0.1, 0.01, -0.001, 0, 0
    );

    fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;

    fs << "features" << "[";
    for (int i = 0; i < 3; ++i) {
        int x = rand() % 640;
        int y = rand() % 480;
        uchar lbp = static_cast<uchar>(rand() % 256);
        fs << "{:" <<  "x" << x << "y" << y <<"lbp" << "[:";
        for (int j = 0; j < 8; ++j)
            fs << ((lbp >> j) & 1);
        fs << "]" <<"}";
    }
    fs << "]";
    fs.release();
    return 0;
}

int Chapter8::read_data(const std::string &file_name)
{
    cv::FileStorage fs(file_name, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        std::cerr << "can't open a file  \'"<< file_name <<"\'  to read from" << std::endl;
        return -ENOENT;
    }
    cv::Mat_<double> anArray(3,3);
    fs["cameraMatrix"] >> anArray;
    for (int i = 0; i < anArray.cols; ++i) {
        for (int j = 0; j < anArray.rows; ++j)
            std::cout <<anArray.at<double>(i,j) << "\t";
        std::cout << std::endl;
    }
    fs.release();
    return 0;
}

/*
1. Create a program that (1) reads frames from a video, (2) turns the result to gray‐
   scale, and (3) performs Canny edge detection on the image. Display all three
   stages of processing in three different windows, with each window appropriately
   named for its function.
        a. Display all three stages of processing in one image. (Hint: create another
        image of the same height but three times the width as the video frame. Copy
        the images into this, either by using pointers or (more cleverly) by creating
        three new image headers that point to the beginning of and to one-third and
        two-thirds of the way into the imageData . Then use Mat::copyTo() .)

        b. Write appropriate text labels describing the processing in each of the three
        slots.
 */
int Chapter8::exer_1()
{
    static constexpr char winname[] = "original, gray, canny";
    static constexpr char origname[] = "original pict";
    static constexpr char grayname[] = "grey pict";
    static constexpr char cannyname[] = "canny pict";
    const cv::Point lable_point(50,50);
    cv::namedWindow(winname, cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap(MetaChapter::VIDEO_PATH);

    if (!cap.isOpened()) {
        std::cerr<<"failed to open capture"<<std::endl;
        return -ENOENT;
    }

    const int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    const cv::Size frame_size(frame_width, frame_height);
    const cv::Size res_img_size(frame_width*3, frame_height); // gonna have 3 equal sections

    cv::Mat res_img = cv::Mat(res_img_size, CV_8UC3); // the image with 3 sections
    cv::Mat orig = res_img(cv::Rect(cv::Point(0, 0), frame_size)); // at the beginig
    cv::Mat gray = res_img(cv::Rect(cv::Point(frame_width, 0), frame_size)); // at the 2nd section
    cv::Mat canny = res_img(cv::Rect(cv::Point(frame_width*2, 0), frame_size)); //at the 3rd section

    for(cv::Mat temp;;) {
        cap >> orig; // took a frame
        if (orig.empty()) {
            std::cerr << "frame is empty" << std::endl;
            break;
        }
        cv::cvtColor(orig, temp, cv::COLOR_BGR2GRAY);
        cv::cvtColor(temp, gray, cv::COLOR_GRAY2BGR);

        cv::Canny(temp, temp, 1, 100, 3, true);
        cv::cvtColor(temp, canny, cv::COLOR_GRAY2BGR);

        cv::putText(orig, origname, lable_point, cv::FONT_HERSHEY_PLAIN, 3.0, MetaChapter::RED, 3);
        cv::putText(gray, grayname, lable_point, cv::FONT_HERSHEY_PLAIN, 3.0, MetaChapter::RED, 3);
        cv::putText(canny, cannyname, lable_point, cv::FONT_HERSHEY_PLAIN, 3.0, MetaChapter::RED, 3);

        cv::imshow(winname, res_img);

        if (cv::waitKey(33) != 255) {
            std::cout<<"pressed "<<"\n";
            break;
        }
    }
    cap.release();
    return 0;
}

/*
2.  Create a program that reads in and displays an image. When the user’s mouse
    clicks on the image, read in the corresponding pixel values (blue, green, red) and
    write those values as text to the screen at the mouse location.
        a. For the program of Exercise 2, display the mouse coordinates of the individ‐
           ual image when clicking anywhere within the three-image display.
 */
int Chapter8::exer_2()
{
    static constexpr char winname[] = "pict";
    static const cv::Mat orig = cv::imread(MetaChapter::PHOTO_PATH); // cashed
    cv::Point ;
    cv::Mat disp;
    orig.copyTo(disp);
    struct on_mouse_t{
        cv::Point *point;
        cv::Mat *img;
    } param = {&click_point, &disp};

    auto on_mouse = [] (int event, int x, int y, int flags, void* data) {
        (void)(flags);
        on_mouse_t *on_mouse_data = static_cast<on_mouse_t*>(data);
        auto *img = on_mouse_data->img;
        auto *point = on_mouse_data->point;
        if (event != cv::EVENT_LBUTTONDOWN)
            return;
        std::cout << "x: " << x << ", y: " << y << std::endl;
        *on_mouse_data->point = cv::Point(x,y);       
        cv::Vec3b bgr = img->at<cv::Vec3b>(*point);
        std::string rgb_info;
        rgb_info.reserve(32);
        for (int i = 2; i >= 0; --i) {
            rgb_info += std::to_string(static_cast<int>(bgr[i]));
            if (i != 0)
                rgb_info += ":";
        }
        std::cout << rgb_info << std::endl;
        cv::putText(*img, rgb_info.c_str(), *point, cv::FONT_HERSHEY_PLAIN, 1, MetaChapter::GREEN, 2);
        cv::imshow(winname, *img);
    };    
    cv::namedWindow(winname);
    cv::setMouseCallback(winname, on_mouse, static_cast<void*>(&param));
    cv::imshow(winname, disp);

    for (;;) {
        char k = static_cast<char>(cv::waitKey(33));
        if ( k == 'r') {
            std::cout<<"reset the img "<<"\n";
            orig.copyTo(disp);
            cv::imshow(winname, disp);
            continue;
        }

        if (k == 27) {
            std::cout<<"pressed "<<"\n";
            break;
        }
    }
    return 0;
}

/*
3. Create a program that reads in and displays an image.
    a. Allow the user to select a rectangular region in the image by drawing a rectan‐
        gle with the mouse button held down, and highlight the region when the
        mouse button is released. Be careful to save an image copy in memory so that
        your drawing into the image does not destroy the original values there. The
        next mouse click should start the process all over again from the original
        image.
    b. In a separate window, use the drawing functions to draw a graph in blue,
        green, and red that represents how many pixels of each value were found in
        the selected box. This is the color histogram of that color region. The x-axis
        should be eight bins that represent pixel values falling within the ranges 0–31,
        32–63,..., 223–255. The y-axis should be counts of the number of pixels that
        were found in that bin range. Do this for each color channel, BGR.
*/
int Chapter8::exer_3()
{
    static constexpr char winname[] = "pict";
    static constexpr size_t SUBPIXELS_COUNT = 3U;
    static constexpr size_t MAX_INTENSE = 255U;
    static constexpr int RIDGE = 32;
    static constexpr size_t RIDGES_COUNT = MAX_INTENSE / RIDGE + 1;

    static const cv::Mat orig = cv::imread(MetaChapter::PHOTO_PATH); // cashed
    typedef std::array<std::array<size_t, RIDGES_COUNT>, SUBPIXELS_COUNT> pixels_vals_t;
    static  pixels_vals_t pixels_vals;

    static auto _summator = []
        (const cv::Mat &roi, const int start_row, const int end_row, pixels_vals_t& out) {
        for (int r = start_row; r < end_row; ++r) {
            for (int c = 0; c < roi.cols; ++c) {
                cv::Vec3b pix = roi.at<cv::Vec3b>(r, c);
                for (size_t i = 0; i < SUBPIXELS_COUNT; ++i) {
                    ++out[i][pix[static_cast<int>(i)]/RIDGE];
                }
            }
        }
    };

    // strightforward counting
    static auto _plain_counter = [] (const cv::Mat &roi) {
        _summator(roi, 0, roi.rows, pixels_vals);
    };

    /// 3 threads count each subpixels
    static auto _subpixels_parall = [] (const cv::Mat &roi) {
        static auto _counter = [] (const cv::Mat &roi, int sub_pixel_num) {
            for (int r = 0; r < roi.rows; ++r) {
                for (int c = 0; c < roi.cols; ++c) {
                    uint8_t sub_pix_val = roi.at<cv::Vec3b>(r, c)[sub_pixel_num];
                    ++pixels_vals[static_cast<size_t>(sub_pixel_num)][sub_pix_val/RIDGE];
                }
            }
        };
        std::thread b_counter(_counter, std::ref(roi), 0),
                    g_counter(_counter, std::ref(roi), 1),
                    r_counter(_counter, std::ref(roi), 2);
        b_counter.join(); g_counter.join(); r_counter.join();
    };

    /// counting the spectrum in n threads (img divided into n areas)
    static auto _rows_parall = [] (const cv::Mat &roi) {
        const int threads_count = static_cast<int>(std::thread::hardware_concurrency());
        std::cerr << "threads count = " << threads_count << std::endl;
        const int task_len = roi.rows / threads_count;
        typedef  std::unique_ptr<std::thread> worker_t;
        std::vector<worker_t> workers;
        std::vector<pixels_vals_t> snitches;
        workers.reserve(static_cast<size_t>(threads_count));
        snitches.resize(static_cast<size_t>(threads_count), {});

        for (int i = 0; i < threads_count; ++i) {
            const int start_row = i * task_len;
            int end_row = start_row + task_len;
            if (i == threads_count - 1)
                end_row = roi.rows;

            workers.push_back(std::make_unique<std::thread>
                                        (_summator, std::ref(roi), start_row, end_row,
                                         std::ref(snitches[static_cast<size_t>(i)])));
        }
        for (size_t i = 0; i < static_cast<size_t>(threads_count); ++i)  {
            workers[i]->join();
            for (size_t j = 0; j < SUBPIXELS_COUNT; ++j)
                for (size_t k = 0; k < RIDGES_COUNT; ++k)
                    pixels_vals[j][k] += snitches[i][j][k];
        }
    };

    static auto counter = [] (const cv::Mat &roi) {
        auto start = std::chrono::system_clock::now();
        for (auto &i : pixels_vals)
            i.fill(0);

        //_plain_counter(roi);
        //_subpixels_parall(roi);
        _rows_parall(roi);

        auto end = std::chrono::system_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "time spent: " << elapsed.count() << " ms" << std::endl;

        for (size_t i = 0; i < SUBPIXELS_COUNT; ++i) {
            for (size_t j = 0; j < RIDGES_COUNT; ++j) {
                std::cout << pixels_vals[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    };
    cv::Point click_point;
    cv::Mat img;
    orig.copyTo(img);    
    auto on_mouse = [] (int event, int x, int y, int flags, void* data) {
        static constexpr int RIDGE = 1;
        (void)flags;
        static bool is_ready = false;
        static bool is_first_point_ready = false;
        static cv::Point start_point;
        static cv::Point end_point;
        cv::Mat *pict = static_cast<cv::Mat*>(data);

        if (event == cv::EVENT_LBUTTONDOWN) {
            if (is_ready == true) { // reseting all to begin again
                is_ready = false;
                is_first_point_ready = false;
                orig.copyTo(*pict);
                cv::imshow(winname, *pict);
                std::cerr << "cleaned" << std::endl;
                return;
            }
            start_point.x = x;
            start_point.y = y;
            is_first_point_ready = true;
            std::cerr << "1st point captured" << std::endl;
            return;
        }
        if (event == cv::EVENT_LBUTTONUP && is_first_point_ready) {
            end_point.x = x;
            end_point.y = y;            
            cv::rectangle(*pict, start_point, end_point, MetaChapter::RED, RIDGE);
            is_first_point_ready = false;
            is_ready = true;
            cv::imshow(winname, *pict);
            double start_point_dist = std::sqrt(std::pow(start_point.x, 2) + std::pow(start_point.y, 2));
            double end_point_dist = std::sqrt(std::pow(end_point.x, 2) + std::pow(end_point.y, 2));
            if (start_point_dist < end_point_dist) {
                start_point.x += RIDGE;
                start_point.y += RIDGE;
                end_point.x -= RIDGE;
                end_point.y -= RIDGE;
            } else {
                start_point.x -= RIDGE;
                start_point.y -= RIDGE;
                end_point.x += RIDGE;
                end_point.y += RIDGE;
            }
            std::cerr << "roi drawn" << std::endl;
            counter((*pict)(cv::Rect(start_point, end_point)));
            return;
        }
    };

    if (orig.empty()) {
        std::cerr<<"failed to open picture"<<std::endl;
        return -ENOENT;
    }

    cv::namedWindow(winname, cv::WINDOW_FULLSCREEN);
    cv::setMouseCallback(winname, on_mouse, static_cast<void*>(&img));
    cv::imshow(winname, img);
    for (;;)
        if (cv::waitKey(33) == 27)
            break;

    return 0;
}
