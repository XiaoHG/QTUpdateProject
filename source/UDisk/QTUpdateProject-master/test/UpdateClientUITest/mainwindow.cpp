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
    //this->setWindowState(Qt::WindowMaximized);
    this->resize(800, 600);

    //Update client provide a signal for main window to start application when update over.
    //connect(UpdateClientUI::getInstall(), SIGNAL(sigCloseMainWindow()), this, SLOT(close()));
}

void MainWindow::on_btnUpdate_clicked()
{
    UpdateClientUI::getInstall()->checkUpdate();
}

