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
#include <QBitmap>
#include <QPainter>
#include <QScrollBar>
#include <QStandardPaths>
#include <QFile>

#define CHECKUPDATE_TIMEOUT 15

AutoUpdaterUI::AutoUpdaterUI(bool bCh, QWidget *parent)
    :QMainWindow(parent),
      m_bTranslator(bCh)
{
    Language(bCh);

    InitUI();

    m_updater = new AutoUpdater();

    //It is download files that updater.xml and versionInfo.txt,
    //and is emit the signal of init files download over,
    //and next step to check betweed download updater.xml and local xml file
    //get whether update or not result.
    connect(m_updater, SIGNAL(sigDownloadInitFileOver()),
            this, SLOT(slotDownloadInitFileOver()));

    //30s time out, exit the application.
    connect(m_updater, SIGNAL(sigDownloadTimeout()),
            this, SLOT(slotDownloadTimeout()));

    //It is emited that each file download at start.
    connect(m_updater, SIGNAL(sigDownloadStartPerFile(QString)),
            this, SLOT(slotDownloadStartPerFile(QString)));

    //It is emited that each file download finish.
    connect(m_updater, SIGNAL(sigDownloadFinishPerFile(QString)),
            this, SLOT(slotDownloadFinishPerFile(QString)));

    m_updatingTimer = new QTimer(this);
    connect(m_updatingTimer, SIGNAL(timeout()), this, SLOT(slotCheckUpdateTimeOut()));
    m_updatingTimer->start(1000);

    m_bTranslator = false;
}

AutoUpdaterUI::~AutoUpdaterUI()
{
    if(m_updater)
    {
        m_updater->deleteLater();
    }
}

/*UI defined*/
void AutoUpdaterUI::InitUI()
{
    this->resize(400, 400);
    this->setStyleSheet("background-color:rgb(100, 100, 100);");

    //line edit widget to version notify
    QFont titleLabelFont( "Microsoft YaHei", 11, 75);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setGeometry(0 , 0, this->width(), 40);
    m_titleLabel->setFont(titleLabelFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setScaledContents(true);
    m_titleLabel->setText(QObject::tr("Checking for update"));
    m_titleLabel->setStyleSheet("background-color:rgb(50, 50, 50);"
                                "color:rgb(200, 200, 200)");

    //add a Text edit widget for output file that need to update
    //QFont outputEditFont( "Microsoft YaHei", 8, 75);
    m_outputVersionInfoEdit = new QTextEdit(this);
    m_outputVersionInfoEdit->setFocusPolicy(Qt::NoFocus);
    m_outputVersionInfoEdit->setWindowFlags(Qt::FramelessWindowHint);
    m_outputVersionInfoEdit->setGeometry(20, m_titleLabel->height() + 40, this->width() - 40,
                            this->height() - m_titleLabel->height() - 100);
    m_outputVersionInfoEdit->setStyleSheet("background-color:rgb(100, 100, 100);"
                                           "color:rgb(200, 200, 200)");
    m_outputVersionInfoEdit->setTextColor(QColor(200, 200, 200, 255));
    //m_outputVersionInfoEdit->setFrameShape(QTextEdit::NoFrame);
    m_outputVersionInfoEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                                "{"
                                                                "width:8px;"
                                                                "background:rgba(0,0,0,0%);"
                                                                "margin:0px,0px,0px,0px;"
                                                                "padding-top:9px;"
                                                                "padding-bottom:9px;"
                                                                "}"
                                                                "QScrollBar::handle:vertical"
                                                                "{"
                                                                "width:8px;"
                                                                "background:rgba(18, 237, 237,50%);" //normol
                                                                " border-radius:4px;"
                                                                "min-height:20;"
                                                                "}"
                                                                "QScrollBar::handle:vertical:hover"
                                                                "{"
                                                                "width:8px;"
                                                                "background:rgba(18, 237, 237, 100%);" //hover
                                                                " border-radius:4px;"
                                                                "min-height:20;"
                                                                "}"
                                                                "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                                "{"
                                                                "background:rgba(0, 0, 0, 10%);" //bottom
                                                                "border-radius:4px;"
                                                                "}"
                                                                "QScrollBar::add-line:vertical"
                                                                "{"
                                                                "height:9px;width:8px;"
                                                                "border-image:url(:/image/down.png);"
                                                                "subcontrol-position:bottom;"
                                                                "}"
                                                                "QScrollBar::sub-line:vertical"
                                                                "{"
                                                                "height:9px;width:8px;"
                                                                "border-image:url(:/image/up.png);"
                                                                "subcontrol-position:top;"
                                                                "}"
                                                                "QScrollBar::add-line:vertical:hover"
                                                                "{"
                                                                "height:10px;width:10px;"
                                                                "border-image:url(:/image/press_down.png);"
                                                                "subcontrol-position:bottom;"
                                                                "}"
                                                                "QScrollBar::sub-line:vertical:hover"
                                                                "{"
                                                                "height:10px;width:10px;"
                                                                "border-image:url(:/image/press_up.png);"
                                                                "subcontrol-position:top;"
                                                                "}");

    QFont logTitleLabelFont( "Microsoft YaHei", 9, 75);
    m_logTitleLabel = new QLabel(this);
    m_logTitleLabel->setFont(logTitleLabelFont);
    m_logTitleLabel->setStyleSheet("color:white");
    m_logTitleLabel->setGeometry(m_outputVersionInfoEdit->x(),
                                 m_outputVersionInfoEdit->y() - 25,
                                 m_outputVersionInfoEdit->width() - 100,
                                 20);
    m_logTitleLabel->setScaledContents(true);
    m_logTitleLabel->setText(QObject::tr("log :"));
    m_logTitleLabel->setStyleSheet("color:rgb(200, 200, 200)");

    m_btnClose = new QPushButton(this);
    m_btnClose->setIcon(QIcon(":/image/close.png"));
    m_btnClose->setGeometry(this->width() - m_titleLabel->height() - 10, 5,
                          m_titleLabel->height() + 5, m_titleLabel->height() - 10);
    m_btnClose->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                              "QPushButton:hover{background-color:rgb(18, 237, 237);}"
                              "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");
    m_btnClose->setFlat(true);
    connect(m_btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));

    UpdateUI();
    UpdatingUI();
    FinishUpdateUI();
    NotUpdateUI();
    DownloadTimeoutUI();

    m_logTitleLabel->setVisible(true);
    m_outputVersionInfoEdit->setVisible(true);
    m_btnClose->setVisible(true);
}

void AutoUpdaterUI::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(), 10, 10);
    setMask(bmp);
}

void AutoUpdaterUI::UpdateUI()
{
    QFont btnUpdateFont( "Microsoft YaHei", 9, 75);
    m_btnUpdate = new QPushButton(this);
    m_btnUpdate->setFont(btnUpdateFont);
    m_btnUpdate->setText(QObject::tr("Update"));
    m_btnUpdate->setIcon(QIcon("://image/update2.png"));
    m_btnUpdate->setGeometry(20, m_outputVersionInfoEdit->height() + m_titleLabel->height() + 50, 70, 25);
    m_btnUpdate->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                        "QPushButton:hover{background-color:rgb(18, 237, 237); color: black;}"
                        "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");

    //new version info
    m_newHaveVersionLabel = new QLabel(this);
    m_newHaveVersionLabel->setGeometry(m_btnUpdate->x() + m_btnUpdate->width() + 10,
                                       m_btnUpdate->y(),
                                       m_outputVersionInfoEdit->width() - m_btnUpdate->width() - 10,
                                       m_btnUpdate->height());
    m_newHaveVersionLabel->setScaledContents(true);
    m_newHaveVersionLabel->setWordWrap(true);
    m_newHaveVersionLabel->setStyleSheet("color:rgb(200, 200, 200)");

    connect(m_btnUpdate, SIGNAL(clicked(bool)), this, SLOT(slotBtnUpdateClicked()));

    m_updateWidgets.push_back(m_btnUpdate);
    m_updateWidgets.push_back(m_newHaveVersionLabel);

    //init false
    ShowUpdateUI(false);
}

void AutoUpdaterUI::UpdatingUI()
{
    m_updateProgressBar = new QProgressBar(this);
    m_updateProgressBar->setGeometry(m_outputVersionInfoEdit->x(), this->height() - 50,
                                       m_outputVersionInfoEdit->width(), 20);
    m_updateProgressBar->setStyleSheet("QProgressBar{"
                                           "font:9pt;"
                                           "border-radius:5px;"
                                           "text-align:center;"
                                           "border:1px solid #E8EDF2;"
                                           "background-color: rgb(255, 255, 255);"
                                           "border-color: rgb(180, 180, 180);"
                                       "}"
                                       "QProgressBar:chunk{"
                                           "border-radius:5px;"
                                           "background-color:#1ABC9C;"
                                       "}");

    //update prosess timer
    m_updateProsessTimer = new QTimer(this);
    connect(m_updateProsessTimer, SIGNAL(timeout()), this, SLOT(slotUpdateProcess()));
    m_updateProsessTimer->stop();

    m_updatingWidgets.push_back(m_updateProgressBar);

    //init false
    ShowUpdatingUI(false);
}

void AutoUpdaterUI::FinishUpdateUI()
{
    m_btnRestart = new QPushButton(this);
    m_btnRestart->setText(QObject::tr("Restart"));
    m_btnRestart->setIcon(QIcon(":/image/restart.png"));
    m_btnRestart->setGeometry(m_btnUpdate->x(), m_btnUpdate->y(),
                         m_btnUpdate->width(), m_btnUpdate->height());
    m_btnRestart->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                        "QPushButton:hover{background-color:rgb(18, 237, 237); color: black;}"
                        "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");

    connect(m_btnRestart, SIGNAL(clicked(bool)), this, SLOT(slotBtnRestartClicked()));

    m_finishWidgets.push_back(m_btnRestart);

    //init false
    ShowFinishUpdateUI(false);
}

void AutoUpdaterUI::NotUpdateUI()
{
    QFont labelLasterVersionFont( "Microsoft YaHei", 10, 75);
    m_curVersionLabel = new QLabel(this);
    m_curVersionLabel->setFont(labelLasterVersionFont);
    m_curVersionLabel->setGeometry(m_btnUpdate->x() + 5,
                                      m_btnUpdate->y() - 10,
                                      m_outputVersionInfoEdit->width(),
                                      30);
    m_curVersionLabel->setStyleSheet("color:rgb(150, 150, 150)");

    m_notUpdateWidgets.push_back(m_curVersionLabel);

    //init false
    ShowNotUpdateUI(false);
}

void AutoUpdaterUI::DownloadTimeoutUI()
{
    m_btnDownloadTimeoutOK = new QPushButton(this);
    m_btnDownloadTimeoutOK->setText(QObject::tr("ok"));
    m_btnDownloadTimeoutOK->setGeometry(m_btnUpdate->x(), m_btnUpdate->y(),
                                     m_btnUpdate->width(), m_btnUpdate->height());
    m_btnDownloadTimeoutOK->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                                        "QPushButton:hover{background-color:rgb(18, 237, 237); color: black;}"
                                        "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");
    connect(m_btnDownloadTimeoutOK, SIGNAL(clicked(bool)), this, SLOT(slotClickTimeoutOk()));

    m_downloadTimeoutWidgets.push_back(m_btnDownloadTimeoutOK);

    ShowDownloadTimeoutUI(false);
}

void AutoUpdaterUI::Update()
{
    m_titleLabel->setText(tr("Update dialog"));
    m_btnClose->setVisible(true);

    m_versionServerInfo = m_updater->GetNewVersion();
    m_newHaveVersionLabel->setText(QObject::tr("Find ") + m_versionServerInfo +
                                   QObject::tr(" version, click update button to update!"));
}

void AutoUpdaterUI::Updating()
{
    m_outputVersionInfoEdit->clear();
    m_titleLabel->setText(QObject::tr("Updating"));
}

void AutoUpdaterUI::FinishUpdate()
{
    //UpdateFinishUI();
    m_updateProsessTimer->stop();
    m_outputVersionInfoEdit->append(QObject::tr("The update is complete, please restart!"));
    m_titleLabel->setText(QObject::tr("Finish"));
    m_btnClose->setVisible(false);
}

void AutoUpdaterUI::NotUpdate()
{
    QString strCurrentVersion = m_updater->GetOldVersion();
    m_curVersionLabel->setText(strCurrentVersion);
    m_titleLabel->setText(QObject::tr("Laster"));
}

void AutoUpdaterUI::DownloadTimeout()
{
    m_titleLabel->setText(QObject::tr("Timedout"));
    QStringList timeoutFileList = m_updater->GetDownloadTimeoutList();
    QString timeoutMsg;
    for(int i = 0; i < timeoutFileList.size(); i++)
    {
        timeoutMsg.append(QObject::tr("File: "));
        timeoutMsg.append(timeoutFileList.at(i));
        timeoutMsg.append(QObject::tr(" download timed out!"));
        m_outputVersionInfoEdit->append(timeoutMsg);
    }

    m_outputVersionInfoEdit->append(QObject::tr("The download failed, please check the network connection status!"));
}

void AutoUpdaterUI::ShowUpdateUI(bool visible)
{
    for(int i = 0; i < m_updateWidgets.size(); ++i)
    {
        m_updateWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::ShowUpdatingUI(bool visible)
{
    for(int i = 0; i < m_updatingWidgets.size(); ++i)
    {
        m_updatingWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::ShowFinishUpdateUI(bool visible)
{
    for(int i = 0; i < m_finishWidgets.size(); ++i)
    {
        m_finishWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::ShowNotUpdateUI(bool visible)
{
    for(int i = 0; i < m_notUpdateWidgets.size(); ++i)
    {
        m_notUpdateWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::ShowDownloadTimeoutUI(bool visible)
{
    for(int i = 0; i < m_downloadTimeoutWidgets.size(); ++i)
    {
        m_downloadTimeoutWidgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::CheckUpdater(bool isFirst)
{
    m_first = isFirst;
    if(!m_first)
        this->show();
    qDebug() << "start time out";
    m_updater->DownloadXMLFile();
}

void AutoUpdaterUI::CheckUpdate()
{
    m_outputVersionInfoEdit->append(QObject::tr("Checking for update "));
}

void AutoUpdaterUI::slotDownloadInitFileOver()
{
    m_updatingTimer->stop();
    m_outputVersionInfoEdit->clear();
    QString strVersionInfo = m_updater->GetVersionInfo();
    if(strVersionInfo.isEmpty())
        m_outputVersionInfoEdit->setText(QObject::tr("Version information is missing"));
    else
        m_outputVersionInfoEdit->setText(strVersionInfo.toLocal8Bit());
    m_outputVersionInfoEdit->moveCursor(QTextCursor::Start);

    //isUpdate is true to update, or not.
    if(m_updater->IsUpdate())
    {
        //Load download files path from ftp server.
        m_updater->LoadUpdateFiles();
        Update();
        ShowUpdateUI(true);
        this->show();
    }
    else
    {
        //This is the laster version so hide update button and cansel button,
        //and show the laster notify message and ok button.
        NotUpdate();
        ShowNotUpdateUI(true);
        if(m_first)
        {
            //This is the main application call updater application from main function
            //and this is not update version at that time, exit update process.
            exit(0);
        }
        this->show();
    }
}

void AutoUpdaterUI::slotDownloadStartPerFile(QString fileName)
{
    QString startDownload;
    startDownload.append(QObject::tr("Updating "));
    startDownload.append(fileName);
    m_outputVersionInfoEdit->append(startDownload);
}

void AutoUpdaterUI::slotDownloadFinishPerFile(QString fileName)
{
    QString startDownload;
    startDownload.append(fileName);
    startDownload.append(QObject::tr(" update is complete!"));
    m_outputVersionInfoEdit->append(startDownload);
}

void AutoUpdaterUI::slotCheckUpdateTimeOut()
{
    static int timeOut = 0;
    timeOut++;
    if(timeOut == CHECKUPDATE_TIMEOUT)
    {
        qDebug() << "time out = " << timeOut;
        m_outputVersionInfoEdit->clear();
        m_outputVersionInfoEdit->append(QObject::tr("Check for update failed!"));
        m_outputVersionInfoEdit->append(QObject::tr("Please check the network link status!"));
        QStringList errorStack = m_updater->GetFtpErrorStack();
        for(int i = 0 ; i < errorStack.size(); i++)
        {
            m_outputVersionInfoEdit->append(errorStack.at(i));
        }
        if(m_first)
        {
            exit(0);
        }
        this->show();
        m_updatingTimer->stop();
    }
    static QString tmpStr[3] = {".", "..", "..."};
    static int i = 0;
    if(i == 3)
        i = 0;
    m_outputVersionInfoEdit->setText(QObject::tr("Checking for update ") + QString::asprintf("%1").arg(tmpStr[i++]));
    qDebug() << "time : " << timeOut;
}

void AutoUpdaterUI::slotDownloadTimeout()
{
    m_updateProsessTimer->stop();
    m_btnClose->setVisible(false);
    ShowUpdatingUI(false);
    DownloadTimeout();
    ShowDownloadTimeoutUI(true);
}

void AutoUpdaterUI::slotClickTimeoutOk()
{
    qDebug() << "close()";
    exit(0);
}

/*update function*/
void AutoUpdaterUI::slotBtnUpdateClicked()
{
    Updating();
    ShowUpdateUI(false);
    ShowUpdatingUI(true);
    //update,and start updateProsessTimer
    m_updateProsessTimer->start(50);

    //Download all files which check out update files.
    m_updater->DownloadUpdateFiles();
}

void AutoUpdaterUI::slotUpdateProcess()
{

    m_updateProgressBar->setValue(m_updater->GetUpdateProcess());

    qDebug() << "m_updateProgressBar->value() = " << m_updateProgressBar->value();
    qDebug() << "m_updateProgressBar->maximum() = " << m_updateProgressBar->maximum();
    if(m_updateProgressBar->value() == m_updateProgressBar->maximum())
    {
        ShowUpdatingUI(false);
        FinishUpdate();
        ShowFinishUpdateUI(true);
    }
}

void AutoUpdaterUI::slotBtnRestartClicked()
{
    m_updater->RestartApp();
    qDebug() << "ok";
}

void AutoUpdaterUI::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        m_mouseStartPoint = event->globalPos();
        m_windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void AutoUpdaterUI::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        QPoint distance = event->globalPos() - m_mouseStartPoint;
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

void AutoUpdaterUI::Language(bool ch)
{
    qDebug() << "onTranslation1";
    if(ch)
    {
        //chinese
        m_qtTranslator.load(":/zh_en.qm");
        qApp->installTranslator(&m_qtTranslator);
    }
}


