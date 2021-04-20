#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <updateclientui.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void init();

private slots:
    void on_btnUpdate_clicked();

private:
    Ui::MainWindow *ui;
    UpdateClientUI *upCUI;
};

#endif // MAINWINDOW_H
