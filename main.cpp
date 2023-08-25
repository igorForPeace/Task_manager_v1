
#include "mainwindow.h"
#include "enterwindow.h"
#include <QApplication>
#include <QDate>
#include <QDebug>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug()<<"Start the program";
    EnterWindow enterWindow;
    QString accPath;
    if(enterWindow.exec()!=QDialog::Accepted)
    {
        return 0;
    }
    else
    {
        accPath = enterWindow.GetPath();
    }
    qDebug()<<accPath;

    MainWindow w(accPath);
    w.show();

    //SingleTask s{"task1", "some descr", "20.08.2023", 2};
    //s.ShowDebug();

    return a.exec();
}
