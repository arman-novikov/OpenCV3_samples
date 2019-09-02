#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QMouseEvent>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void _set_img(const cv::Mat& mat);
    void _reset_img();
    enum mode {LINE, RECT, ELLIPSE,};
    QPushButton *_reset, *_line, *_rect, *_ellipse;
    QSlider *_logical_drawing;
    QHBoxLayout *_buttons_lay;
    QGridLayout *_slider_lay;
    QVBoxLayout *_main_lay;
    QLabel *_img, *_and, *_or, *_xor;
    cv::Mat _mat;
    MainWindow::mode _current_mode;
    bool _is_drawing;
    static constexpr int imgWIDTH = 640;
    static constexpr int imgHEIGHT = 360;
private slots:
    void _line_clicked();
    void _rect_clicked();
    void _ellipse_clicked();
};

#endif // MAINWINDOW_H
