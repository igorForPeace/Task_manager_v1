
#ifndef SINGLETASK_H
#define SINGLETASK_H

#include <QString>
#include <QDate>

enum class Progress { NEW = 1, IN_PROGRESS, COMPLETED };
enum class Priority {LOW = 0, MEDIUM = 1, HIGH = 2};

class SingleTask
{
private:
    QString name;
    QString description;
    QDateTime creation_date;
    QDate final_date;
    Progress status;
    Priority priority;

public:
    SingleTask(QString name, QString description, QDate final_date, unsigned int priority);
    QString GetName() const;
    QString GetDescription() const;
    void SetPriority(unsigned int priority);
    unsigned int GetPriority() const;
    void SetStatus(unsigned int status);
    void ShowDebug() const;
    void SetName(const QString& new_name);
    void SetDescription(const QString& new_descr);
    QDateTime GetCreationDate() const;
    QDate GetFinalDate() const;
    void SetFinalDate(const QDate& date);
    void SetDateOfCreate(const QDateTime& time);
};

#endif // SINGLETASK_H
