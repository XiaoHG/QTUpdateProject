#include "autoupdaterui.h"

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
    Init();
    InitUI();
}

AutoUpdaterUI::~AutoUpdaterUI()
{
}

/*init all*/
void AutoUpdaterUI::Init()
{
    FtpManager *ftp = new FtpManager();
    ftp->setHost("localhost");
    connect(ftp, SIGNAL(sigDownloadUpdaterXmlOver()), this, SLOT(slotDownloadUpdaterXmlOver()));
    ftp->get("/mainV1.0/updater.xml", QApplication::applicationDirPath() + "/download/updater.xml");


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
    QFont titleLabelFont( "Microsoft YaHei", 12, 75);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setGeometry(0 , 0, this->width(), 40);
    m_titleLabel->setFont(titleLabelFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setScaledContents(true);
    m_titleLabel->setStyleSheet("background-color:rgb(50, 50, 50);"
                                "color:rgb(200, 200, 200)");

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


    connect(m_btnOk, SIGNAL(clicked(bool)), this, SLOT(slotOkBtnClicked()));
    connect(m_btnCansel, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(m_btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotUpdateBtnClicked()));

    //this->show();
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

void AutoUpdaterUI::testInterface()
{
    m_ftp->get("/mainV1.0/updater.xml", "F:/updater.xml");
    //connect(pReply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
}

void AutoUpdaterUI::CheckUpdater()
{
    Init();
}

/*update or not, checked update*/
bool AutoUpdaterUI::CheckUpdate()
{
    //从版本文件中读取版本号，并进行本地版本和下载XML版本对比，得出是否更新的结论
    m_isUpdate = m_updater.CheckVersionForUpdate();//对比下载下来的XML和本地版本的XML
    //更新则isUpdate = true,否则false
    if(m_isUpdate)
    {
        //此时需要更新，弹出对话框让客户端进行选择更新与否
        int n = m_updater.CheckUpdateFiles(QApplication::applicationDirPath() + "/download/updater.xml",
                                   QApplication::applicationDirPath() + "/updater.xml");
        if(n != 1)
        {
            NotUpdateUI();
            m_isUpdate = false;
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
    return m_isUpdate;
}

void AutoUpdaterUI::UpdateUI()
{
    m_outputVersionInfoEdit->clear();

    m_titleLabel->setText(QStringLiteral("检查更新！"));

    //隐藏正在更新界面组件
    SetVisibleUpdatingUI(false);

    //隐藏更新完成的部件
    SetVisibleFinishUpdateUI(false);

    //隐藏不需要更新的部件
    SetVisibleNotUpdateUI(false);

    //显示检查更新界面组件
    SetVisibleUpdateUI(true);

    //m_versionServerInfo: 获取到下载的XML的版本，进行显示

    m_versionServerInfo = m_updater.GetVersion(QApplication::applicationDirPath() + "/download/updater.xml");
    m_newVersionInfoLabel->setText(QStringLiteral("检查到新版本 ") + m_versionServerInfo +
                                   QStringLiteral(" 点击更新！"));

    this->show();
}


void AutoUpdaterUI::UpdatingUI()
{
    m_outputVersionInfoEdit->clear();
    m_titleLabel->setText(QStringLiteral("正在更新 ..."));

    //隐藏检查更新界面组件
    SetVisibleUpdateUI(false);

    //显示正在更新界面组件
    SetVisibleUpdatingUI(true);

}

void AutoUpdaterUI::Updating()
{
    //start update prosess timer at the begining update.

    //这里执行更新，就是XML对比出来后的所有需更新文件的下载，拷贝。
    m_updater.DownloadUpdateFiles();
}

void AutoUpdaterUI::FinishUpdate()
{
    //UpdateFinishUI();
    m_updateProsessTimer->stop();

    m_isUpdate = false;

    //隐藏正在更新的部件
    SetVisibleUpdatingUI(false);

    //显示更新完成的部件
    SetVisibleFinishUpdateUI(true);

    //this->close();
}

void AutoUpdaterUI::NotUpdateUI()
{
    m_outputVersionInfoEdit->clear();

    QString strCurrentVersion = m_updater.GetVersion(QApplication::applicationDirPath() + "/updater.xml");
    m_labelLasterVersion->setText(strCurrentVersion);
    m_titleLabel->setText(QStringLiteral("当前版本是最新版本！"));

    //隐藏检查更新界面组件
    SetVisibleUpdateUI(false);

    //隐藏正在更新界面组件
    SetVisibleUpdatingUI(false);

    //隐藏更新完成的部件
    SetVisibleFinishUpdateUI(false);

    //显示不需要更新的界面组件
    SetVisibleNotUpdateUI(true);

    //读取和主程序交互的配置文件，如果flag为false则无弹窗并且直接退出更新程序；
    //需要处理更新程序的自结束，现在时卡死状态
    if(GetConfigFlag())
    {
        this->show();
    }
    else{
        qDebug() << "this->close()";
        this->close();
    }
}

void AutoUpdaterUI::slotDownloadUpdaterXmlOver()
{
    qDebug() << "slotDownloadUpdaterXmlOver";

    FtpManager *ftp = new FtpManager();
    ftp->setHost("localhost");
    connect(ftp, SIGNAL(sigDownloadVersionInfoFileOver()), this, SLOT(slotDownloadVersionInfoFileOver()));
    ftp->get("/mainV1.0/versionInfo.txt", QApplication::applicationDirPath() + "/download/versionInfo.txt");

}

void AutoUpdaterUI::slotDownloadVersionInfoFileOver()
{
    qDebug() << "slotDownloadVersionInfoFileOver";
    //m_downloadVersionInfos获取到了最新版本的版本信息，m_outputVersionInfoEdit进行显示
    //目前为设置读取XML，所以此时为空
    CheckUpdate();
    QStringList strListVersionInfo = m_updater.GetVersionInfo();
    if(strListVersionInfo.isEmpty())
        m_outputVersionInfoEdit->append(QStringLiteral("版本信息缺失！"));
    for(int i = 0; i < strListVersionInfo.size(); ++i)
    {
        m_outputVersionInfoEdit->append(strListVersionInfo.at(i));
    }
    m_outputVersionInfoEdit->moveCursor(QTextCursor::Start);
}

/*update function*/
void AutoUpdaterUI::slotUpdateBtnClicked()
{
    UpdatingUI();
    //update,and start updateProsessTimer
    m_updateProsessTimer->start(50);
    Updating();
}

void AutoUpdaterUI::slotUpdateTimeOut()
{
    static int process = 0;
    QString strCurrentDir = QDir::currentPath();
    QStringList strListDownloadFileDir = m_updater.GetUpdateFileDir();
    QStringList strListDownloadFileName = m_updater.GetUpdateFileName();
    QString strTmpDir;
    static int i = 0;
    qDebug() << "Time out strListDownloadFileName size = " << strListDownloadFileName.size();
    qDebug() << "Time out strListDownloadFileDir size = " << strListDownloadFileDir.size();
    if(process % (100 / strListDownloadFileDir.size()) == 0 && i < strListDownloadFileDir.size())
    {
        m_outputVersionInfoEdit->append(QStringLiteral("正在更新文件") +
                                        strListDownloadFileName.at(i) + " ...");
        if(strListDownloadFileDir.at(i) == "")
        {
            strTmpDir = strCurrentDir + "/" + strListDownloadFileName.at(i);
        }
        else
        {
            strTmpDir = strCurrentDir + "/" + strListDownloadFileDir.at(i) + "/" + strListDownloadFileName.at(i);
        }
        m_outputVersionInfoEdit->append(strTmpDir);
        i++;
    }

    if((process+1) % (100 / strListDownloadFileDir.size()) == 0 && i <= strListDownloadFileDir.size())
        m_outputVersionInfoEdit->append(QStringLiteral("文件") +
                                    strListDownloadFileName.at(i - 1) +
                                    QStringLiteral("更新完成"));

    m_updateProgressBar->setValue(m_updater.GetUpdateProcess());
    qDebug() << "m_updater.GetDownProcess() = " << m_updater.GetDownProcess();
    qDebug() << "m_updater.GetUpdateProcess() = " << m_updater.GetUpdateProcess();
    if(m_updater.GetUpdateProcess() == 100)
    {
        m_outputVersionInfoEdit->append(QStringLiteral("注意：所有文件已经更新完成，"
                                          "点击重启客户端会启动最新版本，"
                                          "点击取消保持当前版本运行，下次启动为最新版本！"));
        //Update finish
        process = 0;
        m_titleLabel->setText(QStringLiteral("更新完成！"));
        FinishUpdate();
    }
}

void AutoUpdaterUI::slotOkBtnClicked()
{
    QString name = QDir::currentPath() + "/debug/AutoUpdateTest.exe"; //主程序名
    ExitApp(name);
}

void AutoUpdaterUI::slotUpdateProgess(int value)
{
    m_updateProgressBar->setValue(value);
}

void AutoUpdaterUI::ExitApp(QString name)
{
    /**运行主程序，并且退出当前更新程序(说明：主程序在上上一级目录中)**/
    if(!QProcess::startDetached(name))//启动主程序，主程序在其上一级目录
    {
        QMessageBox::warning(this, "warning", QStringLiteral("主程序打开错误！"),
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
    this->close();
}

void AutoUpdaterUI::SetVisibleUpdateUI(bool b)
{
    for(int i = 0; i < m_updateWidgets.size(); ++i)
    {
        m_updateWidgets.at(i)->setVisible(b);
    }
}

void AutoUpdaterUI::SetVisibleUpdatingUI(bool b)
{
    for(int i = 0; i < m_updatingWidgets.size(); ++i)
    {
        m_updatingWidgets.at(i)->setVisible(b);
    }
}

void AutoUpdaterUI::SetVisibleNotUpdateUI(bool b)
{
    for(int i = 0; i < m_notUpdateWidgets.size(); ++i)
    {
        m_notUpdateWidgets.at(i)->setVisible(b);
    }
}

void AutoUpdaterUI::SetVisibleFinishUpdateUI(bool b)
{
    for(int i = 0; i < m_finishWidgets.size(); ++i)
    {
        m_finishWidgets.at(i)->setVisible(b);
    }
}

/**
 * @brief AutoUpdaterUI::GetConfigFlag
 * @return
 * 读取主程序和更新程序公用的配置文件，控制无更新时窗口的弹出
 */
bool AutoUpdaterUI::GetConfigFlag()
{
    QString strConfigName = QDir::currentPath() + "/updateConfig.txt";
    QFile file(strConfigName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "GetConfigFlag false, cant't open file " << strConfigName;
        return false;
    }
    return file.readAll().toInt();
}

