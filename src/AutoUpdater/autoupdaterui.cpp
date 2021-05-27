﻿#include "autoupdaterui.h"
#include "updatelog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QFileDialog>
#include <QApplication>
#include <QMouseEvent>

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

extern UpdateLog g_log;

#define CHECKUPDATE_TIMEOUT 15

AutoUpdaterUI::AutoUpdaterUI(bool bCh, QWidget *parent)
    :QMainWindow(parent),
      m_bCh(bCh),
      m_isUpdatingError(false)
{
    //Language set from parent.
    Language(bCh);

    //Init UI
    InitUI();

    //Init update manager
    m_updater = new AutoUpdater(bCh);

    //It is download files that updater.xml and versionInfoCh.txt,
    //and is emit the signal of init files download over,
    //and next step to check betweed download updater.xml and local xml file
    //get whether update or not result.
    connect(m_updater, SIGNAL(sigDownloadInitFileOver()),
            this, SLOT(slotDownloadInitFileOver()));

    //It is emited that each file download at start.
    connect(m_updater, SIGNAL(sigDownloadStartPerFile(QString)),
            this, SLOT(slotDownloadStartPerFile(QString)));

    //It is emited that each file download finish.
    connect(m_updater, SIGNAL(sigDownloadFinishPerFile(QString)),
            this, SLOT(slotDownloadFinishPerFile(QString)));

    //This timer monitor the init file download error if.
    //check per 100ms.
    m_checkForUpdateTimer = new QTimer(this);
    connect(m_checkForUpdateTimer, SIGNAL(timeout()), this, SLOT(slotCheckForUpdateTimeout()));
    m_checkForUpdateTimer->start(100);

    //update prosess timer
    m_updatingTimer = new QTimer(this);
    connect(m_updatingTimer, &QTimer::timeout, this, &AutoUpdaterUI::slotUpdatingTimerout);
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
    this->resize(450, 450);
    this->setStyleSheet("background-color:rgb(100, 100, 100);");

    //line edit widget to version notify
    QFont titleLabelFont( "Microsoft YaHei", 11, 75);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setGeometry(0 , 0, this->width(), this->height() / 10);
    m_titleLabel->setFont(titleLabelFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setScaledContents(true);
    m_titleLabel->setText(QObject::tr("Checking for update"));
    m_titleLabel->setStyleSheet("background-color:rgb(50, 50, 50);"
                                "color:rgb(200, 200, 200)");

    //add a Text edit widget for output file that need to update
    m_outputVersionInfoEdit = new QTextEdit(this);
    m_outputVersionInfoEdit->setFocusPolicy(Qt::NoFocus);
    m_outputVersionInfoEdit->setWindowFlags(Qt::FramelessWindowHint);
    m_outputVersionInfoEdit->setGeometry(20, m_titleLabel->height() + 40, this->width() - 40,
                            this->height() - m_titleLabel->height() - 100);
    m_outputVersionInfoEdit->setStyleSheet("background-color:rgb(100, 100, 100);"
                                           "color:rgb(200, 200, 200)");
    m_outputVersionInfoEdit->setTextColor(QColor(200, 200, 200, 255));
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
                                                                "border-image:url(:/icon/down.png);"
                                                                "subcontrol-position:bottom;"
                                                                "}"
                                                                "QScrollBar::sub-line:vertical"
                                                                "{"
                                                                "height:9px;width:8px;"
                                                                "border-image:url(:/icon/up.png);"
                                                                "subcontrol-position:top;"
                                                                "}"
                                                                "QScrollBar::add-line:vertical:hover"
                                                                "{"
                                                                "height:10px;width:10px;"
                                                                "border-image:url(:/icon/press_down.png);"
                                                                "subcontrol-position:bottom;"
                                                                "}"
                                                                "QScrollBar::sub-line:vertical:hover"
                                                                "{"
                                                                "height:10px;width:10px;"
                                                                "border-image:url(:/icon/press_up.png);"
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
    m_btnClose->setIcon(QIcon(":/icon/close.png"));
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

void AutoUpdaterUI::UpdateUI()
{
    QFont btnUpdateFont( "Microsoft YaHei", 9, 75);
    m_btnUpdate = new QPushButton(this);
    m_btnUpdate->setFont(btnUpdateFont);
    m_btnUpdate->setText(QObject::tr("Update"));
    m_btnUpdate->setIcon(QIcon(":/icon/update2.png"));
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
    //ShowUpdateUI(false);
    ShowWhichUI(m_updateWidgets, false);
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

    m_updatingWidgets.push_back(m_updateProgressBar);

    //init false
    ShowWhichUI(m_updatingWidgets, false);
}

void AutoUpdaterUI::FinishUpdateUI()
{
    m_btnRestart = new QPushButton(this);
    m_btnRestart->setText(QObject::tr("Restart"));
    m_btnRestart->setIcon(QIcon(":/icon/restart.png"));
    m_btnRestart->setGeometry(m_btnUpdate->x(), m_btnUpdate->y(),
                         m_btnUpdate->width(), m_btnUpdate->height());
    m_btnRestart->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                        "QPushButton:hover{background-color:rgb(18, 237, 237); color: black;}"
                        "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");

    connect(m_btnRestart, SIGNAL(clicked(bool)), this, SLOT(slotBtnRestartClicked()));

    m_finishWidgets.push_back(m_btnRestart);

    //init false
    //ShowFinishUpdateUI(false);
    ShowWhichUI(m_finishWidgets, false);
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
    //ShowNotUpdateUI(false);
    ShowWhichUI(m_notUpdateWidgets, false);
}

void AutoUpdaterUI::DownloadTimeoutUI()
{
    m_btnOKExit = new QPushButton(this);
    m_btnOKExit->setText(QObject::tr("ok"));
    m_btnOKExit->setGeometry(m_btnUpdate->x(), m_btnUpdate->y(),
                                     m_btnUpdate->width(), m_btnUpdate->height());
    m_btnOKExit->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                                        "QPushButton:hover{background-color:rgb(18, 237, 237); color: black;}"
                                        "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");
    connect(m_btnOKExit, SIGNAL(clicked(bool)), this, SLOT(slotClickOkExit()));

    m_downloadTimeoutWidgets.push_back(m_btnOKExit);

    ShowWhichUI(m_downloadTimeoutWidgets, false);
}

void AutoUpdaterUI::UpdateFailureUI()
{
    ShowWhichUI(m_updateFailureWidgets, false);
}

void AutoUpdaterUI::Updater(bool isFirst)
{
    m_first = isFirst;

    //If enter is from parent main function, and show the checking UI.
    if(!m_first)
        this->show();
    g_log.log(UpdateLog::DEBUG, "Beging checking for update timer, It is time out at 30 second!", __FILE__, __LINE__);

    //start to check for update.
    m_updater->DownloadXMLFile();
}

void AutoUpdaterUI::CheckForUpdate()
{
    m_outputVersionInfoEdit->append(QObject::tr("Checking for update "));
}

void AutoUpdaterUI::Update()
{
    g_log.log(UpdateLog::INFO, "Update UI", __FILE__, __LINE__);
    m_titleLabel->setText(tr("Update dialog"));
    m_btnClose->setVisible(true);

    m_versionServerInfo = m_updater->GetNewVersion();
    m_newHaveVersionLabel->setText(QObject::tr("Find ") + m_versionServerInfo +
                                   QObject::tr(" version, click update button to update!"));

    //Load download files path from ftp server.
    m_updater->LoadUpdateFiles();
}

void AutoUpdaterUI::Updating()
{
    g_log.log(UpdateLog::INFO, "Updating UI", __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, "Update process timer start at 50ms", __FILE__, __LINE__);
    //update,and start updateProsessTimer
    m_updatingTimer->start(50);
    m_outputVersionInfoEdit->clear();
    m_titleLabel->setText(QObject::tr("Updating"));

    //Download all files which check out update files.
    m_updater->DownloadUpdateFiles();
}

void AutoUpdaterUI::FinishUpdate()
{
    g_log.log(UpdateLog::INFO, "FinishUpdate UI", __FILE__, __LINE__);
    m_updatingTimer->stop();
    m_outputVersionInfoEdit->append(QObject::tr("The update is complete, please restart!"));
    m_titleLabel->setText(QObject::tr("Finish"));
    m_btnClose->setVisible(false);
}

void AutoUpdaterUI::NotUpdate()
{
    g_log.log(UpdateLog::INFO, "NotUpdate UI", __FILE__, __LINE__);
    m_curVersionLabel->setText(m_updater->GetOldVersion());
    m_titleLabel->setText(QObject::tr("Laster"));

    if(m_first)
    {
        //This is the main application call updater application from main function
        //and this is not update version at that time, exit update process.
        exit(0);
    }
}

void AutoUpdaterUI::CheckForUpdateError()
{
    g_log.log(UpdateLog::INFO, "Check for update error", __FILE__, __LINE__);

    if(m_first)
    {
        exit(0);
    }

    m_checkForUpdateTimer->stop();
    m_btnOKExit->setVisible(true);

    m_outputVersionInfoEdit->clear();
    m_outputVersionInfoEdit->append(QObject::tr("Error message: "));
    QStringList ftpErrorStack = m_updater->GetFtpErrorStack();
    for(int i = 0 ; i < ftpErrorStack.size(); i++)
    {
        g_log.log(UpdateLog::WARN, "Check for update error: " + ftpErrorStack.at(i), __FILE__, __LINE__);
        m_outputVersionInfoEdit->append(ftpErrorStack.at(i));
    }

    m_outputVersionInfoEdit->append(QObject::tr("Check for update failed!"));
    m_outputVersionInfoEdit->append(QObject::tr("Please check the network link status!"));

    //exit.
    m_updater->AbnormalExit();
}

void AutoUpdaterUI::UpdatingError()
{
    m_isUpdatingError = true;
    m_updatingTimer->stop();
    m_updateProgressBar->setVisible(false);
    m_btnOKExit->setVisible(true);

    m_outputVersionInfoEdit->append(QObject::tr("Error message: "));
    QStringList ftpErrorStack = m_updater->GetFtpErrorStack();
    for(int i = 0 ; i < ftpErrorStack.size(); i++)
    {
        g_log.log(UpdateLog::WARN, "Update error: " + ftpErrorStack.at(i), __FILE__, __LINE__);
        m_outputVersionInfoEdit->append(ftpErrorStack.at(i));
    }

    m_outputVersionInfoEdit->append(QObject::tr("Update failed!"));
    m_outputVersionInfoEdit->append(QObject::tr("Please check the network link status!"));
    m_outputVersionInfoEdit->append(QObject::tr("Or contact us: www.ancubic.com"));

    //exit.
    m_updater->AbnormalExit();
}

void AutoUpdaterUI::ShowWhichUI(const QList<QWidget *> &widgets, bool visible)
{
    for(int i = 0; i < widgets.size(); ++i)
    {
        widgets.at(i)->setVisible(visible);
    }
}

void AutoUpdaterUI::slotDownloadInitFileOver()
{
    g_log.log(UpdateLog::INFO, "It is success that download updater.xml, versionInfoCh.txt and versionInfoEn.txt",
              __FILE__, __LINE__);
    m_checkForUpdateTimer->stop();
    m_outputVersionInfoEdit->clear();

    //Get the version information from download file.
    QString strVersionInfo = m_updater->GetVersionInfo();
    if(strVersionInfo.isEmpty())
    {
        g_log.log(UpdateLog::WARN, "Version information is missing, please check version information file whether is normal",
                  __FILE__, __LINE__);
        m_outputVersionInfoEdit->setText(QObject::tr("Version information is missing"));
        m_outputVersionInfoEdit->append(QObject::tr("Pleasse check network, or contact us: www.anycubic.com"));
    }
    else
    {
        m_outputVersionInfoEdit->setText(strVersionInfo.toLocal8Bit());
    }

    m_outputVersionInfoEdit->moveCursor(QTextCursor::Start);

    //isUpdate is true to update, or not.
    if(m_updater->IsUpdate())
    {
        Update();
        ShowWhichUI(m_updateWidgets, true);
    }
    else
    {
        //This is the laster version so hide update button and cansel button,
        //and show the laster notify message and ok button.
        NotUpdate();
        ShowWhichUI(m_notUpdateWidgets, true);

    }
    this->show();
}

void AutoUpdaterUI::slotDownloadStartPerFile(QString fileName)
{
    if(m_isUpdatingError)
    {
        g_log.log(UpdateLog::WARN, "Download update file failure!",
                  __FILE__, __LINE__);
        return;
    }
    g_log.log(UpdateLog::DEBUG, "Start download file: " + fileName, __FILE__, __LINE__);
    QString startDownload;
    startDownload.append(QObject::tr("Updating "));
    startDownload.append(fileName);
    m_outputVersionInfoEdit->append(startDownload);
}

void AutoUpdaterUI::slotDownloadFinishPerFile(QString fileName)
{
    if(m_isUpdatingError)
    {
        g_log.log(UpdateLog::WARN, "Download update file failure!",
                  __FILE__, __LINE__);
        return;
    }
    g_log.log(UpdateLog::DEBUG, fileName + " download successful.", __FILE__, __LINE__);
    QString startDownload;
    startDownload.append(fileName);
    startDownload.append(QObject::tr(" update is complete!"));
    m_outputVersionInfoEdit->append(startDownload);
}

void AutoUpdaterUI::slotCheckForUpdateTimeout()
{
    //Timer check ftp download error, if a error find, stop update.
    if(CheckFtpDownloadError())
    {
        g_log.log(UpdateLog::INFO, "CheckUpdateTimeout UI", __FILE__, __LINE__);
        //If the call come from parent's main function, do nothing,
        //but exit the update process.

        //Release all ftp resource.
        CheckForUpdateError();

        return;
    }

    static QString tmpStr[3] = {".", "..", "..."};
    static int i = 0;
    if(i == 3)
        i = 0;
    m_outputVersionInfoEdit->setText(QObject::tr("Checking for update ") + QString::asprintf("%1").arg(tmpStr[i++]));
}

void AutoUpdaterUI::slotClickOkExit()
{
    g_log.log(UpdateLog::INFO, "Exit!", __FILE__, __LINE__);
    exit(0);
}

void AutoUpdaterUI::slotBtnUpdateClicked()
{
    Updating();
    ShowWhichUI(m_updateWidgets, false);
    ShowWhichUI(m_updatingWidgets, true);
}

bool AutoUpdaterUI::CheckFtpDownloadError()
{
    //Get error that download update files from ftp, that is if have
    //any error stop download and exit the application.
    if(!m_updater->GetFtpErrorStack().isEmpty())
    {
        return true;
    }
    return false;
}

void AutoUpdaterUI::slotUpdatingTimerout()
{
    //Timer check ftp download error, if a error find, stop update.
    if(CheckFtpDownloadError())
    {
        g_log.log(UpdateLog::INFO, "Updating error", __FILE__, __LINE__);
        UpdatingError();
        return;
    }

    //Updating process.
    m_updateProgressBar->setValue(m_updater->GetUpdateProcess());
    //It is not any error, and finish update.
    if(m_updateProgressBar->value() == m_updateProgressBar->maximum())
    {
        g_log.log(UpdateLog::INFO, "Download update files over.", __FILE__, __LINE__);
        FinishUpdate();
        ShowWhichUI(m_updatingWidgets, false);
        ShowWhichUI(m_finishWidgets, true);
    }
}

void AutoUpdaterUI::slotBtnRestartClicked()
{
    m_updater->RestartApp();
}

void AutoUpdaterUI::Language(bool ch)
{
    if(ch)
    {
        //chinese
        g_log.log(UpdateLog::INFO, "Language is chinese", __FILE__, __LINE__);
        m_qtTranslator.load(":/zh_en.qm");
        qApp->installTranslator(&m_qtTranslator);
    }
    else
    {
        g_log.log(UpdateLog::INFO, "Language is english", __FILE__, __LINE__);
    }
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





