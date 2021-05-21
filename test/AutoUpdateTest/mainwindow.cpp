#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>

static const QString APPNAME = "AutoUpdateTest";
static const QString VERSION = "V0.0";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    StartUpdateProcess("0");
}

void MainWindow::StartUpdateProcess(QString isFirst)
{
    QStringList arguments;
    arguments << isFirst; //argv[1]
    QString strExe = QApplication::applicationDirPath() + "/../../../src/AutoUpdater/debug/AutoUpdater.exe";
    qDebug() << strExe;
    m_updateProcess->start(strExe, arguments);
}

