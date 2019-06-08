#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QFile"
#include "QDebug"
#include "QMessageBox"
#include <QCloseEvent>



QString currentFile;



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit_Check->setVisible(false);
    connect (ui->action_Open, SIGNAL(triggered()), SLOT(openFile()));    //ПОДКЛЮЧЕНИЕ СИГНАЛОВ К СЛОТАМ
    connect (ui->action_Save, SIGNAL(triggered()), SLOT(saveFile()));    //ПОДКЛЮЧЕНИЕ СИГНАЛОВ К СЛОТАМ
    connect (ui->action_SaveAs, SIGNAL(triggered()), SLOT(saveFileAs()));//ПОДКЛЮЧЕНИЕ СИГНАЛОВ К СЛОТАМ
    connect (ui->action_New, SIGNAL(triggered()), SLOT(newFile()));      //ПОДКЛЮЧЕНИЕ СИГНАЛОВ К СЛОТАМ
    QWidget::setWindowTitle("New.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openFile() //ОТКРЫТИЕ ФАЙЛА
{
    QString s;
    if ((ui->textEdit_Check->toPlainText())!=(ui->edit->toPlainText())) //ПРОВЕРКА НА ИЗМЕНЕНИЯ В ТЕКУЩЕМ ФАЙЛЕ
    {
        QMessageBox *alert = new QMessageBox;
        alert->setWindowTitle("Фаил был изменён");
        alert->setText("Сохранить изменения?");
        alert->setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        alert->setDefaultButton(QMessageBox::Save);
        int ret = alert->exec();
        alert->deleteLater();
        if (ret == QMessageBox::Save) //СОХРАНИТЬ
        {
            if (currentFile.isEmpty()) //ЕСЛИ НЕОБХОДИМО СОЗДАТЬ НОВЫЙ ФАИЛ
                saveFileAs();
            else
                save(currentFile);     //ЕСЛИ ФАИЛ СУЩЕСТВУЕТ
            if (currentFile!="") //ЕСЛИ ФАИЛ СОХРАНЁН УСПЕШНО
            {
                ui->edit->setText("");
                s = QFileDialog::getOpenFileName(this,"Открыть фаил", "/Новая папка/File.txt", "Text Files (*.txt)");
                QFile file(s);
                if (file.open (QIODevice::ReadOnly | QIODevice::Text)) //СЧИТЫВАНИЕ ТЕКСТА С ФАЙЛА
                {
                    currentFile = s;
                    ui->edit->setText ("");
                    QTextStream in(&file);
                    while(!in.atEnd())
                    {
                        QString line = in.readLine();
                        ui->edit->append(line);
                    }
                    file.close ();
                    ui->textEdit_Check->setText(ui->edit->toPlainText());
                }
            }
        }
        if (ret == QMessageBox::Discard) //ЗАБЫТЬ ИЗМЕНЕНИЯ
        {
            s = QFileDialog::getOpenFileName(this,"Открыть фаил", "/Новая папка/File.txt", "Text Files (*.txt)");
            QFile file(s);
            if (file.open (QIODevice::ReadOnly | QIODevice::Text)) //СЧИТЫВАНИЕ ТЕКСТА С ФАЙЛА
            {
                currentFile = s;
                ui->edit->setText ("");
                QTextStream in(&file);
                while(!in.atEnd())
                {
                    QString line = in.readLine();
                    ui->edit->append(line);
                }
                file.close ();
                QWidget::setWindowTitle(s);
                ui->textEdit_Check->setText(ui->edit->toPlainText());
            }
        }
    }
    else
    {
        s = QFileDialog::getOpenFileName(this,"Открыть фаил", "/Новая папка/File.txt", "Text Files (*.txt)");
        QFile file(s);
        if (file.open (QIODevice::ReadOnly | QIODevice::Text)) //СЧИТЫВАНИЕ ТЕКСТА С ФАЙЛА
        {
            int a=file.size();
            if ((a)>1500000)                                   //ПРОВЕРКА РАЗМЕРА ФАЙЛА
            {
                QMessageBox *alert = new QMessageBox;
                alert->setWindowTitle("Ошибка");
                alert->setText("Размер файла слишком велик!");
                alert->setStandardButtons(QMessageBox::Ok);
                alert->setDefaultButton(QMessageBox::Ok);
                int ret = alert->exec();
                alert->deleteLater();
            }
            else
            {
                currentFile = s;
                ui->edit->setText ("");
                QTextStream in(&file);
                while(!in.atEnd())
                {
                    QString line = in.readLine();
                    ui->edit->append(line);
                }
                file.close ();
                QWidget::setWindowTitle(s);
                ui->textEdit_Check->setText(ui->edit->toPlainText());
            }
        }
    }
}


void MainWindow::newFile() //НОВЫЙ ФАИЛ
{
    if ((ui->textEdit_Check->toPlainText())!=(ui->edit->toPlainText()))   //СРАВНЕНИЕ ФАЙЛА С ЕГО ИЗНАЧАЛЬНЫМ СОДЕРЖИМЫМ
    {
        QMessageBox *alert = new QMessageBox;
        alert->setWindowTitle("Фаил был изменён");
        alert->setText("Сохранить изменения?");
        alert->setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        alert->setDefaultButton(QMessageBox::Save);
        int ret = alert->exec();
        alert->deleteLater();
        if (ret == QMessageBox::Save)  //СОХРАНИТЬ
        {
            if (currentFile.isEmpty())
                saveFileAs();
            else
                save(currentFile);
            if (currentFile!="") //ЕСЛИ ФАИЛ СОХРАНЁН УСПЕШНО
            {
                ui->edit->setText("");
                ui->textEdit_Check->setText("");
                QWidget::setWindowTitle("New.txt");
            }
        }
        if (ret == QMessageBox::Discard) //ЗАБЫТЬ ИЗМЕНЕНИЯ
        {
            currentFile = "";
            ui->edit->setText("");
            ui->textEdit_Check->setText("");
            QWidget::setWindowTitle("New.txt");
        }
    }
    else
    {
        currentFile = "";
        ui->edit->setText("");
        ui->textEdit_Check->setText("");
        QWidget::setWindowTitle("New.txt");
    }
}


void MainWindow::save(QString path) //СОХРАНЕНИЕ ИЗМЕНЕНИЙ В ФАЙЛЕ
{
    QFile file(path);
    {
        if (file.open (QIODevice::ReadWrite | QIODevice::Text))
        {
            file.remove ();

            file.close ();

            file.open (QIODevice::ReadWrite | QIODevice::Text);
        }

        QByteArray arr = getEditText ().toLocal8Bit ();

        if (file.write (arr) < arr.length ())
        {
        }
        file.close ();
    }
    ui->textEdit_Check->setText(ui->edit->toPlainText());
}


QString MainWindow::getEditText()
{
    return ui->edit->toPlainText ();
}


void MainWindow::saveFile() //СОХРАНИТЬ ФАИЛ
{
    if (currentFile.isEmpty ()) //ЕСЛИ НЕОБХОДИМО СОЗДАВАТЬ НОВЫЙ ФАИЛ
    {
        saveFileAs ();
    }
    else                        //ЕСЛИ ФАИЛ СУЩЕСТВУЕТ
    {
        save(currentFile);
    }
}



void MainWindow::saveFileAs() //СОХРАНИТЬ КАК НОВЫЙ ФАИЛ
{
    QString s = QFileDialog::getSaveFileName(this,"Сохранитиь фаил", "/Новая папка/File","Текстовый документ(*.txt)");

    if (!s.isEmpty ())
    {
        save(s);
        currentFile = s;
        ui->textEdit_Check->setText(ui->edit->toPlainText());
        QWidget::setWindowTitle(s);
    }
}

void MainWindow::closeEvent(QCloseEvent*e) //ДЕЙСТВИЯ ПРИ ЗАКРЫТИИ ОКНА
{
    if ((ui->textEdit_Check->toPlainText())!=(ui->edit->toPlainText())) //СРАВНЕНИЕ ФАЙЛА С ЕГО ИЗНАЧАЛЬНЫМ СОДЕРЖИМЫМ
    {
        QString s;
        QMessageBox *alert = new QMessageBox;
        alert->setWindowTitle("Фаил был изменён");
        alert->setText("Сохранить изменения?");
        alert->setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        alert->setDefaultButton(QMessageBox::Save);
        int ret = alert->exec();
        alert->deleteLater();
        if (ret == QMessageBox::Save) //СОХРАНИТЬ
        {
            if (currentFile.isEmpty())
                saveFileAs();
            else
                save(currentFile);
            if (currentFile!="")
                e->accept();
            else
                e->ignore();
        }
        if (ret == QMessageBox::Discard) //ЗАБЫТЬ ИЗМЕНЕНИЯ
        {
        }
        if (ret== QMessageBox::Cancel)  //ОТМЕНИТЬ
        {
            e->ignore();
        }
    }
}




