#include "registrationwindow.h"
#include "ui_registrationwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

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
    QString newLogin = ui->LoginLineEdit->text();
    QString password1 = ui->PassworOneLineEdit->text();
    QString password2 = ui->PasswordTwoLineEdit->text();

    if(newLogin.isEmpty()||password1.isEmpty()||password2.isEmpty())
    {
        QMessageBox::warning(this,"Eror","You have to fill all lines!");
        return;
    }
    if(password1!=password2)
    {
        QMessageBox::warning(this, "Error", "Password must be the same!");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT login FROM Users "
                       "WHERE login = :login");
    checkQuery.bindValue(":login", newLogin);
    if(checkQuery.exec()&&checkQuery.next())
    {
        QMessageBox::warning(this, "Error", "We already have this acc");
        return;
    }
    else
    {
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO Users (login, password) "
                            "VALUES (:login, :password)");
        insertQuery.bindValue(":login", newLogin);
        insertQuery.bindValue(":password", password1);
        if(insertQuery.exec())
        {
            QMessageBox::information(this, "Info", "We create acc");
            this->close();
        }
        else
        {
            qDebug()<<"Inserting error: "<<insertQuery.lastError();
        }
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

