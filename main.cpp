#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon icon(":/icons/myicon.ico");
    a.setWindowIcon(icon);

    MainWindow w;

    w.setWindowTitle("Disk info viewer");
    w.show();

    return a.exec();
}
