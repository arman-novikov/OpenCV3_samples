#include "include/Chapter4.h"

Chapter4::Chapter4()
{
    //ctor
}

int Chapter4::matIndAccess()
{
    const char winName[] = "res";
    const int ROWS = 20, COLUMNS = 10;
    const cv::Scalar sBlue(220, 22, 22), sGreen(22, 220, 22), sRed(22, 22, 220);
    //cv::Mat frame = cv::Mat::zeros(ROWS, COLUMNS, CV_8UC3);
    cv::Mat frame = cv::Mat(ROWS, COLUMNS, CV_8UC3, sRed);

    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLUMNS; ++c) {
            //std::cout << frame.at<cv::Vec3b>(r,c)<<std::endl;
            frame.at<cv::Vec3b>(r,c)[0] = 220;
            frame.at<cv::Vec3b>(r,c)[1] = 220;
            frame.at<cv::Vec3b>(r,c)[2] = 22;
        }

    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, frame);
    while (cv::waitKey(33) == 255);

    return 0;
}

int Chapter4::sparseMatExm()
{
    int size[] = { 10,10 };
    cv::SparseMat sm(2, size, CV_32F);
/*
    for (int i = 0; i < 10; i++) { // Fill the array
        int idxx[2];
        idxx[0] = size[0] * rand();
        idxx[1] = size[1] * rand();
        //sm.ref<float>(idxx) += 1.0f; // -Wzero-as-null

        int idxc[2] = { size[0] * rand(), size[1] * rand() };
        //sm.ref<float>(idxc) += 3.0f; // -Wzero-as-null
    }
*/
    cv::SparseMatConstIterator_<float> it = sm.begin<float>();
    cv::SparseMatConstIterator_<float> it_end = sm.end<float>();
    for (; it != it_end; ++it) {
        const cv::SparseMat::Node* node = it.node();
        std::cout<<node->idx[0]<<","<<node->idx[1]<<": "<< *it<<std::endl;
    }

    return 0;
}

int Chapter4::exer_1()
{
    std::cout<< "see cvChapter4Ex" << std::endl;
    return 0;
}

/*!
1. Create a 500 × 500 single channel uchar image with every pixel equal to zero.
    a. Create an ASCII numeric typewriter where you can type numbers into your
        computer and have the number show up in a 20-pixel-high by 10-pixel-wide
        block. As you type, the numbers will display from left to right until you hit the
        end of the image. Then just stop.
    b. Allow for carriage return and backspace.
    c. Allow for arrow keys to edit each number.
    d. Create a key that will convert the resulting image into a color image, each
        number taking on a different color.
2. We want to create a function that makes it efficient to sum up rectangular
    regions in an image by creating a statistics image where each “pixel” holds the
    sum of the rectangle from that point to the image origin. These are called integral
    images and by using just 4 points from the integral image, you can determine the
    sum of any rectangle in the image.
    a. Create a 100 × 200 single-channel uchar image with random numbers. Create
        a 100 × 200 single-channel float “integral image” with all members equal to
        zero.
    b. Fill in each element of the integral image with the corresponding sum of the
        rectangle from that pixel to the origin in the original uchar image.
*/
int Chapter4::exer_2()
{
    const bool DEBUG = false;
    const int ROWS = 100, COLUMNS = 200;
    CvRNG rng = cvRNG(-1);
    cv::Mat_<unsigned char> source(ROWS, COLUMNS);
    cv::Mat_<float> integralImage = cv::Mat::zeros(ROWS, COLUMNS, CV_32FC1);

    for (auto &elem: source)
        elem = cvRandInt(&rng) % 0xff; //[0..255]

    if (DEBUG)
        std::cout<<source<<std::endl<<std::endl<<std::endl;

    integralImage(0,0) = source(0,0);
    for (int j = 1; j < COLUMNS; ++j) {
        integralImage(0,j) = integralImage(0,j-1) + source(0,j);
    }
    for (int i = 1; i < ROWS; ++i) {
        integralImage(i,0) = integralImage(i-1,0) + source(i,0);
    }
    for (int i = 1; i < ROWS; ++i) {
        for (int j = 1; j < COLUMNS; ++j) {
            integralImage(i,j) = source(i,j) + integralImage(i-1,j) + integralImage(i,j-1) - integralImage(i-1,j-1);
        }
    }

    if (DEBUG)
        std::cout<<integralImage<<std::endl<<std::endl<<std::endl;

    auto rectSum = [](const auto &mat,
                           const int upperLeftRow, const int upperLeftCol,
                           const int width, const int height)
    {
        /*B...C
          A...D*/
        const int Br = upperLeftRow - 1;
        const int Bc = upperLeftCol - 1;
        const int Ar = Br + height;
        const int Ac = Bc;
        const int Cr = Br;
        const int Cc = Bc + width;
        const int Dr = Br + height;
        const int Dc = Bc + width;
        if (DEBUG && 0)
            std::cout<<"B: "<<mat(Br, Bc)<<"\tC: "<<mat(Cr, Cc)
               <<"\n"<<"A: "<<mat(Ar, Ac)<<"\tD: "<<mat(Dr, Dc)<<"\n\n\n";

        if (Br >= 0 && Bc >= 0)
            return mat(Dr, Dc) + mat(Br, Bc) - mat(Cr, Cc) - mat(Ar, Ac);
        else if (Br >= 0 && Bc < 0)
            return mat(Dr, Dc) - mat(Cr, Cc);
        else if (Br < 0 && Bc >= 0)
            return mat(Dr, Dc) - mat(Ar, Ac);
        else
            return mat(Dr, Dc);

    };

    std::cout <<"res: "<<rectSum(integralImage, 0, 0, 55, 55)<<std::endl;
    std::cout <<"res: "<<rectSum(integralImage, 0, 2, 55, 55)<<std::endl;
    std::cout <<"res: "<<rectSum(integralImage, 2, 0, 55, 55)<<std::endl;
    std::cout <<"res: "<<rectSum(integralImage, 22, 33, 55, 55)<<std::endl;
    return 0;
}
