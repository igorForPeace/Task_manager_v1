#ifndef SETUPNEWTASK_H
#define SETUPNEWTASK_H

#include <QDialog>
#include <QDate>

namespace Ui {
class SetUpNewTask;
}

class SetUpNewTask : public QDialog
{
    Q_OBJECT

public:
    explicit SetUpNewTask(QWidget *parent = nullptr);
    ~SetUpNewTask();
    QString getEnteredName() const;
    QString getEnteredDescription() const;
    QDateTime getFinalDate() const;
    int GetPriority() const;
    void SetNewChangedEnteredName(const QString& newName);
    void SetNewChangedEnderedDescription(const QString& newDescription);
    void SetNewChangedFinalDate(const QDateTime& newDate);
    void SetNewChangedPriority(int index);

private slots:
    void on_SavePushButton_clicked();

    void on_PriorityComboBox_activated(int index);

    void on_CancelPushButton_clicked();

private:
    Ui::SetUpNewTask *ui;
    QString newName;
    QString newDescription;
    int priority;
    QDateTime finalDate;
};

#endif // SETUPNEWTASK_H
