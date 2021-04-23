﻿#include "updateclientfilecontroler.h"
#include "varsioninfocontroler.h"
#include "updateclientui.h"
#include "serverrequest.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QMovie>

UpdateClientUI::UpdateClientUI(QWidget *parent)
    :QDialog(parent)
{
    init();
}

UpdateClientUI::~UpdateClientUI()
{
}

/*init all*/
void UpdateClientUI::init()
{
    initUI();
}

/*UI defined*/
void UpdateClientUI::initUI()
{
    updateFiles = new QStringList;
//    QWidget *centerWidget = new QWidget(this);
    //this->setStyleSheet("background-color:rgb(100, 100, 100);border-radius: 10px;");
    this->setStyleSheet("background-color:rgb(100, 100, 100)");
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setStyleSheet("border-radius: 10px;");
    //Set update client dialog fix size
    this->resize(400, 300);
    this->setWindowTitle("Check Update");

    //line edit widget to varsion notify
    QFont titleLabelFont( "Microsoft YaHei", 10, 75);
    titleLabel = new QLabel(this);
    titleLabel->setGeometry(0 , 0, this->width(), 40);
    titleLabel->setFont(titleLabelFont);
//    titleLabel->setText("Current Version V1.0.1");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setScaledContents(true);
    titleLabel->setStyleSheet("background-color:rgb(50, 50, 50);color:rgb(200, 200, 200)");
    //updateWidgets.push_back(titleLabel);
    notUpdateWidgets.push_back(titleLabel);

    //add a Text edit widget for output file that need to update
    //QFont outputEditFont( "Microsoft YaHei", 8, 75);
    outputEdit = new QTextEdit(this);
    //outputEdit->setFont(outputEditFont);
    //outputEdit->setEnabled(false);
    outputEdit->setFocusPolicy(Qt::NoFocus);
    //outputEdit->setWindowFlags(Qt::FramelessWindowHint);
    outputEdit->setText("This is a test");
    //outputEdit->setFrameShape(QFrame::NoFrame);
    outputEdit->setGeometry(20, titleLabel->height() + 40, this->width() - 40,
                            this->height() - titleLabel->height() - 100);
    outputEdit->setStyleSheet("background-color:rgb(100, 100, 100);color:rgb(200, 200, 200)");
    //outputEdit->setStyleSheet("border-radius: 0px;");
    outputEdit->setTextColor(QColor(200, 200, 200, 255));
    updateWidgets.push_back(outputEdit);
    notUpdateWidgets.push_back(outputEdit);

    QFont logTitleLabelFont( "Microsoft YaHei", 8, 75);
    logTitleLabel = new QLabel(this);
    logTitleLabel->setFont(logTitleLabelFont);
    logTitleLabel->setStyleSheet("color:white");
    logTitleLabel->setGeometry(outputEdit->x(), outputEdit->y() - 25, outputEdit->width(), 20);
    logTitleLabel->setScaledContents(true);
    logTitleLabel->setText("Varsion Log : ");
    logTitleLabel->setStyleSheet("color:rgb(200, 200, 200)");
    updateWidgets.push_back(logTitleLabel);
    notUpdateWidgets.push_back(logTitleLabel);

    //splitter for update and cansel button and style
    btnUpdate = new QPushButton(this);
    btnUpdate->setText("UPDATE");
    btnUpdate->setIcon(QIcon("://image/update.png"));
    btnUpdate->setGeometry(20, outputEdit->height() + titleLabel->height() + 50, 70, 30);
    btnUpdate->setStyleSheet("background-color:rgb(50, 50, 50);color:rgb(200, 200, 200)");
    updateWidgets.push_back(btnUpdate);

    //laster varsion info
    newVarsionInfoLabel = new QLabel(this);
    newVarsionInfoLabel->setGeometry(btnUpdate->x() + btnUpdate->width() + 10, btnUpdate->y(),
                                     outputEdit->width() - btnUpdate->width() - 10,
                                     btnUpdate->height());
    newVarsionInfoLabel->setScaledContents(true);
    newVarsionInfoLabel->setWordWrap(true);
    newVarsionInfoLabel->setStyleSheet("color:rgb(200, 200, 200)");
    updateWidgets.push_back(newVarsionInfoLabel);

    btnClose = new QPushButton(this);
    btnClose->setGeometry(this->width() - titleLabel->height(), 0,
                          titleLabel->height(), titleLabel->height());
    btnClose->setStyleSheet("background-color:rgb(150, 150, 150)");
    btnClose->setIcon(QIcon(":/image/close.png"));
    btnClose->setFlat(true);
    connect(btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));
    updateWidgets.push_back(btnClose);
    notUpdateWidgets.push_back(btnClose);

    //update prosess timer
    updateProsessTimer = new QTimer(this);
    connect(updateProsessTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimeOut()));
    updateProsessTimer->stop();

    updateProcessSlider = new QSlider(this);
    updateProcessSlider->setOrientation(Qt::Horizontal);
    updateProcessSlider->setVisible(false);
    updateProcessSlider->setGeometry(btnUpdate->x(), btnUpdate->y() + 10,
                                     outputEdit->width(), btnUpdate->height() / 2);
    updateProcessSlider->setEnabled(false);

    updateTitleLabel = new QLabel(this);
    updateTitleLabel->setFont(titleLabelFont);
    updateTitleLabel->setText("Updating ...");
    updateTitleLabel->setAlignment(Qt::AlignVCenter);
    updateTitleLabel->setGeometry(logTitleLabel->x(), 0, titleLabel->width(), titleLabel->height());
    updateTitleLabel->setStyleSheet("background-color:rgb(50, 50, 50);color:rgb(200, 200, 200)");
    updateTitleLabel->setVisible(false);

    lasterVarsionInfoLabel = new QLabel(this);
//    lasterVarsionInfoLabel->setFont(titleLabelFont);
    lasterVarsionInfoLabel->setStyleSheet("color:rgb(200, 200, 200)");
    lasterVarsionInfoLabel->setText("This varsion already laster varsion, thank you for using");
    lasterVarsionInfoLabel->setVisible(false);
    lasterVarsionInfoLabel->setGeometry(btnUpdate->x(), btnUpdate->y() - 5,
                                        outputEdit->width(), btnUpdate->height());
    notUpdateWidgets.push_back(lasterVarsionInfoLabel);

    updatingLabelGif= new QLabel(this);
    updatingLabelGif->setScaledContents(true);
    updatingLabelGifMovie = new QMovie(":/image/updating.gif");
    updatingLabelGif->setMovie(updatingLabelGifMovie);
    updatingLabelGifMovie->start();
    updatingLabelGif->setVisible(false);

    updatingLabel = new QLabel(this);
    updatingLabel->setStyleSheet("color:rgb(200, 200, 200)");
    updatingLabel->setVisible(false);
    updatingLabel->setAlignment(Qt::AlignCenter);

    connect(btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotUpdateBtnClicked()));
}

void UpdateClientUI::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获得鼠标的初始位置
        mouseStartPoint = event->globalPos();
        //mouseStartPoint = event->pos();
        //获得窗口的初始位置
        windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void UpdateClientUI::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        //获得鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        //QPoint distance = event->pos() - mouseStartPoint;
        //改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}

void UpdateClientUI::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

/*update or not, checked update*/
bool UpdateClientUI::checkUpdate()
{
    //checked update and set isUpdate flag.
    //read varsion file
    QString varsionServerFileName = QString::asprintf("%1/debugVarsion2.0/varsionInfo.txt")
                                                .arg(QCoreApplication::applicationDirPath());
    UpdateClientFileControler varsionServerFileInfo(varsionServerFileName);
    varsionServerInfos = varsionServerFileInfo.readFile();
    varsionServerInfo = varsionServerInfos.at(0);

    //ServerRequest sr;
    //sr.getRequest();

    VarsionInfoControler vInfoControl;
    isUpdate = vInfoControl.compareServerAndClientVarsion(varsionServerInfo);
    if(isUpdate)
    {
        //need to update show update message, wait client clicked update button.
        //updateProcessSlider->setVisible(false);
        //btnUpdate->setVisible(true);
        needToUpdateUI();
    }
    else
    {
        //This is the laster varsion so hide update button and cansel button,
        //and show the laster notify message and ok button.
        notUpdateUI(&vInfoControl);
    }

    titleLabel->setText(QString::asprintf("Current varsion : %1")
                            .arg(vInfoControl.getCurrentVarsion()));
    updateProcessSlider->setVisible(false);
    static int firstStartApp = 1;
    if(firstStartApp == 1 && isUpdate == false)
    {
        firstStartApp = 0;
        return isUpdate;
    }
    firstStartApp = 0;
    this->exec();
    return isUpdate;
}

void UpdateClientUI::needToUpdateUI()
{
    outputEdit->clear();
    for(int i = 0; i < varsionServerInfos.size(); ++i)
    {
        outputEdit->append(varsionServerInfos[i]);
    }
    lasterVarsionInfoLabel->setVisible(false);
    updatingLabelGif->setVisible(false);
    updatingLabel->setVisible(false);

    for(int i = 0; i < updateWidgets.size(); ++i)
    {
        updateWidgets.at(i)->setVisible(true);
    }
    newVarsionInfoLabel->setText(QString::asprintf("Varsion server have new varsion %1, "
                                                   "click update button to update.").arg(varsionServerInfo));
}

void UpdateClientUI::notUpdateUI(VarsionInfoControler *vInfoControl)
{
    outputEdit->clear();
    QStringList currentVarsionInfoList = vInfoControl->getCurrenVarsionInfo();
    for(int i = 0; i < currentVarsionInfoList.size(); ++i)
    {
        outputEdit->append(currentVarsionInfoList[i]);
    }
    btnUpdate->setVisible(false);
    newVarsionInfoLabel->setVisible(false);
    updatingLabelGif->setVisible(false);
    updatingLabel->setVisible(false);
    for(int i = 0; i < notUpdateWidgets.size(); ++i)
    {
        notUpdateWidgets.at(i)->setVisible(true);
    }
}

/*update function*/
void UpdateClientUI::slotUpdateBtnClicked()
{
    updatingUI();
    //update,and start updateProsessTimer
    updateProsessTimer->start(20);
    updating();
}

void UpdateClientUI::updating()
{
    //start update prosess timer at the begining update.
    //this->setWindowTitle("Updating...");

}

void UpdateClientUI::updatingUI()
{
    outputEdit->clear();
    updatingLabelGifMovie->start();
    titleLabel->setText("Updating ...");

    btnUpdate->setVisible(false);
    updateTitleLabel->setVisible(false);
    newVarsionInfoLabel->setVisible(false);
    logTitleLabel->setVisible(false);
    btnClose->setVisible(false);
    outputEdit->setVisible(false);
    updatingLabel->setVisible(true);
    updatingLabelGif->setVisible(true);

    updatingLabelGif->setGeometry((this->width() - updatingLabelGif->width()) / 2, 80,
                                  updatingLabelGif->width(), updatingLabelGif->height());
    updatingLabel->setGeometry(0, 90 + updatingLabelGif->height(),
                               this->width(), 30);

}

void UpdateClientUI::slotUpdateTimeOut()
{
    static int process = 0;
    updatingLabel->setText(QString::asprintf("Updating ...%1\%").arg(process));
    if(process++ == 100)
    {
        //Update finish
        process = 0;
        titleLabel->setText("Finish, please restart!");
        finishUpdate();
    }
}

void UpdateClientUI::finishUpdate()
{
    updateFinishUI();
    QMessageBox::information(this, "Update Finish", "Update finish, please restart for run new varsion", QMessageBox::Ok);
    //set new varsion
    isUpdate = false;
    //updatingLabelGifMovie->stop();
    this->close();
    //sigCloseMainWindow();
}

void UpdateClientUI::updateFinishUI()
{
    updateProsessTimer->stop();
    updatingLabelGifMovie->stop();
    outputEdit->clear();
    updateTitleLabel->setVisible(false);
}

