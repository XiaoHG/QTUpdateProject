
#include "updaterui.h"
#include "log.h"


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
#include <QDesktopWidget>

extern Log g_log;

#define CHECKUPDATE_TIMEOUT 15
static const float DEVELOPMENT_DESTOP_WIDTH = 1920.0;
static const float DEVELOPMENT_DESTOP_HEIGHT = 1080.0;

UpdaterUI::UpdaterUI(bool bCh, QWidget *parent)
    :QMainWindow(parent),
      m_bUpdatingError(false)
{
    //Language set from parent.
    language(bCh);

    //Init UI
    initUI();

    //Init update manager
    m_updater = new Updater(bCh);

    //It is download files that updater.xml and versionInfoCh.txt,
    //and is emit the signal of init files download over,
    //and next step to check betweed download updater.xml and local xml file
    //get whether update or not result.
    connect(m_updater, SIGNAL(signal_initFileDownloadFinish()),
            this, SLOT(on_updater_initFileDownloadFinish()));

    //It is emited that each file download at start.
    connect(m_updater, SIGNAL(signal_startDownloadPerFile(QString)),
            this, SLOT(on_updater_startDownloadPerFile(QString)));

    //It is emited that each file download finish.
    connect(m_updater, SIGNAL(signal_finishDownloadPerFile(QString)),
            this, SLOT(on_updater_finishDownloadPerFile(QString)));

    //This timer monitor the init file download error if.
    //check per 100ms.
    m_timerCheckForUpdate = new QTimer(this);
    connect(m_timerCheckForUpdate, SIGNAL(timeout()), this, SLOT(on_timer_checkForUpdate()));
    m_timerCheckForUpdate->start(100);

    //update prosess timer
    m_timerUpdating = new QTimer(this);
    connect(m_timerUpdating, &QTimer::timeout, this, &UpdaterUI::on_timer_updating);
}

UpdaterUI::~UpdaterUI()
{
    if(m_updater)
    {
        m_updater->deleteLater();
    }
}

/*UI defined*/
void UpdaterUI::initUI()
{
    setStyleSheet("background-color:rgb(100, 100, 100);");
    setWindowFlags(Qt::FramelessWindowHint);
    resize(500, 500);

    //line edit widget to version notify
    QFont _fontTitleLabel( "Microsoft YaHei", 11, 75);
    m_labelTitle = new QLabel(this);
    m_labelTitle->setGeometry(0, 0, this->width(), this->height() / 10);
    m_labelTitle->setFont(_fontTitleLabel);
    m_labelTitle->setAlignment(Qt::AlignCenter);
    m_labelTitle->setScaledContents(true);
    m_labelTitle->setText(QObject::tr("Checking for update"));
    m_labelTitle->setStyleSheet("background-color:rgb(50, 50, 50);"
                                "color:rgb(200, 200, 200)");

    //add a Text edit widget for output file that need to update
    m_teOutputVersionInfo = new QTextEdit(this);
    m_teOutputVersionInfo->setFocusPolicy(Qt::NoFocus);
    m_teOutputVersionInfo->setWindowFlags(Qt::FramelessWindowHint);
    m_teOutputVersionInfo->setGeometry(20, m_labelTitle->height() + 40, this->width() - 40,
                            this->height() - m_labelTitle->height() - 100);
    m_teOutputVersionInfo->setStyleSheet("background-color:rgb(100, 100, 100);"
                                           "color:rgb(200, 200, 200)");
    m_teOutputVersionInfo->setTextColor(QColor(200, 200, 200, 255));
    m_teOutputVersionInfo->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
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

    QFont _fontLogTitleLabel( "Microsoft YaHei", 9, 75);
    m_labelLogTitle = new QLabel(this);
    m_labelLogTitle->setFont(_fontLogTitleLabel);
    m_labelLogTitle->setStyleSheet("color:white");
    m_labelLogTitle->setGeometry(m_teOutputVersionInfo->x(),
                                 m_teOutputVersionInfo->y() - 25,
                                 m_teOutputVersionInfo->width() - 100,
                                 20);
    m_labelLogTitle->setScaledContents(true);
    m_labelLogTitle->setText(QObject::tr("log :"));
    m_labelLogTitle->setStyleSheet("color:rgb(200, 200, 200)");

    m_btnClose = new QPushButton(this);
    m_btnClose->setIcon(QIcon(":/icon/close.png"));
    m_btnClose->setGeometry(this->width() - m_labelTitle->height() - 10, 5,
                          m_labelTitle->height() + 5, m_labelTitle->height() - 10);
    m_btnClose->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                              "QPushButton:hover{background-color:rgb(18, 237, 237);}"
                              "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");
    m_btnClose->setFlat(true);
    connect(m_btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));

    updateUI();
    updatingUI();
    finishUpdateUI();
    notUpdateUI();
    downloadErrorUI();

    m_autoResizeHandler = new AutoResize(this, this->rect().width(), this->rect().height());
    m_autoResizeHandler->pushAllResizeItem();

    if(QApplication::desktop()->width() > DEVELOPMENT_DESTOP_WIDTH && QApplication::desktop()->height() > DEVELOPMENT_DESTOP_HEIGHT)
    {
        float widthRatio = QApplication::desktop()->width() / DEVELOPMENT_DESTOP_WIDTH;
        float heightRatio = QApplication::desktop()->height() / DEVELOPMENT_DESTOP_HEIGHT;
        float ratio = widthRatio > heightRatio ? widthRatio : heightRatio;
        g_log.log(Log::INFO, QString::asprintf("Main window width ratio = %1, height ratio = %2")
                  .arg(widthRatio).arg(heightRatio), __FILE__, __LINE__);
        int windowWidth = this->width() * ratio;
        int windowHeight = this->height() * ratio;
        resize(windowWidth, windowHeight);
    }

    m_labelLogTitle->setVisible(true);
    m_teOutputVersionInfo->setVisible(true);
    m_btnClose->setVisible(true);
}

void UpdaterUI::updateUI()
{
    QFont _fontBtnUpdate( "Microsoft YaHei", 9, 75);
    m_btnUpdate = new QPushButton(this);
    m_btnUpdate->setFont(_fontBtnUpdate);
    m_btnUpdate->setText(QObject::tr("Update"));
    m_btnUpdate->setIcon(QIcon(":/icon/update2.png"));
    m_btnUpdate->setGeometry(20, m_teOutputVersionInfo->height() + m_labelTitle->height() + 50, 70, 25);
    m_btnUpdate->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                                "QPushButton:hover{background-color:rgb(18, 237, 237); color: black;}"
                                "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");

    //new version info
    m_labelHaveNewVersionNotise = new QLabel(this);
    m_labelHaveNewVersionNotise->setGeometry(m_btnUpdate->x() + m_btnUpdate->width() + 10,
                                       m_btnUpdate->y(),
                                       m_teOutputVersionInfo->width() - m_btnUpdate->width() - 10,
                                       m_btnUpdate->height());
    m_labelHaveNewVersionNotise->setScaledContents(true);
    m_labelHaveNewVersionNotise->setWordWrap(true);
    m_labelHaveNewVersionNotise->setStyleSheet("color:rgb(200, 200, 200)");

    connect(m_btnUpdate, SIGNAL(clicked(bool)), this, SLOT(on_btn_update_clicked()));

    m_listUpdateWidgets.push_back(m_btnUpdate);
    m_listUpdateWidgets.push_back(m_labelHaveNewVersionNotise);

    //init false
    //ShowUpdateUI(false);
    showWhichUI(m_listUpdateWidgets, false);
}

void UpdaterUI::updatingUI()
{
    m_pbUpdating = new QProgressBar(this);
    m_pbUpdating->setGeometry(m_teOutputVersionInfo->x(), this->height() - 50,
                                       m_teOutputVersionInfo->width(), 20);
    m_pbUpdating->setStyleSheet("QProgressBar{"
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

    m_listUpdatingWidgets.push_back(m_pbUpdating);

    //init false
    showWhichUI(m_listUpdatingWidgets, false);
}

void UpdaterUI::finishUpdateUI()
{
    m_btnRestart = new QPushButton(this);
    m_btnRestart->setText(QObject::tr("Restart"));
    m_btnRestart->setIcon(QIcon(":/icon/restart.png"));
    m_btnRestart->setGeometry(m_btnUpdate->x(), m_btnUpdate->y(),
                         m_btnUpdate->width(), m_btnUpdate->height());
    m_btnRestart->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                                "QPushButton:hover{background-color:rgb(18, 237, 237); color: black;}"
                                "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");

    connect(m_btnRestart, SIGNAL(clicked(bool)), this, SLOT(on_btn_restart_clicked()));

    m_listFinishWidgets.push_back(m_btnRestart);

    //init false
    showWhichUI(m_listFinishWidgets, false);
}

void UpdaterUI::notUpdateUI()
{
    QFont _fontLabelLasterVersion( "Microsoft YaHei", 10, 75);
    m_labelCurrentVersion = new QLabel(this);
    m_labelCurrentVersion->setFont(_fontLabelLasterVersion);
    m_labelCurrentVersion->setGeometry(m_btnUpdate->x() + 5,
                                      m_btnUpdate->y() - 10,
                                      m_teOutputVersionInfo->width(),
                                      30);
    m_labelCurrentVersion->setStyleSheet("color:rgb(150, 150, 150)");

    m_listNotUpdateWidgets.push_back(m_labelCurrentVersion);

    //init false
    showWhichUI(m_listNotUpdateWidgets, false);
}

void UpdaterUI::downloadErrorUI()
{
    m_btnExit = new QPushButton(this);
    m_btnExit->setText(QObject::tr("ok"));
    m_btnExit->setGeometry(m_btnUpdate->x(), m_btnUpdate->y(),
                                     m_btnUpdate->width(), m_btnUpdate->height());
    m_btnExit->setStyleSheet("QPushButton{background-color:rgba(50, 50, 50, 100%); color:white; border-radius: 6;}"
                            "QPushButton:hover{background-color:rgb(18, 237, 237); color: black;}"
                            "QPushButton:pressed{background-color:rgb(18, 237, 237); border-style: inset; }");
    connect(m_btnExit, SIGNAL(clicked(bool)), this, SLOT(on_btn_exit_clicked()));

    m_listDownloadErrorWidgets.push_back(m_btnExit);

    showWhichUI(m_listDownloadErrorWidgets, false);
}

void UpdaterUI::updater(bool isFromParentMain, QString parentPid)
{
    m_bFromParentMain = isFromParentMain;

    //If enter is from parent main function, and show the checking UI.
    if(!m_bFromParentMain)
        this->show();
    g_log.log(Log::DEBUG, "Beging checking for update timer, It is time out at 30 second!", __FILE__, __LINE__);

    //set parent pid
    m_updater->setParentPid(parentPid);

    m_updater->deleteOldPath();

    //start to check for update.
    m_updater->downloadLasterVFile();
}

void UpdaterUI::checkForUpdate()
{
    m_teOutputVersionInfo->append(QObject::tr("Checking for update "));
}

void UpdaterUI::update()
{
    g_log.log(Log::INFO, "Update UI", __FILE__, __LINE__);
    m_labelTitle->setText(tr("Update dialog"));
    m_btnClose->setVisible(true);

    m_strVersionServerInfo = m_updater->getNewVersion();
    m_labelHaveNewVersionNotise->setText(QObject::tr("Find ") + m_strVersionServerInfo +
                                   QObject::tr(" version, click update button to update!"));

    //Load download files path from ftp server.
    m_updater->loadUpdateFiles();
}

void UpdaterUI::showVersionInfo()
{
    m_teOutputVersionInfo->clear();
    m_teOutputVersionInfo->append(QObject::tr("Current version is ") + m_updater->getOldVersion());
    m_teOutputVersionInfo->append(QObject::tr(" "));
    m_teOutputVersionInfo->append(QObject::tr("Changed log of history: "));

    //Get the version information from download file.
    QString _strVersionInfo = m_updater->getVersionInfo();
    if (_strVersionInfo.isEmpty())
    {
        g_log.log(Log::WARN, "Version information is missing, please check version information file whether is normal",
                    __FILE__, __LINE__);
        m_teOutputVersionInfo->setText(QObject::tr("Version information is missing!"));
        m_teOutputVersionInfo->append(QObject::tr("Pleasse check network, or contact us: www.anycubic.com"));
    }
    else
    {
        m_teOutputVersionInfo->append(_strVersionInfo.toLocal8Bit());
    }

    m_teOutputVersionInfo->moveCursor(QTextCursor::Start);
}

void UpdaterUI::updating()
{
    g_log.log(Log::INFO, "Updating UI", __FILE__, __LINE__);
    g_log.log(Log::INFO, "Update process timer start at 50ms", __FILE__, __LINE__);
    //update,and start updateProsessTimer
    m_timerUpdating->start(100);
    m_teOutputVersionInfo->clear();
    m_labelTitle->setText(QObject::tr("Updating"));

    //Download all files which check out update files.
    m_updater->downloadUpdateFiles();
}

void UpdaterUI::finishUpdate()
{
    g_log.log(Log::INFO, "FinishUpdate UI", __FILE__, __LINE__);
    m_timerUpdating->stop();
    m_teOutputVersionInfo->append(QObject::tr("The update is complete, please restart!"));
    m_labelTitle->setText(QObject::tr("Finish"));
    m_btnClose->setVisible(false);
}

void UpdaterUI::notUpdate()
{
    g_log.log(Log::INFO, "NotUpdate UI", __FILE__, __LINE__);
    m_labelCurrentVersion->setText(m_updater->getOldVersion());
    m_labelTitle->setText(QObject::tr("Laster"));

    if(m_bFromParentMain)
    {
        //This is the main application call updater application from main function
        //and this is not update version at that time, exit update process.
        exit(0);
    }
}

void UpdaterUI::checkForUpdateError()
{
    g_log.log(Log::INFO, "Check for update error", __FILE__, __LINE__);

    if(m_bFromParentMain)
    {
        exit(0);
    }

    m_timerCheckForUpdate->stop();
    m_btnExit->setVisible(true);

    m_labelTitle->setText(QObject::tr("Error"));
    m_teOutputVersionInfo->clear();
    m_teOutputVersionInfo->append(QObject::tr("Check for update failed!"));
    m_teOutputVersionInfo->append(QObject::tr("Error message: "));
    QStringList _listFtpErrorStack = m_updater->getFtpErrorStack();
    for(int i = 0 ; i < _listFtpErrorStack.size(); i++)
    {
        g_log.log(Log::WARN, "Check for update error: " + _listFtpErrorStack.at(i), __FILE__, __LINE__);
        m_teOutputVersionInfo->append(_listFtpErrorStack.at(i));
    }
    m_teOutputVersionInfo->append(QObject::tr("Please check the network link status!"));
    m_teOutputVersionInfo->append(QObject::tr("Or contact us: www.anycubic.com"));

    //exit.
    m_updater->abnormalExit();
}

void UpdaterUI::updatingError()
{
    m_bUpdatingError = true;
    m_timerUpdating->stop();
    m_pbUpdating->setVisible(false);
    m_btnExit->setVisible(true);

    m_labelTitle->setText(QObject::tr("Error"));
    m_teOutputVersionInfo->append(QObject::tr("Update failed!"));
    m_teOutputVersionInfo->append(QObject::tr("Error message: "));
    QStringList ftpErrorStack = m_updater->getFtpErrorStack();
    for(int i = 0 ; i < ftpErrorStack.size(); i++)
    {
        g_log.log(Log::WARN, "Update error: " + ftpErrorStack.at(i), __FILE__, __LINE__);
        m_teOutputVersionInfo->append(ftpErrorStack.at(i));
    }

    m_teOutputVersionInfo->append(QObject::tr("Please check the network link status!"));
    m_teOutputVersionInfo->append(QObject::tr("Or contact us: www.anycubic.com."));

    //exit.
    m_updater->abnormalExit();
}

void UpdaterUI::showWhichUI(const QList<QWidget *> &widgets, bool visible)
{
    for(int i = 0; i < widgets.size(); ++i)
    {
        widgets.at(i)->setVisible(visible);
    }
}

void UpdaterUI::on_updater_initFileDownloadFinish()
{
    m_timerCheckForUpdate->stop();

    //isUpdate is true to update, or not.

    if(m_updater->isUpdate())
    {
        update();
        showWhichUI(m_listUpdateWidgets, true);
    }
    else
    {
        //This is the laster version so hide update button and cansel button,
        //and show the laster notify message and ok button.
        notUpdate();
        showWhichUI(m_listNotUpdateWidgets, true);
    }

    showVersionInfo();

    this->show();
}

void UpdaterUI::parseXmlError(Updater::UPDATER_ERROR_CODE code)
{
    g_log.log(Log::INFO, "Xml file parse error UI", __FILE__, __LINE__);
    m_labelTitle->setText(QObject::tr("Error"));
    m_btnExit->setVisible(true);

    if(m_bFromParentMain)
    {
        //This is the main application call updater application from main function
        //and this is not update version at that time, exit update process.
        exit(0);
    }

    m_teOutputVersionInfo->clear();

    switch (code) {
        case Updater::LOCALXML_PARSE_ERR:
            m_teOutputVersionInfo->append(QObject::tr("Local xml version file parse error!"));
            m_teOutputVersionInfo->append(QObject::tr("Local xml version file unusual, please check it."));
            m_teOutputVersionInfo->append(QObject::tr("Or contact us: www.anycubic.com"));
            break;
        case Updater::DOWNLOADXML_PARSE_ERR:
            m_teOutputVersionInfo->append(QObject::tr("Download xml version file parse error!"));
            m_teOutputVersionInfo->append(QObject::tr("Pleasse check network, or contact us: www.anycubic.com."));
            break;
        default:
            break;
    }

}

void UpdaterUI::on_updater_startDownloadPerFile(QString fileName)
{
    if(m_bUpdatingError)
    {
        g_log.log(Log::WARN, "Download update file failure!",
                  __FILE__, __LINE__);
        return;
    }
    g_log.log(Log::DEBUG, "Start update file: " + fileName, __FILE__, __LINE__);
    QString _strStartDownload;
    _strStartDownload.append(QObject::tr("Updating "));
    _strStartDownload.append(fileName);
    m_teOutputVersionInfo->append(_strStartDownload);
}

void UpdaterUI::on_updater_finishDownloadPerFile(QString fileName)
{
    if(m_bUpdatingError)
    {
        g_log.log(Log::WARN, "Download update file failure!",
                  __FILE__, __LINE__);
        return;
    }
    g_log.log(Log::DEBUG, fileName + " update successful.", __FILE__, __LINE__);
    QString _strStartDownload;
    _strStartDownload.append(fileName);
    _strStartDownload.append(QObject::tr(" update is complete!"));
    m_teOutputVersionInfo->append(_strStartDownload);
}

void UpdaterUI::on_timer_checkForUpdate()
{
    //Timer check ftp download error, if a error find, stop update.
    if(isFtpDownloadError())
    {

        checkForUpdateError();

        return;
    }

    static QString _s_strTmp[3] = {".", "..", "..."};
    static int _s_iDoct = 0;
    if(_s_iDoct == 3)
        _s_iDoct = 0;
    m_teOutputVersionInfo->setText(QObject::tr("Checking for update ") + QString::asprintf("%1").arg(_s_strTmp[_s_iDoct++]));
}

void UpdaterUI::on_btn_exit_clicked()
{
    g_log.log(Log::INFO, "Exit!", __FILE__, __LINE__);
    exit(0);
}

void UpdaterUI::on_btn_update_clicked()
{
    updating();
    showWhichUI(m_listUpdateWidgets, false);
    showWhichUI(m_listUpdatingWidgets, true);
}

bool UpdaterUI::isFtpDownloadError()
{
    //Get error that download update files from ftp, that is if have
    //any error stop download and exit the application.
    if(!m_updater->getFtpErrorStack().isEmpty())
    {
        return true;
    }
    return false;
}

void UpdaterUI::on_timer_updating()
{
    //Timer check ftp download error, if a error find, stop update.
    if(isFtpDownloadError())
    {
        g_log.log(Log::INFO, "Updating error", __FILE__, __LINE__);
        updatingError();
        return;
    }

    //Updating process.
    m_pbUpdating->setValue(m_updater->getUpdateProcess());
    //It is not any error, and finish update.
    if(m_pbUpdating->value() == m_pbUpdating->maximum())
    {
        g_log.log(Log::INFO, "Download update files over.", __FILE__, __LINE__);
        finishUpdate();
        showWhichUI(m_listUpdatingWidgets, false);
        showWhichUI(m_listFinishWidgets, true);
    }
}

void UpdaterUI::on_btn_restart_clicked()
{
    m_updater->restartApp();
}

void UpdaterUI::language(bool ch)
{
    if(ch)
    {
        //chinese
        g_log.log(Log::INFO, "Language is chinese", __FILE__, __LINE__);
        m_translator.load(":/zh_en.qm");
        qApp->installTranslator(&m_translator);
    }
    else
    {
        g_log.log(Log::INFO, "Language is english", __FILE__, __LINE__);
    }
}

void UpdaterUI::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QBitmap _bmp(this->size());
    _bmp.fill();
    QPainter _p(&_bmp);
    _p.setPen(Qt::NoPen);
    _p.setBrush(Qt::black);
    _p.drawRoundedRect(_bmp.rect(), 10, 10);
    setMask(_bmp);
}

void UpdaterUI::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
    m_autoResizeHandler->doAutoResize();
}

void UpdaterUI::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        m_pointMouseStart = event->globalPos();
        m_pointWindowTopLeft = this->frameGeometry().topLeft();
    }
}

void UpdaterUI::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        QPoint distance = event->globalPos() - m_pointMouseStart;
        this->move(m_pointWindowTopLeft + distance);
    }
}

void UpdaterUI::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

