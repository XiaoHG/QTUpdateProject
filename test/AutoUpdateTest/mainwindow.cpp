#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>

#include <process.h>

static const QString APPNAME = "AutoUpdateTest";
static const QString VERSION = "V0.0";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_isCh(false)
{
    ui->setupUi(this);
    this->resize(800, 500);
    this->setWindowTitle(APPNAME + VERSION);

    m_updateProcess = new QProcess(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString strCh = m_isCh ? "1" : "0";
    StartUpdateProcess("0", strCh);
}

void MainWindow::StartUpdateProcess(QString isFirst, QString isCh)
{
    QStringList arguments;
    QString pid = QString::asprintf("%1").arg(_getpid());
    arguments << isFirst; //argv[1]
    arguments << isCh; //argv[2]
    arguments << pid; //argv[2]
	qDebug() << arguments;
    QString strExe = QApplication::applicationDirPath() + "/../../../src/AutoUpdater/debug/AutoUpdater.exe";
    qDebug() << strExe;
    m_updateProcess->start(strExe, arguments);
}


void MainWindow::on_radioButton_clicked()
{
    m_isCh = true;
}

void MainWindow::on_radioButton_2_clicked()
{
    m_isCh = false;
}
