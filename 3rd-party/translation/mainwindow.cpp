#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bTranslator(false)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton, SIGNAL(clicked()),
                    this, SLOT(onTranslation()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTranslation()
{
    if(!m_bTranslator)
    {
        //  翻译文件
        m_qtTranslator.load(":/qzher_en.qm");
        qApp->installTranslator(&m_qtTranslator);
        ui->retranslateUi(this);
    }
    else
    {
        qApp->removeTranslator(&m_qtTranslator);
        ui->retranslateUi(this);
    }
    m_bTranslator = !m_bTranslator;
}

