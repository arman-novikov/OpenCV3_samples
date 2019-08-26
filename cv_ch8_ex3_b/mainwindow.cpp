#include "mainwindow.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QDebug>

static constexpr char winname[] = "pict";

struct data_t {
    cv::Mat img;
    MainWindow* main_window;
};
static data_t g_data;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    _threads_count(static_cast<int>(std::thread::hardware_concurrency()))
{
    this->_draw_chart({});
}

MainWindow::~MainWindow()
{

}

void MainWindow::set_img(const std::string& path)
{
    this->_orig = cv::imread(path.c_str());
    this->_orig.copyTo(g_data.img);
    g_data.main_window = this;

    if (g_data.img.empty()) {
        std::cerr<<"failed to open picture"<<std::endl;
        return;
    }

    cv::namedWindow(winname, cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback(winname, MainWindow::on_mouse, static_cast<void*>(&g_data));
    cv::imshow(winname, g_data.img);

    return;
}

void MainWindow::_draw_chart(const pixels_vals_t& pixels_vals)
{
    QT_CHARTS_USE_NAMESPACE
    QChart *chart = new QChart();
    QBarSeries *series = new QBarSeries();
    QBarSet* subpixels_sets[MainWindow::SUBPIXELS_COUNT];

    for (size_t i = 0; i < MainWindow::SUBPIXELS_COUNT; ++i) {
        QString color(MainWindow::SUBPIXELS_NAMES[i][0]);
        subpixels_sets[i] = new QBarSet(color);
        subpixels_sets[i]->setColor(MainWindow::SUBPIXELS_COLORS[i]);
        for (size_t j = 0; j < THRESHOLDS_COUNT; ++j)
            *subpixels_sets[i] << pixels_vals[i][j];
        series->append(subpixels_sets[i]);
    }

    chart->addSeries(series);
    chart->setTitle("Subpixels spectrum"); // todo: avoid hardcode
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setAnimationDuration(1000);

    QStringList categories;
    for (size_t i = 0; i < MainWindow::THRESHOLDS_COUNT;) {
        QString category = QString::number(MainWindow::THRESHOLD*i);
        category += "..";
        category += QString::number(MainWindow::THRESHOLD*(++i));
        categories << category;
    }
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->setCentralWidget(chartView);
    this->resize(800, 300);
}

void MainWindow::_counter(const cv::Mat &roi, pixels_vals_t& out) const
{
    auto start = std::chrono::system_clock::now();
    for (auto &i : out)
        i.fill(0);

    std::cerr << "threads count = " << this->_threads_count << std::endl;
    const int task_len = roi.rows / this->_threads_count;
    typedef  std::unique_ptr<std::thread> worker_t;
    std::vector<worker_t> workers;
    std::vector<pixels_vals_t> snitches;
    workers.reserve(static_cast<size_t>(this->_threads_count));
    snitches.resize(static_cast<size_t>(this->_threads_count), {});

    for (int i = 0; i < this->_threads_count; ++i) {
        const int start_row = i * task_len;
        int end_row = start_row + task_len;
        if (i == this->_threads_count - 1)
            end_row = roi.rows;

        workers.push_back(std::make_unique<std::thread>
                                    (MainWindow::_summator, std::ref(roi), start_row, end_row,
                                     std::ref(snitches[static_cast<size_t>(i)])));
    }
    for (size_t i = 0; i < static_cast<size_t>(this->_threads_count); ++i)  {
        workers[i]->join();
        for (size_t j = 0; j < MainWindow::SUBPIXELS_COUNT; ++j)
            for (size_t k = 0; k < MainWindow::THRESHOLDS_COUNT; ++k)
                out[j][k] += snitches[i][j][k];
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "time spent: " << elapsed.count() << " ms" << std::endl;

    for (size_t i = 0; i < MainWindow::SUBPIXELS_COUNT; ++i) {
        for (size_t j = 0; j < MainWindow::THRESHOLDS_COUNT; ++j) {
            std::cout << out[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

void MainWindow::_summator(const cv::Mat &roi,
                          const int start_row,
                          const int end_row,
                          pixels_vals_t& out)
{
    for (int r = start_row; r < end_row; ++r) {
        for (int c = 0; c < roi.cols; ++c) {
            cv::Vec3b pix = roi.at<cv::Vec3b>(r, c);
            for (size_t i = 0; i < MainWindow::SUBPIXELS_COUNT; ++i) {
                ++out[i][pix[static_cast<int>(i)]/MainWindow::THRESHOLD];
            }
        }
    }
}

void MainWindow::on_mouse(int event, int x, int y, int flags, void* data)
{
    static constexpr int RIDGE = 1;
    (void)flags;
    static bool is_ready = false;
    static bool is_first_point_ready = false;
    static cv::Point start_point;
    static cv::Point end_point;

    static MainWindow::pixels_vals_t pixels_vals;
    auto casted_data = static_cast<data_t*>(data);
    MainWindow *mainWindow = casted_data->main_window;
    cv::Mat *pict = &casted_data->img;

    if (event == cv::EVENT_LBUTTONDOWN) {
        if (is_ready == true) { // reseting all to begin again
            is_ready = false;
            is_first_point_ready = false;
            mainWindow->_orig.copyTo(*pict);
            cv::imshow(winname, *pict);
            std::cerr << "cleaned" << std::endl;
            return;
        }
        start_point.x = x;
        start_point.y = y;
        is_first_point_ready = true;
        std::cerr << "1st point captured" << std::endl;
        return;
    }
    if (event == cv::EVENT_LBUTTONUP && is_first_point_ready) {
        end_point.x = x;
        end_point.y = y;
        cv::rectangle(*pict, start_point, end_point, MainWindow::RED, RIDGE);
        is_first_point_ready = false;
        is_ready = true;
        cv::imshow(winname, *pict);
        double start_point_dist = std::sqrt(std::pow(start_point.x, 2) + std::pow(start_point.y, 2));
        double end_point_dist = std::sqrt(std::pow(end_point.x, 2) + std::pow(end_point.y, 2));
        if (start_point_dist < end_point_dist) {
            start_point.x += RIDGE;
            start_point.y += RIDGE;
            end_point.x -= RIDGE;
            end_point.y -= RIDGE;
        } else {
            start_point.x -= RIDGE;
            start_point.y -= RIDGE;
            end_point.x += RIDGE;
            end_point.y += RIDGE;
        }
        std::cerr << "roi drawn" << std::endl;
        mainWindow->_counter((*pict)(cv::Rect(start_point, end_point)), pixels_vals);
        mainWindow->_draw_chart(pixels_vals);
        return;
    }
}

const cv::Scalar MainWindow::RED = {0,0,250};
const QString MainWindow::SUBPIXELS_NAMES[MainWindow::SUBPIXELS_COUNT]
    = {"blue", "green" , "red", };
const QColor MainWindow::SUBPIXELS_COLORS[MainWindow::SUBPIXELS_COUNT]
    = {Qt::blue, Qt::green, Qt::red};
