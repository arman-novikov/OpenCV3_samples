#include "include/Chapter3.h"

Chapter3::Chapter3()
{
    //ctor
}

/*!
1. Find and open .../opencv/cxcore/include/cxtypes.h. Read through and find the
many conversion helper functions.
    a. Choose a negative floating-point number.
    b. Take its absolute value, round it, and then take its ceiling and floor.
    c. Generate some random numbers.
    d. Create a floating-point cv::Point2f and convert it to an integer cv::Point.
        Convert a cv::Point to a cv::Point2f.

2. Compact matrix and vector types:
    a. Using the cv::Mat33f and cv::Vec3f objects (respectively), create a 3 × 3
        matrix and 3-row vector.
    b. Can you multiply them together directly? If not, why not?

3. Compact matrix and vector template types:
    a. Using the cv::Mat<> and cv::Vec<> templates (respectively), create a 3 × 3
        matrix and 3-row vector.
    b. Can you multiply them together directly? If not, why not?
    c. Try type-casting the vector object to a 3 × 1 matrix, using the cv::Mat<> tem‐
        plate. What happens now?
*/
int Chapter3::exer()
{
    float f = -100500.314;
    std::cout<<"abs: "<<cv::abs(f)<<'\t'
             <<"round: "<<cvRound(f)<<'\t'
             <<"ceiling: "<<cvCeil(f)<<'\t'
             <<"flor: "<<cvFloor(f)<<std::endl;

    auto rng = cvRNG(-1);
    std::cout<<"random real number:"<<cvRandReal(&rng)<<std::endl;
    std::cout<<"random int  number:"<<cvRandInt(&rng)<<std::endl<<std::endl;

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
