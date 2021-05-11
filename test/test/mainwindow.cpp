#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setCheckable(true);
    ui->pushButton_2->setCheckable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
