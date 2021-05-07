#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "udisk.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void on_pbn_identify_clicked();
private:
    Ui::MainWindow *ui;
    uDisk *m_uDisk = Q_NULLPTR;
};

#endif // MAINWINDOW_H
