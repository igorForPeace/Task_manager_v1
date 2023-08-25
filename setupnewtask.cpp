#include "setupnewtask.h"
#include "ui_setupnewtask.h"
#include <QMessageBox>

SetUpNewTask::SetUpNewTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetUpNewTask)
{
    ui->setupUi(this);
}

SetUpNewTask::~SetUpNewTask()
{
    delete ui;
}

QString SetUpNewTask::getEnteredName() const
{
    return this->newName;
}

QString SetUpNewTask::getEnteredDescription() const
{
    return this->newDescription;
}

QDateTime SetUpNewTask::getFinalDate() const
{
    return this->finalDate;
}

int SetUpNewTask::GetPriority() const
{
    return this->priority;
}

void SetUpNewTask::SetNewChangedEnteredName(const QString& newName)
{
    ui->NewNameLineEdit->setText(newName);

}

void SetUpNewTask::SetNewChangedEnderedDescription(const QString &newDescription)
{
    ui->NewDescriptionTextEdit->setText(newDescription);
}

void SetUpNewTask::SetNewChangedFinalDate(const QDateTime &newDate)
{
    ui->FinalDate->setDateTime(newDate);
}

void SetUpNewTask::SetNewChangedPriority(int index)
{
    ui->PriorityComboBox->setCurrentIndex(index);
}

void SetUpNewTask::on_SavePushButton_clicked()
{
    this->newName = ui->NewNameLineEdit->text();
    this->newDescription = ui->NewDescriptionTextEdit->toPlainText();
    this->finalDate = ui->FinalDate->dateTime();

    if(finalDate > QDateTime::currentDateTime()&&!this->newName.isEmpty()&&!this->newDescription.isEmpty())
    {
        this->accept();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Check if all fields are filled in and if the date is correct!");
    }
}

void SetUpNewTask::on_PriorityComboBox_activated(int index)
{
    this->priority = index;
}

void SetUpNewTask::on_CancelPushButton_clicked()
{
    this->reject();
}

