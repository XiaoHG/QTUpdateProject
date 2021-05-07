#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "autoupdaterui.h"

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centerWidget = new QWidget(this);
    this->setCentralWidget(centerWidget);
    QVBoxLayout *vl = new QVBoxLayout(centerWidget);

    QPushButton *btnUpdate = new QPushButton(tr("update") ,centerWidget);
    connect(btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotBtnUpdateCliecked()));
    vl->addWidget(btnUpdate);

    QPushButton *btnDownload = new QPushButton(tr("btnDownload") ,centerWidget);
    connect(btnDownload, SIGNAL(clicked(bool)), this, SLOT(slotBtnDownloadCliecked()));
    vl->addWidget(btnDownload);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::slotBtnUpdateCliecked()
{
    //主程序开始启动，在调用更新程序之前都将配置文件中的flag设置为false，表示如果没有更新则更新程序不要弹窗
    //flag = false;
    WriteConfigFileFlag("1");
    AutoUpdaterUI::getInstance()->CheckUpdater();
}

void MainWindow::slotBtnDownloadCliecked()
{
    AutoUpdaterUI::getInstance()->testInterface();
}
