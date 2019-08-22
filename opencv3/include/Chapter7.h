#ifndef CHAPTER7_H
#define CHAPTER7_H
#include "MetaChapter.h"

class Chapter7: public MetaChapter
{
public:
    Chapter7() = default;
    int exer_1();
    int exer_1_uni_maker();
    int exer_1_cond_maker();
    static constexpr size_t HOW_MANY = 4096U;
    typedef std::array<float, Chapter7::HOW_MANY> float_container_t;
    typedef std::array<double, Chapter7::HOW_MANY> double_container_t;
    typedef std::array<uint8_t, Chapter7::HOW_MANY> uint8_container_t;
protected:
    template <typename T>
    void printer(const T &container);
    float_container_t float_ones;
    double_container_t double_ones;
    uint8_container_t uint8_ones;
};

#endif // CHAPTER7_H
