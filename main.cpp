
#include "mainwindow.h"
#include "enterwindow.h"
#include <QApplication>
#include <QDate>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

//adding sql features
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug()<<"Start the program";
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=DESKTOP-JACI7RM;DATABASE=Task_manager_ex_1");
    db.setUserName("testIhor");
    db.setPassword("12345");
    int accId{};
    if(!db.open())
    {
        qDebug()<<db.lastError().text();
    }
    else
    {
        qDebug()<<"db is opened";
        EnterWindow enterWindow;

        if(enterWindow.exec()!=QDialog::Accepted)
        {
            return 0;
        }
        else
        {
            //accPath = enterWindow.GetPath();
            accId = enterWindow.GetAccId();
        }
        qDebug()<<accId;

    }

    MainWindow w(accId);
    w.show();


    qDebug()<<"End the program";
    return a.exec();
}
