#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(nullptr, QApplication::desktop()->availableGeometry(), "Main", nullptr);
    MainWindow Edit_weight(nullptr,  QApplication::desktop()->availableGeometry(), "Edit_weight", &w);
    w.Edit_weight=&Edit_weight;
    MainWindow Edit_stud(nullptr,  QApplication::desktop()->availableGeometry(), "Edit_stud", &w);
    w.Edit_stud=&Edit_stud;
    w.show();
    return a.exec();
}
