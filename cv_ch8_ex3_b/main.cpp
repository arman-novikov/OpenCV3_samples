#include "mainwindow.h"
#include <QApplication>
#include <thread>

const std::string path("/home/arman/3.jpg");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.set_img(path);

    w.show();
    return a.exec();
}
