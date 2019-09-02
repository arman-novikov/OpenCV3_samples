#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    _mat(MainWindow::imgHEIGHT, MainWindow::imgWIDTH, CV_8UC3, cv::Scalar(0,0,0)),
    _current_mode(MainWindow::mode::LINE),
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
    this->setCentralWidget(central_widget);
    this->setFixedSize(QSize(MainWindow::imgWIDTH+20, MainWindow::imgHEIGHT + 100));

    connect(this->_reset, &QPushButton::clicked, [this](){this->_reset_img();});
    connect(this->_line, SIGNAL(clicked()), this, SLOT(_line_clicked()));
    connect(this->_rect, SIGNAL(clicked()), this, SLOT(_rect_clicked()));
    connect(this->_ellipse, SIGNAL(clicked()), this, SLOT(_ellipse_clicked()));
    this->setMouseTracking(true);
}

void MainWindow::_set_img(const cv::Mat& mat)
{
    cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
    cv::resize(mat, mat, cv::Size(MainWindow::imgWIDTH, MainWindow::imgHEIGHT), 0, 0, cv::INTER_CUBIC);
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
    static int i = 0;
    qDebug() << "in mousemove event" << i++;
    if(e->button() == Qt::RightButton)
    {
        qDebug() << "Right mouse click!" << endl;
        e->accept();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QWidget * w_ptr = this->childAt(e->globalPos());
    if (w_ptr != this->_img)
        return;
    qDebug() << "in mousepress event";
    e->accept();
}

void MainWindow::_line_clicked()
{
    this->_line->setEnabled(false);
    this->_rect->setEnabled(true);
    this->_ellipse->setEnabled(true);
    this->_current_mode = MainWindow::mode::LINE;
}

void MainWindow::_rect_clicked()
{
    this->_line->setEnabled(true);
    this->_rect->setEnabled(false);
    this->_ellipse->setEnabled(true);
    this->_current_mode = MainWindow::mode::RECT;
}

void MainWindow::_ellipse_clicked()
{
    this->_line->setEnabled(true);
    this->_rect->setEnabled(true);
    this->_ellipse->setEnabled(false);
    this->_current_mode = MainWindow::mode::ELLIPSE;
}

MainWindow::~MainWindow()
{

}
