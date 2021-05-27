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

    /**
     * @brief StartUpdateProcess
     * @param isFirst not "0" is from main function, or others
     * @param isCh "0" is mean current language is english, or chinese.
     */
    void StartUpdateProcess(QString isFirst, QString isCh);
private slots:
    void on_pushButton_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *m_updateProcess;
    bool m_isCh;
};

#endif // MAINWINDOW_H
