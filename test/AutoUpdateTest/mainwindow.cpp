#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>

#ifdef Q_OS_MAC
    #include <unistd.h>
#endif

#ifdef Q_OS_LINUX
    #include <unistd.h>
#endif

#ifdef Q_OS_WIN
    #include <process.h>
#endif

static const QString APPNAME = "AutoUpdateTest";
static const QString VERSION = "V11.0";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_isCh(true)
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

    QString pid;
#ifdef Q_OS_MAC
    pid = QString::asprintf("%1").arg(getpid());
#endif

#ifdef Q_OS_LINUX
    pid = QString::asprintf("%1").arg(getpid());
#endif

#ifdef Q_OS_WIN
    pid = QString::asprintf("%1").arg(_getpid());
#endif

    arguments << isFirst; //argv[1]
    arguments << isCh; //argv[2]
    arguments << pid; //argv[2]
	qDebug() << arguments;

	QString strExe;
#ifdef Q_OS_MAC
    strExe = QApplication::applicationDirPath() + "/../../src/AutoUpdater/AutoUpdater";
#endif

#ifdef Q_OS_LINUX
    strExe = QApplication::applicationDirPath() + "/../../src/AutoUpdater/AutoUpdater";
#endif

#ifdef Q_OS_WIN
    strExe = QApplication::applicationDirPath() + "/../../../src/AutoUpdater/release/AutoUpdater.exe";
#endif
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
