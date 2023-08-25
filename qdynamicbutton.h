
#ifndef QDYNAMICBUTTON_H
#define QDYNAMICBUTTON_H

#include <QPushButton>
#include "singletask.h"

class QDynamicButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QDynamicButton(QWidget* parent, QString name, QString description, QDateTime final_date, unsigned int priority);
    ~QDynamicButton();

    QString GetTaskName() const;
    QString GetTaskDescription() const;
    QDateTime GetTaskCreationDate() const;
    QDateTime GetTaskFinalDate() const;
    unsigned int GetPriority() const;

    void SetTaskName(const QString& name);
    void SetTaskDescription(const QString& descr);
    void SetTaskFinalDate(QDateTime date);
    void SetTaskCreateDate(const QDateTime& time);
    void SetPriority(int priority);
    void SetImage(const QString& name, int priority);
private:
    SingleTask task;

};

#endif // QDYNAMICBUTTON_H
