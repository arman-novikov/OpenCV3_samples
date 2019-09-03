#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    _img_pos(0,0),
    _drawing_start_pos(0,0),
    _mat(MainWindow::imgHEIGHT, MainWindow::imgWIDTH, CV_8UC3, cv::Scalar(0,0,0)),
    _buf(MainWindow::imgHEIGHT, MainWindow::imgWIDTH, CV_8UC3, cv::Scalar(0,0,0)),
    _current_mode(MainWindow::Mode::LINE),
    _is_drawing(false)
{
    QWidget *central_widget = new QWidget();

    this->_reset = new QPushButton("reset");
    this->_line = new QPushButton("line");
        this->_line->setEnabled(false);
    this->_rect = new QPushButton("rect");
    this->_ellipse = new QPushButton("ellipse");

    this->_buttons_lay = new QHBoxLayout();
        this->_buttons_lay->addWidget(this->_reset);
        this->_buttons_lay->addWidget(this->_line);
        this->_buttons_lay->addWidget(this->_rect);
        this->_buttons_lay->addWidget(this->_ellipse);

    this->_logical_drawing = new QSlider(Qt::Orientation::Horizontal);
        this->_logical_drawing->setRange(0,2);
        this->_logical_drawing->setSingleStep(1);

    this->_and = new QLabel("AND");
    this->_or = new QLabel("OR");
    this->_xor = new QLabel("XOR");
    this->_slider_lay = new QGridLayout();
        this->_slider_lay->addWidget(this->_logical_drawing, 0, 0, Qt::AlignHCenter);
        this->_slider_lay->addWidget(this->_and, 1, 0, 1, 1, Qt::AlignLeft);
        this->_slider_lay->addWidget(this->_or, 1, 1, Qt::AlignHCenter);
        this->_slider_lay->addWidget(this->_xor, 1, 2, Qt::AlignRight);
        this->_slider_lay->setSpacing(0);

    this->_img = new QLabel();
        this->_img->setFixedSize(QSize(MainWindow::imgWIDTH, MainWindow::imgHEIGHT));

    this->_main_lay = new QVBoxLayout();
        this->_main_lay->addLayout(this->_buttons_lay);
        this->_main_lay->addWidget(this->_logical_drawing);
        this->_main_lay->addLayout(this->_slider_lay);
        this->_main_lay->addWidget(this->_img);

    this->_reset_img();
    central_widget = new QWidget();
    central_widget->setLayout(this->_main_lay);
    central_widget->layout()->setMargin(MainWindow::MARGIN);
    this->setCentralWidget(central_widget);

    connect(this->_reset, &QPushButton::clicked, [this](){this->_reset_img();});
    connect(this->_line, SIGNAL(clicked()), this, SLOT(_line_clicked()));
    connect(this->_rect, SIGNAL(clicked()), this, SLOT(_rect_clicked()));
    connect(this->_ellipse, SIGNAL(clicked()), this, SLOT(_ellipse_clicked()));

    this->setMouseTracking(true);
}

void MainWindow::_set_img(const cv::Mat& mat)
{
    cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);

    QImage img = QImage(static_cast<const uint8_t*>(mat.data),
                        mat.cols, mat.rows,
                        static_cast<int>(mat.step), QImage::Format_RGB888);
    QPixmap pixmap(MainWindow::imgWIDTH, MainWindow::imgHEIGHT);
    pixmap.convertFromImage(img);
    this->_img->setPixmap(pixmap);
}

void MainWindow::_reset_img()
{
    this->_mat = cv::Mat(MainWindow::imgHEIGHT, MainWindow::imgWIDTH, CV_8UC3, cv::Scalar(0,0,0));
    this->_set_img(this->_mat);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(this->_is_drawing) {
        this->_mat.copyTo(this->_buf);
        this->_draw(e->pos() - this->_img_pos);
        this->_set_img(this->_buf);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{    
    QPoint click_point = e->pos();
    e->accept();

    if (this->childAt(click_point) != this->_img &&
        e->button() != Qt::LeftButton)
        return;

    this->_img_pos = this->_img->mapTo(this, this->_img->rect().topLeft());
    this->_mat.copyTo(this->_buf);
    this->_drawing_start_pos = click_point - this->_img_pos;
    this->_is_drawing = true;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        this->_is_drawing = false;
        this->_buf.copyTo(this->_mat);
    }
}

void MainWindow::_line_clicked()
{
    this->_line->setEnabled(false);
    this->_rect->setEnabled(true);
    this->_ellipse->setEnabled(true);
    this->_current_mode = MainWindow::Mode::LINE;
}

void MainWindow::_rect_clicked()
{
    this->_line->setEnabled(true);
    this->_rect->setEnabled(false);
    this->_ellipse->setEnabled(true);
    this->_current_mode = MainWindow::Mode::RECT;
}

void MainWindow::_ellipse_clicked()
{
    this->_line->setEnabled(true);
    this->_rect->setEnabled(true);
    this->_ellipse->setEnabled(false);
    this->_current_mode = MainWindow::Mode::ELLIPSE;
}

void MainWindow::_draw(QPoint dest_point)
{
    static const cv::Scalar color = cv::Scalar(250, 250, 250);
    cv::Point cv_start_point(this->_drawing_start_pos.x(),
                             this->_drawing_start_pos.y());
    cv::Point cv_dest_point(dest_point.x(), dest_point.y());

    switch (this->_current_mode) {
    case MainWindow::Mode::LINE:
        cv::line(this->_buf, cv_start_point, cv_dest_point, color, 4);
        break;
    case MainWindow::Mode::RECT:
        cv::rectangle(this->_buf, cv_start_point, cv_dest_point, color, 4);
        break;
    case MainWindow::Mode::ELLIPSE:
        auto width = qAbs(cv_dest_point.x - cv_start_point.x);
        auto height = qAbs(cv_dest_point.y - cv_start_point.y);
        cv::Point2f center = cv::Point2f(cv_start_point.x + width/2,
                                         cv_start_point.y + height/2);
        cv::Size size = cv::Size(width, height);
        auto region = cv::RotatedRect(center, size, 0);
        cv::ellipse(this->_buf, region, color);
        break;
    }
}

MainWindow::~MainWindow()
{

}
