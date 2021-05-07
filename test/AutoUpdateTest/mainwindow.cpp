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

void MainWindow::on_pushButton_clicked()
{
    //这里主程序读取配置文件并且把flag = true;
    WriteConfigFileFlag("1");

    //更新程序在检查到无新版本的时候，再读取配置文件获取flag的值，如果为true则弹窗，否则不弹窗。
    //并且再每次点击菜单栏上的检查更新按钮主程序都将flag设为true，即不管是否有更新都弹窗。
    QString strExe = QDir::currentPath() + "/../../src/AutoUpdater/debug/AutoUpdater.exe";
    qDebug() << strExe;
    QProcess::startDetached(strExe);
}

void MainWindow::WriteConfigFileFlag(const char* flag)
{
    QString strConfigName = QDir::currentPath() + "/updateConfig.txt";
    QFile file(strConfigName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "WriteConfigFileFlag false!";
        return;
    }
    file.write(flag);
}
