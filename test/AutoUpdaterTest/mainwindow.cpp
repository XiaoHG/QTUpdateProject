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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotBtnUpdateCliecked()
{
    AutoUpdaterUI::getInstance()->CheckUpdater(false);
}

void MainWindow::slotBtnDownloadCliecked()
{

}
