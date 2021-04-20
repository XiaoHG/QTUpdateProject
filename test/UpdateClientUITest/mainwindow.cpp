#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "updateclientui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    this->setWindowState(Qt::WindowMaximized);
}

void MainWindow::on_btnUpdate_clicked()
{
    UpdateClientUI *update = UpdateClientUI::getInstall();
    update->checkUpdate();
    update->show();
}
