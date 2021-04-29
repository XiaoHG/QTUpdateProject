#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800, 500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionactUpdate_triggered()
{
    QString strExe = QDir::currentPath() + "/../../src/AutoUpdater/debug/AutoUpdater.exe";
    qDebug() << strExe;
    QProcess::startDetached(strExe);
}

void MainWindow::on_pushButton_clicked()
{
    QString strExe = QDir::currentPath() + "/../../src/AutoUpdater/debug/AutoUpdater.exe";
    qDebug() << strExe;
    QProcess::startDetached(strExe);
}
