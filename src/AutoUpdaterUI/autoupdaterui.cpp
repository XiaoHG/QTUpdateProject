﻿#include "autoupdaterui.h"

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
#include <QProcess>
#include <QTextCursor>
#include <QTextBlock>

//m_btnClose->setStyleSheet("QPushButton{background-color:rgba(150, 150, 150, 100%);\
//color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}" // 按键本色
//"QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
//"QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");   // 鼠标按下的色彩

AutoUpdaterUI::AutoUpdaterUI(QWidget *parent)
    :QDialog(parent)
{
    InitUI();
}

AutoUpdaterUI::~AutoUpdaterUI()
{
}

/*UI defined*/
void AutoUpdaterUI::InitUI()
{
    m_updateFiles = new QStringList;
    this->setStyleSheet("background-color:rgb(100, 100, 100)");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(400, 300);
    this->setWindowTitle("Check Update");

    //line edit widget to version notify
    QFont titleLabelFont( "Microsoft YaHei", 11, 75);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setGeometry(0 , 0, this->width(), 40);
    m_titleLabel->setFont(titleLabelFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setScaledContents(true);
    m_titleLabel->setStyleSheet("background-color:rgb(50, 50, 50);"
                                "color:rgb(200, 200, 200)");

    QProgressBar *pbCheckUpdate = new QProgressBar(this);
    pbCheckUpdate->setGeometry(10, 50, 200, 20);
    pbCheckUpdate->setVisible(false);
    m_checkUpdateWidgets.push_back(pbCheckUpdate);

    //add a Text edit widget for output file that need to update
    //QFont outputEditFont( "Microsoft YaHei", 8, 75);
    m_outputVersionInfoEdit = new QTextEdit(this);
    m_outputVersionInfoEdit->setFocusPolicy(Qt::NoFocus);
    m_outputVersionInfoEdit->setWindowFlags(Qt::FramelessWindowHint);
    m_outputVersionInfoEdit->setText("This is a test");
    m_outputVersionInfoEdit->setGeometry(20, m_titleLabel->height() + 40, this->width() - 40,
                            this->height() - m_titleLabel->height() - 100);
    m_outputVersionInfoEdit->setStyleSheet("background-color:rgb(100, 100, 100);"
                                           "color:rgb(200, 200, 200)");
    m_outputVersionInfoEdit->setTextColor(QColor(200, 200, 200, 255));
    m_updateWidgets.push_back(m_outputVersionInfoEdit);
    m_notUpdateWidgets.push_back(m_outputVersionInfoEdit);
    m_updatingWidgets.push_back(m_outputVersionInfoEdit);
    m_finishWidgets.push_back(m_outputVersionInfoEdit);

    QFont logTitleLabelFont( "Microsoft YaHei", 9, 75);
    m_logTitleLabel = new QLabel(this);
    m_logTitleLabel->setFont(logTitleLabelFont);
    m_logTitleLabel->setStyleSheet("color:white");
    m_logTitleLabel->setGeometry(m_outputVersionInfoEdit->x(),
                                 m_outputVersionInfoEdit->y() - 25,
                                 m_outputVersionInfoEdit->width(),
                                 20);
    m_logTitleLabel->setScaledContents(true);
    m_logTitleLabel->setText(QStringLiteral("更新日志 : "));
    m_logTitleLabel->setStyleSheet("color:rgb(200, 200, 200)");
    m_updateWidgets.push_back(m_logTitleLabel);
    m_notUpdateWidgets.push_back(m_logTitleLabel);
    m_updatingWidgets.push_back(m_logTitleLabel);
    m_finishWidgets.push_back(m_logTitleLabel);

    //splitter for update and cansel button and style
    //QFont btnUpdateFont( "Microsoft YaHei", 7, 50);
    m_btnUpdate = new QPushButton(this);
    m_btnUpdate->setText("UPDATE");
    m_btnUpdate->setIcon(QIcon("://image/update.png"));
    m_btnUpdate->setGeometry(20, m_outputVersionInfoEdit->height() + m_titleLabel->height() + 50, 70, 25);
    m_btnUpdate->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%);"
                        "color: white;   border-radius: 5; border-style: outset;}" // 按键本色
                        "QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
                        "QPushButton:pressed{background-color:rgb(85, 170, 255); "
                        "border-style: inset; }");   // 鼠标按下的色彩
    m_updateWidgets.push_back(m_btnUpdate);

    QFont labelLasterVersionFont( "Microsoft YaHei", 10, 75);
    m_labelLasterVersion = new QLabel(this);
    m_labelLasterVersion->setFont(labelLasterVersionFont);
    m_labelLasterVersion->setGeometry(m_btnUpdate->x() + 5,
                                      m_btnUpdate->y() - 10,
                                      m_outputVersionInfoEdit->width(),
                                      30);
    m_labelLasterVersion->setStyleSheet("color:rgb(150, 150, 150)");
    m_notUpdateWidgets.push_back(m_labelLasterVersion);

    //laster version info
    m_newVersionInfoLabel = new QLabel(this);
    m_newVersionInfoLabel->setGeometry(m_btnUpdate->x() + m_btnUpdate->width() + 10,
                                       m_btnUpdate->y(),
                                       m_outputVersionInfoEdit->width() - m_btnUpdate->width() - 10,
                                       m_btnUpdate->height());
    m_newVersionInfoLabel->setScaledContents(true);
    m_newVersionInfoLabel->setWordWrap(true);
    m_newVersionInfoLabel->setStyleSheet("color:rgb(200, 200, 200)");
    m_updateWidgets.push_back(m_newVersionInfoLabel);

    m_btnClose = new QPushButton(this);
    m_btnClose->setIcon(QIcon(":/image/close.png"));
    m_btnClose->setGeometry(this->width() - m_titleLabel->height(), 0,
                          m_titleLabel->height(), m_titleLabel->height());
    m_btnClose->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%);"
                                "color: white; border-style: outset;}" // 按键本色
                                "QPushButton:hover{background-color:rgb(100, 100, 100);}"  // 鼠标停放时的色彩
                                "QPushButton:pressed{background-color:rgb(150, 150, 150); "
                                "border-style: inset; }");   // 鼠标按下的色彩
    m_btnClose->setFlat(true);
    connect(m_btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));
    m_updateWidgets.push_back(m_btnClose);
    m_notUpdateWidgets.push_back(m_btnClose);
    m_finishWidgets.push_back(m_btnClose);

    m_updateProgressBar = new QProgressBar(this);
    m_updateProgressBar->setGeometry(m_outputVersionInfoEdit->x(), this->height() - 50,
                                       m_outputVersionInfoEdit->width(), 20);
    m_updateProgressBar->setStyleSheet("QProgressBar{"
                                       "font:9pt;"
                                       "border-radius:5px;"
                                       "text-align:center;"
                                       "border:1px solid #E8EDF2;"
                                       "background-color: rgb(255, 255, 255);"
                                       "border-color: rgb(180, 180, 180);}"
                                       "QProgressBar:chunk{"
                                       "border-radius:5px;"
                                       "background-color:#1ABC9C;}");
    m_updatingWidgets.push_back(m_updateProgressBar);

    m_btnOk = new QPushButton(this);
    m_btnOk->setText("Reboot");
    m_btnOk->setGeometry(m_btnUpdate->x() + 200, m_btnUpdate->y(),
                         m_btnUpdate->width(), m_btnUpdate->height());
    m_btnOk->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%);"
                        "color: white;   border-radius: 5; border-style: outset;}" // 按键本色
                        "QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
                        "QPushButton:pressed{background-color:rgb(85, 170, 255); "
                        "border-style: inset; }");   // 鼠标按下的色彩
    m_finishWidgets.push_back(m_btnOk);

    m_btnCansel = new QPushButton(this);
    m_btnCansel->setText("Cancel");
    m_btnCansel->setGeometry(m_btnUpdate->x() + m_btnOk->width() + 210, m_btnUpdate->y(),
                             m_btnUpdate->width(), m_btnUpdate->height());
    m_btnCansel->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%);"
                        "color: white; border-radius: 5; border-style: outset;}" // 按键本色
                        "QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
                        "QPushButton:pressed{background-color:rgb(85, 170, 255); "
                        "border-style: inset; }");   // 鼠标按下的色彩
    m_finishWidgets.push_back(m_btnCansel);

    //update prosess timer
    m_updateProsessTimer = new QTimer(this);
    connect(m_updateProsessTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimeOut()));
    m_updateProsessTimer->stop();


    connect(m_btnOk, SIGNAL(clicked(bool)), this, SLOT(slotBtnOkClicked()));
    connect(m_btnCansel, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(m_btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotBtnUpdateClicked()));

    //this->exec();
}

void AutoUpdaterUI::CheckUpdateUI()
{

}

void AutoUpdaterUI::mousePressEvent(QMouseEvent *event)
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

void AutoUpdaterUI::mouseMoveEvent(QMouseEvent *event)
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

void AutoUpdaterUI::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

void AutoUpdaterUI::CheckUpdater(bool isFirst)
{
    m_first = isFirst;

    QDir downloadDir(QApplication::applicationDirPath() + "/download");
    if(!downloadDir.exists())
        downloadDir.mkdir(QApplication::applicationDirPath() + "/download");

    FtpManager *ftp = new FtpManager();
    ftp->setHost("localhost");
    ftp->get("/version/updater.xml", QApplication::applicationDirPath() + "/download/updater.xml");
    connect(ftp, SIGNAL(sigDownloadUpdaterXmlOver()), this, SLOT(slotDownloadUpdaterXmlOver()));
}

void AutoUpdaterUI::UpdateUI()
{
    m_titleLabel->setText(QStringLiteral("检查更新！"));

    //隐藏正在检查更新组件
    ShowCheckUpdateUI(false);

    //隐藏正在更新界面组件
    ShowUpdatingUI(false);

    //隐藏更新完成的部件
    ShowFinishUpdateUI(false);

    //隐藏不需要更新的部件
    ShowNotUpdateUI(false);

    //显示检查更新界面组件
    ShowUpdateUI(true);

    //m_versionServerInfo: 获取到下载的XML的版本，进行显示

    m_versionServerInfo = m_updater.GetVersion(QApplication::applicationDirPath() + "/download/updater.xml");
    m_newVersionInfoLabel->setText(QStringLiteral("检查到新版本 ") + m_versionServerInfo +
                                   QStringLiteral(" 点击更新！"));

    this->exec();
}


void AutoUpdaterUI::UpdatingUI()
{
    m_outputVersionInfoEdit->clear();
    m_titleLabel->setText(QStringLiteral("正在更新 ..."));

    //隐藏检查更新界面组件
    ShowUpdateUI(false);

    //显示正在更新界面组件
    ShowUpdatingUI(true);

}

void AutoUpdaterUI::FinishUpdate()
{
    //UpdateFinishUI();
    m_updateProsessTimer->stop();

    //隐藏正在更新的部件
    ShowUpdatingUI(false);

    //显示更新完成的部件
    ShowFinishUpdateUI(true);

    //this->close();
}

void AutoUpdaterUI::NotUpdateUI()
{

    QString strCurrentVersion = m_updater.GetVersion(QApplication::applicationDirPath() + "/updater.xml");
    m_labelLasterVersion->setText(strCurrentVersion);
    m_titleLabel->setText(QStringLiteral("当前版本是最新版本！"));

    //隐藏检查更新界面组件
    ShowUpdateUI(false);

    //隐藏正在更新界面组件
    ShowUpdatingUI(false);

    //隐藏更新完成的部件
    ShowFinishUpdateUI(false);

    //显示不需要更新的界面组件
    ShowNotUpdateUI(true);

    if(m_first)
    {
        return;
    }
    this->exec();
}

void AutoUpdaterUI::FinishUpdateUI()
{

}

void AutoUpdaterUI::slotDownloadUpdaterXmlOver()
{
    qDebug() << "slotDownloadUpdaterXmlOver";

    FtpManager *ftp = new FtpManager();
    ftp->setHost("localhost");
    ftp->get("/version/versionInfo.txt", QApplication::applicationDirPath() + "/download/versionInfo.txt");
    connect(ftp, SIGNAL(sigDownloadVersionInfoFileOver()), this, SLOT(slotDownloadVersionInfoFileOver()));
}

void AutoUpdaterUI::slotDownloadVersionInfoFileOver()
{
    qDebug() << "slotDownloadVersionInfoFileOver, version infomation download over!";

    m_outputVersionInfoEdit->clear();
    QStringList strListVersionInfo = m_updater.GetVersionInfo();
    if(strListVersionInfo.isEmpty())
        m_outputVersionInfoEdit->append(QStringLiteral("版本信息缺失！"));
    qDebug() << "strListVersionInfo.size = " << strListVersionInfo.size();
    for(int i = 0; i < strListVersionInfo.size(); ++i)
    {
        qDebug() << "version content: " << i << " : " << strListVersionInfo.at(i);
        m_outputVersionInfoEdit->append(strListVersionInfo.at(i));
    }
    m_outputVersionInfoEdit->moveCursor(QTextCursor::Start);

    //更新则isUpdate = true,否则false
    if(m_updater.CheckVersionForUpdate())
    {
        //此时需要更新，弹出对话框让客户端进行选择更新与否
        int n = m_updater.CheckUpdateFiles(QApplication::applicationDirPath() + "/download/updater.xml",
                                   QApplication::applicationDirPath() + "/updater.xml");
        if(n != 1)
        {
            NotUpdateUI();
        }
        else
        {
            UpdateUI();
        }
    }
    else
    {
        //This is the laster version so hide update button and cansel button,
        //and show the laster notify message and ok button.
        NotUpdateUI();
    }
}

/*update function*/
void AutoUpdaterUI::slotBtnUpdateClicked()
{
    UpdatingUI();
    //update,and start updateProsessTimer
    m_updateProsessTimer->start(50);

    //这里执行更新，就是XML对比出来后的所有需更新文件的下载，拷贝。
    m_updater.DownloadUpdateFiles();
}

void AutoUpdaterUI::slotUpdateTimeOut()
{
    QStringList strCurrDownloadFileList = m_updater.GetCurrDownloadFileList();
    qDebug() << "strCurrDownloadFileList.size() = " << strCurrDownloadFileList.size();
    static int i = 0;
    for(; i < strCurrDownloadFileList.size(); ++i)
    {
        m_outputVersionInfoEdit->append(QStringLiteral("正在更新文件..."));
        m_outputVersionInfoEdit->append(strCurrDownloadFileList.at(i));
    }

    QStringList strFinishDownloadFileList = m_updater.GetFinishDownloadFileList();
    qDebug() << "strFinishDownloadFileList.size() = " << strFinishDownloadFileList.size();
    static int j = 0;
    for(; j < strFinishDownloadFileList.size(); ++j)
    {
        m_outputVersionInfoEdit->append(QStringLiteral("完成更新文件..."));
        m_outputVersionInfoEdit->append(strFinishDownloadFileList.at(j));
    }

    m_updateProgressBar->setValue(m_updater.GetUpdateProcess());

    qDebug() << "m_updateProgressBar->value() = " << m_updateProgressBar->value();
    qDebug() << "m_updateProgressBar->maximum() = " << m_updateProgressBar->maximum();
    if(m_updateProgressBar->value() == m_updateProgressBar->maximum())
    {
        m_outputVersionInfoEdit->append(QStringLiteral("注意：所有文件已经更新完成，"
                                          "点击重启客户端会启动最新版本，"
                                          "点击取消保持当前版本运行，下次启动为最新版本！"));
        m_titleLabel->setText(QStringLiteral("更新完成！"));
        FinishUpdate();
    }
}

void AutoUpdaterUI::slotBtnOkClicked()
{

}

void AutoUpdaterUI::ShowUpdateUI(bool b)
{
    for(int i = 0; i < m_updateWidgets.size(); ++i)
    {
        m_updateWidgets.at(i)->setVisible(b);
    }
}

void AutoUpdaterUI::ShowUpdatingUI(bool b)
{
    for(int i = 0; i < m_updatingWidgets.size(); ++i)
    {
        m_updatingWidgets.at(i)->setVisible(b);
    }
}

void AutoUpdaterUI::ShowNotUpdateUI(bool b)
{
    for(int i = 0; i < m_notUpdateWidgets.size(); ++i)
    {
        m_notUpdateWidgets.at(i)->setVisible(b);
    }
}

void AutoUpdaterUI::ShowFinishUpdateUI(bool b)
{
    for(int i = 0; i < m_finishWidgets.size(); ++i)
    {
        m_finishWidgets.at(i)->setVisible(b);
    }
}

void AutoUpdaterUI::ShowCheckUpdateUI(bool b)
{
    for(int i = 0; i < m_checkUpdateWidgets.size(); ++i)
    {
        m_checkUpdateWidgets.at(i)->setVisible(b);
    }
}


