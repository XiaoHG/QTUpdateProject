#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chttpdownloadfile.h"

#include <QDesktopWidget>
#include <QFile>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->ui->menuBar->hide();
    this->ui->mainToolBar->hide();
    this->setFixedSize(400, 200);
    QPalette pal;
    pal.setColor(QPalette::Background, QColor(255, 245, 225));
    this->setPalette(pal);
    this->setAutoFillBackground(true);


    QFile fileInfo(QCoreApplication::applicationDirPath());
    QString m_strFileName = fileInfo.fileName();
    qDebug() << m_strFileName;

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::downloadXMLFile()
//{
//    QString strCurrentDir = QDir::currentPath();
//    QString strDownLoad = strCurrentDir + "/download/";

//    QDir diretory(strDownLoad);
//    if(!diretory.exists())//如果路径不存在，则创建
//    {
//        diretory.mkpath(strDownLoad);
//    }

//    CHttpDownloadFile *m_httpXML = new CHttpDownloadFile("http://localhost/updateClientVarsion/update.xml",
//                                                         "", strCurrentDir, this);
//}
