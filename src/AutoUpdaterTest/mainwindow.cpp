#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "autoupdaterui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    AutoUpdaterUI::getInstance()->CheckUpdate();
}

MainWindow::~MainWindow()
{
    delete ui;
}
