
#include "updateclientui.h"
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

CUpdateClientUI::CUpdateClientUI(QWidget *parent)
    :QDialog(parent)
{
    Init();
}

CUpdateClientUI::~CUpdateClientUI()
{
}

/*init all*/
void CUpdateClientUI::Init()
{
    InitUI();
}

/*UI defined*/
void CUpdateClientUI::InitUI()
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

    //line edit widget to version notify
    QFont titleLabelFont( "Microsoft YaHei", 10, 75);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setGeometry(0 , 0, this->width(), 40);
    m_titleLabel->setFont(titleLabelFont);
//    titleLabel->setText("Current Version V1.0.1");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setScaledContents(true);
    m_titleLabel->setStyleSheet("background-color:rgb(50, 50, 50);color:rgb(200, 200, 200)");

    //add a Text edit widget for output file that need to update
    //QFont outputEditFont( "Microsoft YaHei", 8, 75);
    m_outputVersionInfoEdit = new QTextEdit(this);
    //outputEdit->setFont(outputEditFont);
    //outputEdit->setEnabled(false);
    m_outputVersionInfoEdit->setFocusPolicy(Qt::NoFocus);
    //outputEdit->setWindowFlags(Qt::FramelessWindowHint);
    m_outputVersionInfoEdit->setText("This is a test");
    //outputEdit->setFrameShape(QFrame::NoFrame);
    m_outputVersionInfoEdit->setGeometry(20, m_titleLabel->height() + 40, this->width() - 40,
                            this->height() - m_titleLabel->height() - 100);
    m_outputVersionInfoEdit->setStyleSheet("background-color:rgb(100, 100, 100);color:rgb(200, 200, 200)");
    //outputEdit->setStyleSheet("border-radius: 0px;");
    m_outputVersionInfoEdit->setTextColor(QColor(200, 200, 200, 255));
    m_updateWidgets.push_back(m_outputVersionInfoEdit);
    m_notUpdateWidgets.push_back(m_outputVersionInfoEdit);

    QFont logTitleLabelFont( "Microsoft YaHei", 8, 75);
    m_logTitleLabel = new QLabel(this);
    m_logTitleLabel->setFont(logTitleLabelFont);
    m_logTitleLabel->setStyleSheet("color:white");
    m_logTitleLabel->setGeometry(m_outputVersionInfoEdit->x(), m_outputVersionInfoEdit->y() - 25, m_outputVersionInfoEdit->width(), 20);
    m_logTitleLabel->setScaledContents(true);
    m_logTitleLabel->setText(QString::fromLocal8Bit("更新日志 : "));
    m_logTitleLabel->setStyleSheet("color:rgb(200, 200, 200)");
    m_updateWidgets.push_back(m_logTitleLabel);
    m_notUpdateWidgets.push_back(m_logTitleLabel);

    //splitter for update and cansel button and style
    m_btnUpdate = new QPushButton(this);
    m_btnUpdate->setText("UPDATE");
    m_btnUpdate->setIcon(QIcon("://image/update.png"));
    m_btnUpdate->setGeometry(20, m_outputVersionInfoEdit->height() + m_titleLabel->height() + 50, 70, 30);
    m_btnUpdate->setStyleSheet("background-color:rgb(50, 50, 50);color:rgb(200, 200, 200)");
    m_updateWidgets.push_back(m_btnUpdate);

    //laster version info
    m_newVersionInfoLabel = new QLabel(this);
    m_newVersionInfoLabel->setGeometry(m_btnUpdate->x() + m_btnUpdate->width() + 10, m_btnUpdate->y(),
                                     m_outputVersionInfoEdit->width() - m_btnUpdate->width() - 10,
                                     m_btnUpdate->height());
    m_newVersionInfoLabel->setScaledContents(true);
    m_newVersionInfoLabel->setWordWrap(true);
    m_newVersionInfoLabel->setStyleSheet("color:rgb(200, 200, 200)");
    m_updateWidgets.push_back(m_newVersionInfoLabel);

    btnClose = new QPushButton(this);
    btnClose->setGeometry(this->width() - m_titleLabel->height(), 0,
                          m_titleLabel->height(), m_titleLabel->height());
    btnClose->setStyleSheet("background-color:rgb(150, 150, 150)");
    btnClose->setIcon(QIcon(":/image/close.png"));
    btnClose->setFlat(true);
    connect(btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));
    m_updateWidgets.push_back(btnClose);
    m_notUpdateWidgets.push_back(btnClose);

    //update prosess timer
    m_updateProsessTimer = new QTimer(this);
    connect(m_updateProsessTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimeOut()));
    m_updateProsessTimer->stop();

    m_updatingLabelGif= new QLabel(this);
    m_updatingLabelGif->setScaledContents(true);
    m_updatingLabelGifMovie = new QMovie(":/image/updating.gif");
    m_updatingLabelGif->setMovie(m_updatingLabelGifMovie);
    m_updatingLabelGifMovie->start();
    m_updatingWidgets.push_back(m_updatingLabelGif);

    m_updatingProcessLabel = new QLabel(this);
    m_updatingProcessLabel->setStyleSheet("color:rgb(200, 200, 200)");
    m_updatingProcessLabel->setVisible(false);
    m_updatingProcessLabel->setAlignment(Qt::AlignCenter);
    m_updatingWidgets.push_back(m_updatingProcessLabel);

    connect(m_btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotUpdateBtnClicked()));
}

void CUpdateClientUI::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获得鼠标的初始位置
        m_mouseStartPoint = event->globalPos();
        //mouseStartPoint = event->pos();
        //获得窗口的初始位置
        m_windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void CUpdateClientUI::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        //获得鼠标移动的距离
        QPoint distance = event->globalPos() - m_mouseStartPoint;
        //QPoint distance = event->pos() - mouseStartPoint;
        //改变窗口的位置
        this->move(m_windowTopLeftPoint + distance);
    }
}

void CUpdateClientUI::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

/*update or not, checked update*/
bool CUpdateClientUI::CheckUpdate()
{
//    //checked update and set isUpdate flag.
//    //read version file
//    QString versionServerFileName = QString::asprintf("%1/debugversion2.0/versionInfo.txt")
//                                                .arg(QCoreApplication::applicationDirPath());
//    UpdateClientFileControler versionServerFileInfo(versionServerFileName);
//    versionServerInfos = versionServerFileInfo.readFile();
//    versionServerInfo = versionServerInfos.at(0);

//    //ServerRequest sr;
//    //sr.getRequest();

//    versionInfoControler vInfoControl;
//    isUpdate = vInfoControl.compareServerAndClientversion(versionServerInfo);

    //从版本文件中读取版本号，并进行本地版本和下载XML版本对比，得出是否更新的结论
    //更新则isUpdate = true,否则false
    m_isUpdate = false;

    if(m_isUpdate)
    {
        //need to update show update message, wait client clicked update button.
        //updateProcessSlider->setVisible(false);
        //btnUpdate->setVisible(true);
        //此时需要更新，弹出对话框让客户端进行选择更新与否
        UpdateUI();
    }
    else
    {
        //This is the laster version so hide update button and cansel button,
        //and show the laster notify message and ok button.
        NotUpdateUI();
    }

//    m_titleLabel->setText(QString::asprintf("Current version : %1")
//                            .arg(vInfoControl.getCurrentversion()));

    //标题需要显示当前版本信息，即设置titleLabel

    //判断第一次启动标志位
    static int firstStartApp = 1;
    if(firstStartApp == 1 && m_isUpdate == false)
    {
        firstStartApp = 0;
        return m_isUpdate;
    }
    firstStartApp = 0;
    this->exec();
    return m_isUpdate;
}

void CUpdateClientUI::UpdateUI()
{
    m_outputVersionInfoEdit->clear();

    //m_downloadVersionInfos获取到了最新版本的版本信息，m_outputVersionInfoEdit进行显示
    //目前为设置读取XML，所以此时为空
    for(int i = 0; i < m_downloadVersionInfos.size(); ++i)
    {
        m_outputVersionInfoEdit->append(m_downloadVersionInfos[i]);
    }

    m_titleLabel->setText(QString::fromLocal8Bit("检查更新！"));

    //隐藏正在更新界面组件
    SetVisibleUpdatingUI(false);

    //显示检查更新界面组件
    SetVisibleUpdateUI(true);

    //m_versionServerInfo: 获取到下载的XML的版本，进行显示
    m_newVersionInfoLabel->setText(QString::fromLocal8Bit("有最新版本：") + m_versionServerInfo +
                                   QString::fromLocal8Bit("，点击更新按钮执行更新！"));
}

void CUpdateClientUI::NotUpdateUI()
{
    m_outputVersionInfoEdit->clear();

    //此时无可更新版本，m_outputVersionInfoEdit显示的是当前版本信息，即最新版本信息
    //m_currentVersionInfoList保存的是从当前版本XML读取出的的版本信息
    //此时还没有读取XML信息，所以为空。
    for(int i = 0; i < m_currentVersionInfoList.size(); ++i)
    {
        m_outputVersionInfoEdit->append(m_currentVersionInfoList[i]);
    }

    m_outputVersionInfoEdit->setText(QString::fromLocal8Bit("当前版本是最新版本：V2.1"));
    m_titleLabel->setText(QString::fromLocal8Bit("当前版本是最新版本！"));

//    m_btnUpdate->setVisible(false);
//    m_newVersionInfoLabel->setVisible(false);

    //隐藏检查更新界面组件
    SetVisibleUpdateUI(false);

    //隐藏正在更新界面组件
    SetVisibleUpdatingUI(false);

    //显示不需要更新的界面组件
    SetVisibleNotUpdateUI(true);
}

/*update function*/
void CUpdateClientUI::slotUpdateBtnClicked()
{
    UpdatingUI();
    //update,and start updateProsessTimer
    m_updateProsessTimer->start(20);
    Updating();
}

void CUpdateClientUI::Updating()
{
    //start update prosess timer at the begining update.
    //this->setWindowTitle("Updating...");

    //这里执行更新，就是XML对比出来后的所有需更新文件的下载，拷贝。

}

void CUpdateClientUI::UpdatingUI()
{
    m_outputVersionInfoEdit->clear();
    m_updatingLabelGifMovie->start();
    m_titleLabel->setText(QString::fromLocal8Bit("正在更新 ..."));

    //隐藏检查更新界面组件
    SetVisibleUpdateUI(false);

    //显示正在更新界面组件
    SetVisibleUpdatingUI(true);

    m_updatingLabelGif->setGeometry((this->width() - m_updatingLabelGif->width()) / 2, 80,
                                  m_updatingLabelGif->width(), m_updatingLabelGif->height());
    m_updatingProcessLabel->setGeometry(0, 90 + m_updatingLabelGif->height(),
                               this->width(), 30);

}

void CUpdateClientUI::slotUpdateTimeOut()
{
    static int process = 0;
    QString strTmp = QString::fromLocal8Bit("更新进度 ...");
    strTmp += QString::asprintf("%1\%").arg(process);
    m_updatingProcessLabel->setText(strTmp);
    if(process++ == 100)
    {
        //Update finish
        process = 0;
        m_titleLabel->setText(QString::fromLocal8Bit("更新完成，请重启！"));
        FinishUpdate();
    }
}

void CUpdateClientUI::FinishUpdate()
{
    //UpdateFinishUI();
    m_updateProsessTimer->stop();
    m_updatingLabelGifMovie->stop();
    QMessageBox::information(this, QString::fromLocal8Bit("更新完成"),
                             QString::fromLocal8Bit("更新完成，请重启！"));

    m_isUpdate = false;
    this->close();
}

void CUpdateClientUI::SetVisibleUpdateUI(bool b)
{
    for(int i = 0; i < m_updateWidgets.size(); ++i)
    {
        m_updateWidgets.at(i)->setVisible(b);
    }
}

void CUpdateClientUI::SetVisibleUpdatingUI(bool b)
{
    for(int i = 0; i < m_updatingWidgets.size(); ++i)
    {
        m_updatingWidgets.at(i)->setVisible(b);
    }
}

void CUpdateClientUI::SetVisibleNotUpdateUI(bool b)
{
    for(int i = 0; i < m_notUpdateWidgets.size(); ++i)
    {
        m_notUpdateWidgets.at(i)->setVisible(b);
    }
}

