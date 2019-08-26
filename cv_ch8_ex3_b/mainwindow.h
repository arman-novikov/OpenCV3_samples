#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <array>
#include <string>

//void on_mouse(int event, int x, int y, int flags, void* data);
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void set_img(const std::string& path);
private:
    static constexpr size_t SUBPIXELS_COUNT = 3U;
    static const QString SUBPIXELS_NAMES[MainWindow::SUBPIXELS_COUNT];
    static const QColor SUBPIXELS_COLORS[MainWindow::SUBPIXELS_COUNT];
    static constexpr size_t MAX_INTENSE = 255U;
    static constexpr int THRESHOLD = 32;
    static constexpr size_t THRESHOLDS_COUNT = MAX_INTENSE / THRESHOLD + 1;
    static const cv::Scalar RED;

    typedef std::array<std::array<size_t, THRESHOLDS_COUNT>, SUBPIXELS_COUNT> pixels_vals_t;
    static void on_mouse(int event, int x, int y, int flags, void* data);

    void _counter(const cv::Mat &roi, pixels_vals_t& out) const;
    static void _summator(const cv::Mat &roi,
                   const int start_row,
                   const int end_row,
                   pixels_vals_t& out);
    void _draw_chart(const pixels_vals_t& pixels_vals);

    const int _threads_count;
    cv::Mat _orig;
};
struct data_t;
#endif // MAINWINDOW_H
