#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static void hello();
    QString getEditText();
    void save(QString path);
    void message(QString s);
    void closeEvent(QCloseEvent*e);



private slots:
    void openFile();
    void saveFile();
    void saveFileAs();
    void newFile();



private:
    Ui::MainWindow *ui;
};

#endif
