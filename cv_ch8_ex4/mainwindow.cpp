#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <iostream>
#include <QImage>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    _offset_ms(0), _frame_count(0), _is_paused(true)
{
    this->_img = new QLabel();
        this->_img->setFixedSize(QSize(MainWindow::imgWIDTH, MainWindow::imgHEIGHT));
    this->_pause = new QPushButton("pause");
        this->_pause->setEnabled(false);
    this->_unpause = new QPushButton("unpause");
        this->_unpause->setEnabled(false);
    this->_upload = new QPushButton("upload video");
    this->_buttons_lay = new QHBoxLayout();
        this->_buttons_lay->addWidget(this->_pause);
        this->_buttons_lay->addWidget(this->_unpause);
        this->_buttons_lay->addWidget(this->_upload);
    this->_frame_navigator = new QSlider(Qt::Orientation::Horizontal);
        this->_frame_navigator->setMinimum(MainWindow::min_frame_navigator_val);
        this->_frame_navigator->setMaximum(MainWindow::max_frame_navigator_val);
    this->_main_lay = new QVBoxLayout();
        this->_main_lay->addLayout(this->_buttons_lay);
        this->_main_lay->addWidget(this->_frame_navigator);
        this->_main_lay->addWidget(this->_img);
    QWidget *central_widget = new QWidget();
    central_widget->setLayout(this->_main_lay);
    this->setCentralWidget(central_widget);
    this->setFixedSize(QSize(MainWindow::imgWIDTH, MainWindow::imgHEIGHT+190));
    this->setWindowTitle("OpenCV3, ch_8_ex4");

    connect(this->_upload, SIGNAL(clicked()), this, SLOT(_upload_clicked()));
    connect(this->_pause, SIGNAL(clicked()), this, SLOT(_pause_clicked()));
    connect(this->_unpause, SIGNAL(clicked()), this, SLOT(_unpause_clicked()));
    connect(this->_frame_navigator, SIGNAL(valueChanged(int)),
            this, SLOT(_frame_navigator_changed(int)));
    connect(&this->_img_update_timer, SIGNAL(timeout()),
            this, SLOT(_img_update_timer_handler()));
}

void MainWindow::_upload_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Video"), "", tr("Video Files (*.avi, *.mp4)"));
    if (filename.isEmpty())
        return;

    this->_cap.open(filename.toLocal8Bit().data());
    if (!this->_cap.isOpened()) {
        std::cerr << "the video is empty";
        return;
    }
    this->_img_update_timer.stop();
    this->_cap >> this->_frame;
    this->_set_frame();
    this->_frame_count = static_cast<size_t>
            (this->_cap.get(CV_CAP_PROP_FRAME_COUNT));
    const int FPS = static_cast<int>(this->_cap.get(cv::CAP_PROP_FPS));
    this->_offset_ms = 1000 / FPS;
    this->_is_paused = true;
    this->_unpause->setEnabled(true);
    this->_pause->setEnabled(false);
}

void MainWindow::_pause_clicked()
{
    this->_img_update_timer.stop();
    this->_pause->setEnabled(false);
    this->_unpause->setEnabled(true);
    this->_is_paused = true;
}

void MainWindow::_unpause_clicked()
{
    this->_pause->setEnabled(true);
    this->_unpause->setEnabled(false);
    this->_is_paused = false;
    this->_img_update_timer.start(this->_offset_ms);
}

void MainWindow::_frame_navigator_changed(int new_value)
{
    const double frame_count = static_cast<double>(this->_frame_count);
    const double max_navig_val = static_cast<double>(MainWindow::max_frame_navigator_val);
    const double offset = static_cast<double>(new_value);
    const double frame_num_to_set = frame_count / max_navig_val * offset;

    this->_img_update_timer.stop();
    this->_cap.set(cv::CAP_PROP_POS_FRAMES, frame_num_to_set);
    this->_cap >> this->_frame;
    this->_img_update_timer.start(this->_offset_ms);
}

void MainWindow::_img_update_timer_handler()
{
    if (!this->_frame.empty() && !this->_is_paused) {
        this->_cap >> this->_frame;
        this->_set_frame();
        this->_img_update_timer.start(this->_offset_ms);
    } else {
        this->_img_update_timer.stop();
    }
}

void MainWindow::_set_frame()
{
    auto &mat = this->_frame;
    if (mat.empty())
        return;

    cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
    cv::resize(mat, mat, cv::Size(MainWindow::imgWIDTH, MainWindow::imgHEIGHT), 0, 0, cv::INTER_CUBIC);
    QImage img = QImage(static_cast<const uint8_t*>(mat.data),
                        mat.cols, mat.rows,
                        static_cast<int>(mat.step), QImage::Format_RGB888);
    QPixmap pixmap(MainWindow::imgWIDTH, MainWindow::imgHEIGHT);
    pixmap.convertFromImage(img);
    this->_img->setPixmap(pixmap);
}
MainWindow::~MainWindow()
{

}
