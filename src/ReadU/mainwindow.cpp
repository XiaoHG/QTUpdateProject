#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStorageInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_uDisk = new uDisk;
   qApp->installNativeEventFilter(m_uDisk);
   connect(m_uDisk, &uDisk::sigUDiskCome, [=](QString uDiskName){
       //ui->label->setText(tr("U Disk Come!").append(tr("The Disk is ") + uDiskName));
       qDebug() << tr("U Disk Come!").append(tr("The Disk is ") + uDiskName);
   });
   connect(m_uDisk, &uDisk::sigUDiskRemove, [=](){
       //ui->label->setText(tr("U Disk Remove!"));
       qDebug() << tr("U Disk Remove!");
   });

}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_uDisk)
    {
        delete m_uDisk;
        m_uDisk = Q_NULLPTR;
    }

}
