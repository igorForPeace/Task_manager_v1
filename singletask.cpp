
#include "singletask.h"
#include <QDebug>

SingleTask::SingleTask(QString name, QString description, QDate final_date, unsigned int priority)
    : name(name), description(description), creation_date(QDateTime::currentDateTime())
{
    this->final_date = final_date;
    status = Progress::NEW;
    this->SetPriority(priority);
}

QString SingleTask::GetName() const
{
    return this->name;
}

QString SingleTask::GetDescription() const
{
    return this->description;
}

void SingleTask::SetPriority(unsigned int priority)
{
    switch (priority)
    {
    case 0:
        this->priority = Priority::LOW;
        break;
    case 1:
        this->priority = Priority::MEDIUM;
        break;
    default:
        this->priority = Priority::HIGH;
        break;
    }
}

unsigned int SingleTask::GetPriority() const
{
    return static_cast<unsigned int>(this->priority);
}

void SingleTask::SetStatus(unsigned int status)
{
    switch (status)
    {
    case 0:
    case 1:
        this->status = Progress::NEW;
        break;
    case 2:
        this->status = Progress::IN_PROGRESS;
        break;
    default:
        this->status = Progress::COMPLETED;
        break;
    }
}

void SingleTask::ShowDebug() const
{
    qDebug()<< "---------------------";
    qDebug()<< "Name: " << this->name;
    qDebug()<< "Creation date: " << this->creation_date.toString("dd.MM.yyyy");
    qDebug().noquote()<< "Status: ";
    switch (status)
    {
    case Progress::NEW:
        qDebug()<< "NEW";
        break;
    case Progress::IN_PROGRESS:
        qDebug()<< "IN PROGRESS";
        break;
    case Progress::COMPLETED:
        qDebug()<< "COMPLETED";
        break;
    }
    qDebug().noquote()<< "Priority: ";
    switch (priority)
    {
    case Priority::LOW:
        qDebug()<< "LOW";
        break;
    case Priority::MEDIUM:
        qDebug()<< "MEDIUM";
        break;
    case Priority::HIGH:
        qDebug()<< "HIGH";
        break;
    }
    qDebug()<< "Description: " << this->description;
    qDebug()<< "Final date: " << this->final_date.toString("dd.MM.yyyy");
}

void SingleTask::SetName(const QString &new_name)
{
    name = new_name;
}

void SingleTask::SetDescription(const QString &new_descr)
{
    description = new_descr;
}

QDateTime SingleTask::GetCreationDate() const
{
    return this->creation_date;
}

QDate SingleTask::GetFinalDate() const
{
    return this->final_date;
}

void SingleTask::SetFinalDate(const QDate &date)
{
    this->final_date = date;
}

void SingleTask::SetDateOfCreate(const QDateTime &time)
{
    this->creation_date = time;
}
