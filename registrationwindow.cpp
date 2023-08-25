#include "registrationwindow.h"
#include "ui_registrationwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QDir>

RegistrationWindow::RegistrationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);
    ui->PassworOneLineEdit->setEchoMode(QLineEdit::Password);
    ui->PasswordTwoLineEdit->setEchoMode(QLineEdit::Password);
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}

void RegistrationWindow::on_MakeRegPushButton_clicked()
{
    QString path = "..\\Task_manager_ex_1\\" + ui->LoginLineEdit->text();
    QString password1 = ui->PassworOneLineEdit->text();
    QString password2 = ui->PasswordTwoLineEdit->text();
    if(ui->LoginLineEdit->text().isEmpty()||password1.isEmpty()||password2.isEmpty())
    {
        QMessageBox::warning(this,"Eror","You have to fill all lines!");
        return;
    }
    if(password1!=password2)
    {
        QMessageBox::warning(this, "Error", "Password must be the same!");
        return;
    }
    QDir newFolder;
    if(!newFolder.exists(path))
    {
        if(newFolder.mkdir(path))
        {
            QString filePath = path + "\\Password.txt";
            QFile file(filePath);
            if(file.open(QIODevice::WriteOnly|QIODevice::Text))
            {
                QTextStream stream(&file);
                stream<<password1;
                file.close();
                QMessageBox::information(this, "Info", "We create acc");
                this->close();
            }
            else
            {
                if(newFolder.rmdir(path))
                {
                    QMessageBox::warning(this, "Error", "Cannot create password file. Folder delet");
                }
                else
                {
                    QMessageBox::warning(this, "Error", "Cannot create password file. Folder are not delet");
                }
            }

        }
        else
        {
            QMessageBox::warning(this, "Error", "Cannot create this acc");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "We already have this accaunt");
    }
}


void RegistrationWindow::on_CancelPushButton_clicked()
{
    this->close();
}


void RegistrationWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->PassworOneLineEdit->setEchoMode(QLineEdit::Normal);
        ui->PasswordTwoLineEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->PassworOneLineEdit->setEchoMode(QLineEdit::Password);
        ui->PasswordTwoLineEdit->setEchoMode(QLineEdit::Password);
    }
}

