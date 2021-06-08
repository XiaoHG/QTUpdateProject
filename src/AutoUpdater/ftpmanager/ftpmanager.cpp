#include "ftpmanager.h"
#include "loger/log.h"
#include "errorstack/errorstack.h"

#include <QFile>
#include <QNetworkRequest>

extern Log g_log;

bool FtpManager::m_bDownloadError = false;

static const int INITFILE_TIMEOUT = 10;
static const int UPDATEFILE_TIMEOUT = 30;
static const QString HOST = "192.168.4.132";
static const QString PROTOCAL = "ftp";

FtpManager::FtpManager(QString md5, QObject *parent) :
    QObject(parent),
    m_iDownloadTimeSec(0),
    m_iRetryDownloadTimes(1),
    m_strMd5(md5),
    m_timeout(0)
{
    m_url.setScheme(PROTOCAL);
    setHost(HOST);
    m_timerDownloading = new QTimer(this);
    connect(m_timerDownloading, SIGNAL(timeout()), this, SLOT(on_timer_downloading()));
    m_timerDownloading->start(1000);
}

FtpManager::~FtpManager()
{

}

void FtpManager::get(const QString &downloadPath, const QString &localPath)
{
    g_log.log(Log::DEBUG, "Start downloadPath file: " + downloadPath, __FILE__, __LINE__);
    g_log.log(Log::DEBUG, "localPath path: " + localPath, __FILE__, __LINE__);

    if(localPath.contains("updater"))
    {
        m_timeout = INITFILE_TIMEOUT;
    }
    else
    {
        m_timeout = UPDATEFILE_TIMEOUT;
    }

    g_log.log(Log::DEBUG, QString::asprintf("Download time out is : %1").arg(m_timeout), __FILE__, __LINE__);

    m_iDownloadTimeSec = 0;
    m_strLocalPath = localPath;
    m_strDownloadPath = downloadPath;
    signal_startDownload(m_strLocalPath);

    m_url.setPath(downloadPath);
    m_pReply = m_networkManager.get(QNetworkRequest(m_url));
    connect(m_pReply, SIGNAL(finished()), this, SLOT(on_reply_downloadFinish()));
    connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(on_reply_error(QNetworkReply::NetworkError)));
}

void FtpManager::on_reply_downloadFinish()
{
    m_timerDownloading->stop();
    if(m_bDownloadError)
    {
        return;
    }

    //Write to file.
    QFile _file(m_strLocalPath);

    if(_file.exists())
    {
        g_log.log(Log::INFO, "file: " + m_strLocalPath + " exists, befor download delete it first.", __FILE__, __LINE__);
        _file.remove();
    }

    if (!_file.open(QIODevice::ReadWrite))
    {
        QString _strError = "Can't open local file : " + m_strLocalPath;
        g_log.log(Log::WARN, _strError, __FILE__, __LINE__);
        retryDownload();
        return;
    }

    qint64 writeLen = _file.write(m_pReply->readAll());
    if(writeLen <= 0)
    {
        QString _strError = "Download file : " + m_strLocalPath + QString::asprintf(" finish, but content is empty! writeLen = %1").arg(writeLen);
        g_log.log(Log::WARN, _strError, __FILE__, __LINE__);
        retryDownload();
        return;
    }

    g_log.log(Log::DEBUG, "Local: Finish download : " + m_strLocalPath + QString::asprintf(", file lenght = %1").arg(writeLen) , __FILE__, __LINE__);

    _file.flush();
    _file.close();

    if(m_strLocalPath.contains("LatestV.xml"))
    {
        g_log.log(Log::DEBUG, "Finish updater LatestV.xml file!", __FILE__, __LINE__);
        signal_downloadLatestVFinish();
        return;
    }

    if(!isMatchMd5())
    {
        return;
    }

    g_log.log(Log::INFO, QString::asprintf("Server: Finish download %1 file!").arg(m_url.path()), __FILE__, __LINE__);
    signal_finishDownload(m_strLocalPath);
}

bool FtpManager::isMatchMd5()
{
    if(m_strMd5 == "0")
    {
        return true;
    }

    g_log.log(Log::INFO, "Check MD5, file: " + m_strLocalPath, __FILE__, __LINE__);

    QFile _file(m_strLocalPath);
    _file.open(QIODevice::ReadOnly);
    QByteArray _baMd5 = QCryptographicHash::hash(_file.readAll(), QCryptographicHash::Md5);
    _file.close();

    QString _strDownloadMd5 = _baMd5.toHex();

    g_log.log(Log::INFO, "Check MD5, xml file md5 = " + m_strMd5 + ", download file md5 = " + _strDownloadMd5, __FILE__, __LINE__);

    //check
    if(_strDownloadMd5 != m_strMd5)
    {
        QString _strErrLogMsg = m_strDownloadPath + " md5 compare error! download md5 = " + _strDownloadMd5 + ", xml file md5 = " + m_strMd5;
        reportError(_strErrLogMsg, MD5_CHECK_ERR, g_mapError.find(MD5_CHECK_ERR).value());
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
    g_log.log(Log::DEBUG, "Download file " + m_url.path() + " time: " + QString::asprintf("%1").arg(m_iDownloadTimeSec),
              __FILE__, __LINE__);
    if(m_iDownloadTimeSec++ == m_timeout)
    {
        m_timerDownloading->stop();

        QString _strErrLogMsg = "Download file " + m_url.path() + " time out";
        reportError(_strErrLogMsg, DOWNLOAD_TIMEOUT, g_mapError.find(DOWNLOAD_TIMEOUT).value());
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

    QString _strErrLogMsg = "Download error: code = ";
    _strErrLogMsg.append(QString::asprintf("%1").arg(errorCode));
    _strErrLogMsg.append(". error string = ");
    _strErrLogMsg.append(m_pReply->errorString());

    //record error message.
    reportError(_strErrLogMsg, errorCode, m_pReply->errorString());
}

void FtpManager::retryDownload()
{
    //Log
    QString _strErrorMsg = QString::asprintf("Try time = %1, Download error: code = ").arg(m_iRetryDownloadTimes);
    _strErrorMsg.append(QString::asprintf("%1").arg(m_pReply->error()));
    _strErrorMsg.append(". error string = ");
    _strErrorMsg.append(m_pReply->errorString());
    g_log.log(Log::WARN, _strErrorMsg, __FILE__, __LINE__);

    m_iRetryDownloadTimes++;
    if(m_iRetryDownloadTimes > 3)
    {
        //download over with fail.
        QString _strErrLogMsg = "Retry download file " + m_url.path() + " times over 3.";
        reportError(_strErrLogMsg, RETRYDOWNLOAD_TIMESOVER, g_mapError.find(RETRYDOWNLOAD_TIMESOVER).value());
    }

    //Download retry
    m_pReply->disconnect(this);
    get(m_strDownloadPath, m_strLocalPath);
}

void FtpManager::reportError(const QString &strLogMsg, const int &eErrStackCode, const QString &strErrStack)
{
    m_bDownloadError = true;
    g_log.log(Log::FATAL, strLogMsg, __FILE__, __LINE__);
    ErrorStack::pushError(eErrStackCode, strErrStack);
    signal_replyError(strErrStack);
}






