
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <algorithm>
#include "setupnewtask.h"
#include <QTimer>


MainWindow::MainWindow(QString newPath, QWidget *parent)
    : QMainWindow(parent), path(newPath)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->taskNameLineEdit->setReadOnly(true);
    ui->taskDescriptionTextEdit->setReadOnly(true);
    lay = new QVBoxLayout(this);
    ui->scrollAreaWidgetContents->setLayout(lay);
    lay->setAlignment(Qt::AlignTop);
    ui->AccLabel->setText(this->path);
    this->LoadTaskFromFile();
}

void MainWindow::LoadTaskFromFile()
{
    QString folderPath = this->path;
    QDir folderDir(folderPath);
    if(!folderDir.exists())
    {
        QMessageBox::warning(this, "Error", "Folder doesn't exist");
        return;
    }

    QStringList filters;
    filters<<"*.txt";
    folderDir.setNameFilters(filters);

    QDateTime currentTime = QDateTime::currentDateTime();
    QString mostImportantTasks;

    QStringList fileList = folderDir.entryList();
    foreach (QString fileName, fileList)
    {
        if(fileName == "Password.txt")
        {
            continue;
        }

        QFile file(folderPath + "\\" + fileName);
        if(file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&file);
            QString name = in.readLine();
            QString tempStrCreateDateTime = in.readLine();
            QDateTime createDateTime = QDateTime::fromString(tempStrCreateDateTime, "ddd MMM dd hh:mm:ss yyyy");
            QString tempStrFinalDate = in.readLine();
            QDateTime finalDate = QDateTime::fromString(tempStrFinalDate, "ddd MMM dd hh:mm:ss yyyy");
            int priority = in.readLine().toInt();

            int differenceInTime = currentTime.secsTo(finalDate);
            if(differenceInTime<86400 && differenceInTime>0)
            {
                mostImportantTasks += name + "\n";
            }

            QStringList descriptionLines;
            while(!in.atEnd())
            {
                descriptionLines.append(in.readLine());
            }

            QString description = descriptionLines.join("\n");

            QSharedPointer<QDynamicButton> button = QSharedPointer<QDynamicButton>::create(this, name, description, finalDate, priority);
            button->SetTaskCreateDate(createDateTime);
            buttonVector.push_back(button);
            lay->addWidget(button.data(),0, Qt::AlignmentFlag::AlignTop);
            connect(button.data(), SIGNAL(clicked()), this, SLOT(slotGetNameDescription()));
            file.close();
        }
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
    QStringList fileList = QDir(this->path).entryList(QStringList()<<"*.txt");
    foreach (QString fileName, fileList)
    {
        if(fileName!="Password.txt")
        {
            QFile::remove(this->path + "\\" + fileName);
        }
    }
    this->SaveTaskToFile();
    if(trayIcon!=nullptr)
    {
        delete trayIcon;
    }
}

QString MainWindow::GetPath() const
{
    return this->path;
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
        QSharedPointer<QDynamicButton> button = QSharedPointer<QDynamicButton>::create(this, name, descr, date, priority);
        buttonVector.push_back(button);
        lay->addWidget(button.data(),0, Qt::AlignmentFlag::AlignTop);
        connect(button.data(), SIGNAL(clicked()), this, SLOT(slotGetNameDescription()));
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
                buttonVector.erase(buttonVector.cbegin()+i);
                ui->taskNameLineEdit->clear();
                ui->taskDescriptionTextEdit->clear();
                break;
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

    SetUpNewTask* changeTaskWindow = new SetUpNewTask(this);
    changeTaskWindow->SetNewChangedEnteredName(buttonVector[changeIndex]->GetTaskName());
    changeTaskWindow->SetNewChangedEnderedDescription(buttonVector[changeIndex]->GetTaskDescription());
    changeTaskWindow->SetNewChangedFinalDate(buttonVector[changeIndex]->GetTaskFinalDate());
    changeTaskWindow->SetNewChangedPriority(buttonVector[changeIndex]->GetPriority());
    changeTaskWindow->exec();

    if(changeTaskWindow->result()==QDialog::Accepted)
    {
        buttonVector[changeIndex]->SetTaskName(changeTaskWindow->getEnteredName());
        buttonVector[changeIndex]->SetTaskDescription(changeTaskWindow->getEnteredDescription());
        buttonVector[changeIndex]->SetTaskFinalDate(changeTaskWindow->getFinalDate());
        buttonVector[changeIndex]->SetPriority(changeTaskWindow->GetPriority());
        buttonVector[changeIndex]->SetImage(changeTaskWindow->getEnteredName(), changeTaskWindow->GetPriority());
        buttonVector[changeIndex]->click();

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

void MainWindow::SaveTaskToFile()
{
    for(int i = 0 ; i < buttonVector.size();++i)
    {
        QFile file(this->path + "\\" + buttonVector[i]->GetTaskName() + ".txt");
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out<<buttonVector[i]->GetTaskName() + "\n";
            out<<buttonVector[i]->GetTaskCreationDate().toString() + "\n";
            out<<buttonVector[i]->GetTaskFinalDate().toString() + "\n";
            out<<QString::number(buttonVector[i]->GetPriority()) + "\n";
            out<<buttonVector[i]->GetTaskDescription() + "\n";
            file.close();
        }
    }
}



