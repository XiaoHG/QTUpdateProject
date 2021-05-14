﻿#include "mainwindow.h"
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

    QPushButton *btnTest2_0 = new QPushButton(tr("btnTest2_0") ,centerWidget);
    connect(btnTest2_0, SIGNAL(clicked(bool)), this, SLOT(slotBtnDownloadCliecked()));
    vl->addWidget(btnTest2_0);
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
