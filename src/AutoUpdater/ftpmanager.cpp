#include "ftpmanager.h"
#include "updatelog.h"

#include <QFile>
#include <QNetworkRequest>

extern UpdateLog g_log;

bool FtpManager::m_bDownloadError = false;

static const int INITFILE_TIMEOUT = 10;
static const int UPDATEFILE_TIMEOUT = 30;

FtpManager::FtpManager(QString md5, QObject *parent) :
    QObject(parent),
    m_iDownloadTimeSec(0),
    m_iRetryDownloadTimes(1),
    m_strMd5(md5),
    m_timeout(0)
{
    m_url.setScheme("ftp");
    setHost("192.168.4.132");
    m_timerDownloading = new QTimer(this);
    connect(m_timerDownloading, SIGNAL(timeout()), this, SLOT(on_timer_downloading()));
    m_timerDownloading->start(1000);
}

FtpManager::~FtpManager()
{

}

void FtpManager::get(const QString &downloadPath, const QString &localPath)
{
    g_log.log(UpdateLog::INFO, "Start downloadPath file: " + downloadPath, __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, "localPath path: " + localPath, __FILE__, __LINE__);

    if(localPath.contains("download"))
    {
        m_timeout = INITFILE_TIMEOUT;
    }
    else
    {
        m_timeout = UPDATEFILE_TIMEOUT;
    }

    g_log.log(UpdateLog::INFO, QString::asprintf("Download time out is : %1").arg(m_timeout), __FILE__, __LINE__);

    m_iDownloadTimeSec = 0;
    m_strLocalPath = localPath;
    m_strDownloadPath = downloadPath;
    signal_startDownloadPerFile(m_strDownloadPath);

    m_url.setPath(downloadPath);
    m_pReply = m_networkManager.get(QNetworkRequest(m_url));
    connect(m_pReply, SIGNAL(finished()), this, SLOT(on_reply_downloadFinish()));
    connect(m_pReply, SIGNAL(on_reply_error(QNetworkReply::NetworkError)), this, SLOT(on_reply_error(QNetworkReply::NetworkError)));
}

void FtpManager::on_reply_downloadFinish()
{
    m_timerDownloading->stop();
    if(m_bDownloadError)
    {
        return;
    }

    //Write to file.
    QFile file(m_strLocalPath);
    if (!file.open(QIODevice::ReadWrite))
    {
        g_log.log(UpdateLog::FATAL, "Can't open local file : " + m_strLocalPath, __FILE__, __LINE__);
        reportError("Can't open local file : " + m_strLocalPath);
        return;
    }

    qint64 writeLen = file.write(m_pReply->readAll());
    if(writeLen <= 0)
    {
        QString strError = "Download file : " + m_strLocalPath + " finish, but content is empty!";
        g_log.log(UpdateLog::FATAL, strError, __FILE__, __LINE__);
        reportError(strError);
        file.close();
        return;
    }

    g_log.log(UpdateLog::FATAL, "Finish download : " + m_strLocalPath + QString::asprintf(", file lenght = %1").arg(writeLen) , __FILE__, __LINE__);

    file.flush();
    file.close();

    if(m_strLocalPath.contains("/download/updater.xml"))
    {
        g_log.log(UpdateLog::INFO, "Finish download updater.xml file!", __FILE__, __LINE__);
        signal_downloadXmlFinish();
        return;
    }
    if(m_strLocalPath.contains("/download/versionInfoCh.txt"))
    {
        g_log.log(UpdateLog::INFO, "Finish download versionInfoCh.txt file!", __FILE__, __LINE__);
        signal_downloadChFinish();
        return;
    }
    if(m_strLocalPath.contains("/download/versionInfoEn.txt"))
    {
        g_log.log(UpdateLog::INFO, "Finish download versionInfoEn.txt file!", __FILE__, __LINE__);
        signal_downloadEnFinish();
        return;
    }

    if(!isMatchMd5())
    {
        //download failure;
        return;
    }

    g_log.log(UpdateLog::INFO, QString::asprintf("Finish download %1 file!").arg(m_url.path()), __FILE__, __LINE__);
    signal_finishDownloadPerFile(m_url.path());
}

bool FtpManager::isMatchMd5()
{
    if(m_strMd5 == "0")
    {
        return true;
    }

    g_log.log(UpdateLog::INFO, "Check MD5, file: " + m_strLocalPath, __FILE__, __LINE__);

    QFile theFile(m_strLocalPath);
    theFile.open(QIODevice::ReadOnly);
    QByteArray baMd5 = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
    theFile.close();

    QString downloadMd5 = baMd5.toHex();

    g_log.log(UpdateLog::INFO, "Check MD5, xml file md5 = " + m_strMd5 + ", download file md5 = " + downloadMd5, __FILE__, __LINE__);

    //check
    if(downloadMd5 != m_strMd5)
    {
        QString errStr = m_strDownloadPath + " md5 compare error! download md5 = " + downloadMd5 + ", xml file md5 = " + m_strMd5;
        reportError(errStr);
        return false;
    }

    return true;
}

void FtpManager::on_timer_downloading()
{
    if(m_bDownloadError)
    {
        m_timerDownloading->stop();
        return;
    }
    g_log.log(UpdateLog::FATAL, "Download file " + m_url.path() + " time: " + QString::asprintf("%1").arg(m_iDownloadTimeSec),
              __FILE__, __LINE__);
    if(m_iDownloadTimeSec++ == m_timeout)
    {
        g_log.log(UpdateLog::FATAL, "Download file " + m_url.path() + " time out", __FILE__, __LINE__);
        m_timerDownloading->stop();

        //It is emited to AutoUpdater.
        QString timeoutMsg;
        timeoutMsg.append(m_url.path());
        timeoutMsg.append(QObject::tr(" file download time out!"));
        reportError(timeoutMsg);
    }
}

void FtpManager::on_reply_error(QNetworkReply::NetworkError errorCode)
{
    switch (errorCode) {
    case QNetworkReply::ContentAccessDenied:
        retryDownload();
        return;
    default:
        break;
    }

	QString errorMsg = "Download error: code = ";
    errorMsg.append(QString::asprintf("%1").arg(errorCode));
	errorMsg.append(". error string = ");
	errorMsg.append(m_pReply->errorString());
    g_log.log(UpdateLog::FATAL, errorMsg, __FILE__, __LINE__);

    reportError(m_pReply->errorString());
}

void FtpManager::retryDownload()
{
    //Log
    QString errorMsg = QString::asprintf("Try time = %1, Download error: code = ").arg(m_iRetryDownloadTimes);
    errorMsg.append(QString::asprintf("%1").arg(m_pReply->error()));
    errorMsg.append(". error string = ");
    errorMsg.append(m_pReply->errorString());
    g_log.log(UpdateLog::WARN, errorMsg, __FILE__, __LINE__);

    m_iRetryDownloadTimes++;
    if(m_iRetryDownloadTimes > 3)
    {
        //download over with fail.
        reportError(m_pReply->errorString());
    }

    //Download retry
    m_pReply->disconnect(this);
    get(m_strDownloadPath, m_strLocalPath);
}

void FtpManager::reportError(QString error)
{
    //It is emited to AutoUpdater.
    emit signal_replyError(error);
    m_bDownloadError = true;
}






