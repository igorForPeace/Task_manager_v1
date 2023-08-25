
#include "qdynamicbutton.h"
#include "singletask.h"

QDynamicButton::QDynamicButton(QWidget* parent, QString name,
                               QString description, QDateTime final_date, unsigned int priority)
    :QPushButton(parent), task(name, description, final_date, priority)
{
//    if(priority == 0)
//    {
//        this->setStyleSheet("background-color: #ADFF2F; font-weight: bold");
//    }
//    else if(priority == 1)
//    {
//        this->setStyleSheet("background-color: yellow;font-weight: bold");
//    }
//    else
//    {
//        this->setStyleSheet("background-color: #f23a0c;font-weight: bold");
//    }
//    this->setText("name: " + name);

    this->SetImage(name, priority);
}

QDynamicButton::~QDynamicButton()
{

}

QString QDynamicButton::GetTaskName() const
{
    return this->task.GetName();
}

QString QDynamicButton::GetTaskDescription() const
{
    return this->task.GetDescription();
}

QDateTime QDynamicButton::GetTaskCreationDate() const
{
    return this->task.GetCreationDate();
}

QDateTime QDynamicButton::GetTaskFinalDate() const
{
    return this->task.GetFinalDate();
}

unsigned int QDynamicButton::GetPriority() const
{
    return this->task.GetPriority();
}

void QDynamicButton::SetTaskName(const QString &name)
{
    this->task.SetName(name);
}

void QDynamicButton::SetTaskDescription(const QString &descr)
{
    this->task.SetDescription(descr);
}

void QDynamicButton::SetTaskFinalDate(QDateTime date)
{
    this->task.SetFinalDate(date);
}

void QDynamicButton::SetTaskCreateDate(const QDateTime &time)
{
    this->task.SetDateOfCreate(time);
}

void QDynamicButton::SetPriority(int priority)
{
    this->task.SetPriority(priority);
}

void QDynamicButton::SetImage(const QString &name, int priority)
{
    if(priority == 0)
    {
        this->setStyleSheet("background-color: #ADFF2F; font-weight: bold");
    }
    else if(priority == 1)
    {
        this->setStyleSheet("background-color: yellow;font-weight: bold");
    }
    else
    {
        this->setStyleSheet("background-color: #f23a0c;font-weight: bold");
    }
    this->setText("name: " + name);
}
