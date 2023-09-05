#ifndef ENTERWINDOW_H
#define ENTERWINDOW_H

#include <QDialog>

namespace Ui {
class EnterWindow;
}

class EnterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EnterWindow(QWidget *parent = nullptr);
    ~EnterWindow();

    int GetAccId() const;

private slots:
    void on_EnterPushButton_clicked();

    void on_RegistreationPushButton_clicked();

    void on_VisiblecCheckBox_stateChanged(int arg1);

private:
    Ui::EnterWindow *ui;

    int accId;
};

#endif // ENTERWINDOW_H
