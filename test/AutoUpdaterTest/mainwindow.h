#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void WriteConfigFileFlag(const char* flag);

public slots:
    void slotBtnUpdateCliecked();

    void slotBtnDownloadCliecked();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
