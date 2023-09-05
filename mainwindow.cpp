
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <algorithm>
#include "setupnewtask.h"
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>


MainWindow::MainWindow(int accId, QWidget *parent)
    : QMainWindow(parent), accId(accId)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->taskNameLineEdit->setReadOnly(true);
    ui->taskDescriptionTextEdit->setReadOnly(true);
    lay = new QVBoxLayout(this);
    ui->scrollAreaWidgetContents->setLayout(lay);
    lay->setAlignment(Qt::AlignTop);
    this->LoadTaskFromDataBase();
}

void MainWindow::LoadTaskFromDataBase()
{
    QString mostImportantTasks;
    QDateTime currentTime = QDateTime::currentDateTime();

    QSqlQuery loadQuery;
    loadQuery.prepare("SELECT name, description, creation_date, final_date, priority, user_id "
                      "FROM Tasks "
                      "WHERE user_id = :user_id");
    loadQuery.bindValue(":user_id", this->accId);
    if(loadQuery.exec())
    {
        while(loadQuery.next())
        {
            QString name = loadQuery.value(0).toString();
            QString descr = loadQuery.value(1).toString();
            QDateTime creatDate = loadQuery.value(2).toDateTime();
            QDateTime finalDate = loadQuery.value(3).toDateTime();
            int priority = loadQuery.value(4).toInt();

            int differenceInTime = currentTime.secsTo(finalDate);
            if(differenceInTime<86400 && differenceInTime>0)
            {
                mostImportantTasks += name + "\n";
            }

            QSharedPointer<QDynamicButton> button = QSharedPointer<QDynamicButton>::create(this, name, descr, finalDate, priority);
            button->SetTaskCreateDate(creatDate);
            buttonVector.push_back(button);
            lay->addWidget(button.data(),0, Qt::AlignmentFlag::AlignTop);
            connect(button.data(), SIGNAL(clicked()), this, SLOT(slotGetNameDescription()));
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", loadQuery.lastError().text());
    }
    if(!mostImportantTasks.isEmpty())
    {
        trayIcon = new QSystemTrayIcon(QIcon("..\\Task_manager_ex_1\\icon.png"), nullptr);
        trayIcon->show();
        trayIcon->showMessage("Most Important tasks: ", mostImportantTasks, QSystemTrayIcon::Information, 5000);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if(trayIcon!=nullptr)
    {
        delete trayIcon;
    }
}

int MainWindow::GetAccId() const
{
    return this->accId;
}

void MainWindow::on_AddNewTaskPushButton_clicked()
{
    SetUpNewTask* newTaskWindow = new SetUpNewTask(this);
    newTaskWindow->exec();
    if(newTaskWindow->result()==QDialog::Accepted)
    {
        QString name = newTaskWindow->getEnteredName();
        QString descr = newTaskWindow->getEnteredDescription();
        QDateTime date = newTaskWindow->getFinalDate();
        int priority = newTaskWindow->GetPriority();

        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO Tasks (name, description, creation_date, final_date, priority, user_id) "
                            "VALUES (:name, :description, :creation_date, :final_date, :priority, :user_id)");

        insertQuery.bindValue(":name", name);
        insertQuery.bindValue(":description", descr);
        insertQuery.bindValue(":creation_date", QDateTime::currentDateTime());
        insertQuery.bindValue(":final_date", date);
        insertQuery.bindValue(":priority", priority);
        insertQuery.bindValue(":user_id", this->GetAccId());
        if(insertQuery.exec())
        {
            QSharedPointer<QDynamicButton> button = QSharedPointer<QDynamicButton>::create(this, name, descr, date, priority);
            buttonVector.push_back(button);
            lay->addWidget(button.data(),0, Qt::AlignmentFlag::AlignTop);
            connect(button.data(), SIGNAL(clicked()), this, SLOT(slotGetNameDescription()));
        }
        else
        {
            QMessageBox::warning(this, "Error", insertQuery.lastError().text());
        }

    }
    delete newTaskWindow;
}


void MainWindow::on_RemoveTaskPushButton_clicked()
{
    for(int i = 0 ; i < buttonVector.size(); ++i)
    {
        if(buttonVector[i]->GetTaskName()==ui->taskNameLineEdit->text())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Remove task", "Do you really want to delete \""
                                          + buttonVector[i]->GetTaskName() + "\"",
                                          QMessageBox::Yes|QMessageBox::No);

            if(reply == QMessageBox::Yes)
            {
                QSqlQuery deleteQuery;
                deleteQuery.prepare("DELETE FROM Tasks "
                                    "WHERE name = :name AND description = :description AND user_id = :user_id");
                deleteQuery.bindValue(":name", buttonVector[i]->GetTaskName());
                deleteQuery.bindValue(":description", buttonVector[i]->GetTaskDescription());
                deleteQuery.bindValue(":user_id", this->accId);
                if(deleteQuery.exec())
                {
                    buttonVector.erase(buttonVector.cbegin()+i);
                    ui->taskNameLineEdit->clear();
                    ui->taskDescriptionTextEdit->clear();
                    break;
                }
                else
                {
                    QMessageBox::warning(this, "Error", deleteQuery.lastError().text());
                }
            }
            else
            {
                break;
            }
        }
    }
}

void MainWindow::slotGetNameDescription()
{
    QDynamicButton *button = (QDynamicButton*) sender();
    QString tempPriority;
    switch (button->GetPriority()) {
    case 0:
        tempPriority = "LOW";
        break;
    case 1:
        tempPriority = "MEDIUM";
        break;
    default:
        tempPriority = "HIGH";
    }
    ui->taskNameLineEdit->setText(button->GetTaskName());
    QString tempDescr = "Date of creation: " + button->GetTaskCreationDate().toString() +
                        "\nFinal date: " + button->GetTaskFinalDate().toString() +
                        "\nPriority: " + tempPriority +
                        "\n\nDescription: \n" + button->GetTaskDescription();
    ui->taskDescriptionTextEdit->setText(tempDescr);

}

bool compareButtonsByCreateDate(const QSharedPointer<QDynamicButton> &one, const QSharedPointer<QDynamicButton> &two)
{
    return one->GetTaskCreationDate() < two->GetTaskCreationDate();
}

bool compareButtonsByPriority(const QSharedPointer<QDynamicButton> &one, const QSharedPointer<QDynamicButton> &two)
{
    return one->GetPriority() > two->GetPriority();
}

void MainWindow::on_SortOptionComboBox_activated(int index)
{
    for(int i = 0; i < buttonVector.size(); i++)
    {
        lay->removeWidget(buttonVector[i].data());
    }
    if(index == 0)
    {
        std::sort(buttonVector.begin(), buttonVector.end(), &compareButtonsByCreateDate);
    }
    else if(index == 1)
    {
        std::sort(buttonVector.begin(), buttonVector.end(), &compareButtonsByPriority);
    }
    for(int i = 0; i < buttonVector.size(); i++)
    {
        lay->addWidget(buttonVector[i].data());
    }
}


void MainWindow::on_ChangePushButton_clicked()
{
    int changeIndex =-1;
    for(int i = 0; i < buttonVector.size(); i++)
    {
        if(buttonVector[i]->GetTaskName()==ui->taskNameLineEdit->text())
        {
            changeIndex = i;
            break;
        }
    }

    if(changeIndex==-1)
    {
        QMessageBox::warning(this, "Error", "Choose a task!");
        return;
    }

    QString oldName = buttonVector[changeIndex]->GetTaskName();
    QString oldDescr = buttonVector[changeIndex]->GetTaskDescription();

    SetUpNewTask* changeTaskWindow = new SetUpNewTask(this);
    changeTaskWindow->SetNewChangedEnteredName(buttonVector[changeIndex]->GetTaskName());
    changeTaskWindow->SetNewChangedEnderedDescription(buttonVector[changeIndex]->GetTaskDescription());
    changeTaskWindow->SetNewChangedFinalDate(buttonVector[changeIndex]->GetTaskFinalDate());
    changeTaskWindow->SetNewChangedPriority(buttonVector[changeIndex]->GetPriority());
    changeTaskWindow->exec();

    if(changeTaskWindow->result()==QDialog::Accepted)
    {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE Tasks SET name = :name, description = :description, final_date = :final_date, priority = :priority "
                            "WHERE name = :oldName AND description = :oldDescription AND user_id = :user_id");
        updateQuery.bindValue(":name", changeTaskWindow->getEnteredName());
        updateQuery.bindValue(":description", changeTaskWindow->getEnteredDescription());
        updateQuery.bindValue(":final_date", changeTaskWindow->getFinalDate());
        updateQuery.bindValue(":priority", changeTaskWindow->GetPriority());
        updateQuery.bindValue(":oldName", oldName);
        updateQuery.bindValue(":oldDescription", oldDescr);
        updateQuery.bindValue(":user_id", this->accId);
        if(updateQuery.exec())
        {
            buttonVector[changeIndex]->SetTaskName(changeTaskWindow->getEnteredName());
            buttonVector[changeIndex]->SetTaskDescription(changeTaskWindow->getEnteredDescription());
            buttonVector[changeIndex]->SetTaskFinalDate(changeTaskWindow->getFinalDate());
            buttonVector[changeIndex]->SetPriority(changeTaskWindow->GetPriority());
            buttonVector[changeIndex]->SetImage(changeTaskWindow->getEnteredName(), changeTaskWindow->GetPriority());
            buttonVector[changeIndex]->click();
        }
        else
        {
            QMessageBox::warning(this, "Error", updateQuery.lastError().text());
        }
    }
    delete changeTaskWindow;
}

void MainWindow::on_searchLineEdit_textEdited(const QString &arg1)
{
    for(int i = 0; i < buttonVector.size(); i++)
    {
        buttonVector[i]->show();
    }

    for(int i = 0; i < buttonVector.size();i++)
    {
        if(!buttonVector[i]->GetTaskName().toLower().contains(arg1))
        {
            buttonVector[i]->hide();
        }
    }
}





