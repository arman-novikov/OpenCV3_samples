#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*!
    Make an application that reads and displays a video and is controlled by sliders.
    One slider will control the position within the video from start to end in 10
    increments; another binary slider should control pause/unpause. Label both slid‐
    ers appropriately.
*/
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QPushButton *_pause, *_unpause, *_upload;
    QSlider *_frame_navigator;
    QHBoxLayout *_buttons_lay;
    QVBoxLayout *_main_lay;
    QLabel *_img;
    QTimer _img_update_timer;
    int _offset_ms;
    cv::Mat _frame;
    cv::VideoCapture _cap;
    size_t _frame_count;
    bool _is_paused;
    void _set_frame();
    static constexpr int imgWIDTH = 640;
    static constexpr int imgHEIGHT = 360;
    static constexpr int min_frame_navigator_val = 0;
    static constexpr int max_frame_navigator_val = 10;
private slots:
    void _upload_clicked();
    void _pause_clicked();
    void _unpause_clicked();
    void _frame_navigator_changed(int new_value);
    void _img_update_timer_handler();
};

#endif // MAINWINDOW_H
