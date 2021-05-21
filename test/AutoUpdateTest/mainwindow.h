#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void StartUpdateProcess(QString isFirst);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *m_updateProcess;
};

#endif // MAINWINDOW_H
