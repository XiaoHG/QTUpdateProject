#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QStorageInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Read()
{
    qDebug() << "----------start----------";
    QString UDiskPath = "";
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes())
    {
        qDebug() << storage.rootPath();
        if(storage.isValid() && storage.isReady())
        {
            UDiskPath = storage.rootPath();
            if(UDiskPath.contains("media"))
            {
                qDebug() << "qstring path:" << UDiskPath;
            }
        }
    }
    ListFile(UDiskPath);
}

void MainWindow::ListFile(QString name)
{
    QDir dir(name);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i < folder_list.size(); ++i)
        ListFile(folder_list.at(i).filePath());

    for(int i = 0; i < file_list.size(); ++i)
        qDebug() << file_list.at(i).fileName();
}

void MainWindow::on_pushButton_clicked()
{
    Read();
}
