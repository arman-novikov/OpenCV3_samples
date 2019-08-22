#ifndef CHAPTER8_H
#define CHAPTER8_H
#include <include/MetaChapter.h>

class Chapter8 : public MetaChapter
{
public:
    void get_video_codec(const std::string &addr = MetaChapter::VIDEO_PATH);
    int write_data(const std::string &file_name);
    int read_data(const std::string &file_name);
    int exer_1();
    int exer_2();
    int exer_3();
};

#endif // CHAPTER8_H
