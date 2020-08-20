#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

int main()
{
    float f = -100500.314;
    std::cout<<"abs: "<<cv::abs(f)<<'\t'
             <<"round: "<<cvRound(f)<<'\t'
             <<"ceiling: "<<cvCeil(f)<<'\t'
             <<"flor: "<<cvFloor(f)<<std::endl;

    auto rng = cv::RNG(-1);
    std::cout<<"random real number:"<<rng.uniform(0, 200)<<std::endl;
    std::cout<<"random int  number:"<<rng.uniform(7888.0, 888.9)<<std::endl<<std::endl;

    cv::Point p(1, 3), p_2cast;
    cv::Point2f p2f(2.5, 2.5), p2f_2cast;
    p_2cast = static_cast<cv::Point>(p2f);
    p2f_2cast = static_cast<cv::Point2f>(p);
    std::cout<<"p2f_2_p: "<<p_2cast<<std::endl;
    std::cout<<"p_2_fp: "<<p2f_2cast<<std::endl<<std::endl;

    cv::Matx33f m33f(5);
    cv::Vec3f v3f(7);
    std::cout<<"matx:\n"<<m33f<<std::endl;
    std::cout<<"vect:\n"<<v3f<<std::endl;
    std::cout<<"matx * vect=\n"<<m33f * v3f<<std::endl<<std::endl;
    //std::cout<<"vect * matx=\n"<<v3f * m33f<<std::endl<<std::endl; // impossible

    cv::Matx<int, 3, 3> matx33i(3,3,3);
    cv::Vec<int, 3> vect3i(8,8,8);
    std::cout<<"matx33i * vect3i=\n"<<matx33i * vect3i<<std::endl<<std::endl;

    cv::Matx<int, 3, 1> matx31i;
    //cv::Vec<int, 3> matx31i_2_vect2i = static_cast< cv::Vec<int,3> >(matx31i);
    return 0;
}
