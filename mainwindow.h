
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "qdynamicbutton.h"
#include <QSharedPointer>
#include <QVBoxLayout>
#include <QSystemTrayIcon>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(int accId, QWidget *parent = nullptr);
    ~MainWindow();
    //QString GetPath() const;

    int GetAccId() const;

private slots:
    void on_AddNewTaskPushButton_clicked();

    void on_RemoveTaskPushButton_clicked();

    void slotGetNameDescription();

    void on_SortOptionComboBox_activated(int index);

    void on_ChangePushButton_clicked();

    void on_searchLineEdit_textEdited(const QString &arg1);

private:
    void SaveTaskToFile();
    void LoadTaskFromFile();
    void LoadTaskFromDataBase();
    //QString path;
    int accId;
    Ui::MainWindow *ui;
    QVBoxLayout* lay;
    QSystemTrayIcon *trayIcon = nullptr;

protected:
    QVector<QSharedPointer<QDynamicButton>> buttonVector;
};

#endif // MAINWINDOW_H
