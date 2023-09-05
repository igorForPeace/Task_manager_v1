#include "enterwindow.h"
#include "ui_enterwindow.h"
#include <QMessageBox>
#include "registrationwindow.h"
#include <QSqlQuery>
#include <QSqlError>

EnterWindow::EnterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterWindow)
{
    ui->setupUi(this);
    ui->pasLineEdit->setEchoMode(QLineEdit::Password);
}

EnterWindow::~EnterWindow()
{
    delete ui;
}

int EnterWindow::GetAccId() const
{
    return this->accId;
}

void EnterWindow::on_EnterPushButton_clicked()
{
    QString log = ui->logLineEdit->text();
    QString password = ui->pasLineEdit->text();
    if(log.isEmpty()||password.isEmpty())
    {
        QMessageBox::warning(this,"Error", "Fill the lines!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM Users WHERE login = :login "
                  "AND password = :password");
    query.bindValue(":login", log);
    query.bindValue(":password", password);

    if(query.exec())
    {
        if(query.next())
        {
            QMessageBox::information(this, "Access", "Entering the acc " + log);
            this->accId = query.value(0).toInt();
            this->accept();
        }
        else
        {
            QMessageBox::warning(this,"Error", "Acc doesn't create or wrong password!");
            ui->logLineEdit->clear();
            ui->pasLineEdit->clear();
        }
    }
    else
    {
        qDebug()<<"Error: "<<query.lastError().text();
    }
}


void EnterWindow::on_RegistreationPushButton_clicked()
{
    RegistrationWindow* regWindow = new RegistrationWindow(this);
    regWindow->exec();
    delete regWindow;
}


void EnterWindow::on_VisiblecCheckBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->pasLineEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->pasLineEdit->setEchoMode(QLineEdit::Password);
    }
}

