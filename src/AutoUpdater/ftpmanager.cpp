﻿#include "ftpmanager.h"
#include "updatelog.h"

#include <QFile>
#include <QNetworkRequest>

extern UpdateLog g_log;

bool FtpManager::m_isDownloadError = false;

FtpManager::FtpManager(QString md5, QObject *parent) :
    QObject(parent),
    m_timeout(0),
    m_retryDownloadTimes(1),
    m_md5(md5)
{
    m_url.setScheme("ftp");
    setHost("192.168.4.132");
    m_downloadTimeout = new QTimer(this);
    connect(m_downloadTimeout, SIGNAL(timeout()), this, SLOT(slotDownloadTimeout()));
    m_downloadTimeout->start(1000);
}

FtpManager::~FtpManager()
{

}

void FtpManager::get(const QString &downloadPath, const QString &localPath)
{
    g_log.log(UpdateLog::INFO, "Start download file: " + downloadPath, __FILE__, __LINE__);
    m_timeout = 0;

    m_localPath = localPath;
    m_downloadPath = downloadPath;
    sigDownloadStartPerFile(m_downloadPath);

    m_url.setPath(downloadPath);
    m_pReply = m_manager.get(QNetworkRequest(m_url));
    connect(m_pReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(Error(QNetworkReply::NetworkError)));
}

void FtpManager::downloadFinished()
{
    m_downloadTimeout->stop();
    if(m_isDownloadError)
    {
        return;
    }

    //Write to file.
    QFile file(m_localPath);
    if (!file.open(QIODevice::WriteOnly))
    {
        g_log.log(UpdateLog::FATAL, "Can't open local file : " + m_localPath, __FILE__, __LINE__);
        sigReplyError("Can't open local file : " + m_localPath);
        return;
    }

    file.write(m_pReply->readAll());
    file.flush();
    file.close();

    if(m_localPath.contains("/download/updater.xml"))
    {
        g_log.log(UpdateLog::INFO, "Finish download updater.xml file!", __FILE__, __LINE__);
        sigDownloadUpdaterXmlOver();
        return;
    }
    if(m_localPath.contains("/download/versionInfoCh.txt"))
    {
        g_log.log(UpdateLog::INFO, "Finish download versionInfoCh.txt file!", __FILE__, __LINE__);
        sigDownloadVersionInfoFileOver();
        return;
    }
    if(m_localPath.contains("/download/versionInfoEn.txt"))
    {
        g_log.log(UpdateLog::INFO, "Finish download versionInfoEn.txt file!", __FILE__, __LINE__);
        sigDownloadVersionInfoEnfileOver();
        return;
    }

    if(!checkMd5())
    {
        return;
    }

    g_log.log(UpdateLog::INFO, QString::asprintf("Finish download %1 file!").arg(m_url.path()), __FILE__, __LINE__);
    sigDownloadFinishPerFile(m_url.path());
}

bool FtpManager::checkMd5()
{
    if(m_md5 == "0")
    {
        return true;
    }

    g_log.log(UpdateLog::INFO, "Check MD5, file: " + m_localPath, __FILE__, __LINE__);

    QFile theFile(m_localPath);
    theFile.open(QIODevice::ReadOnly);
    QByteArray baMd5 = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
    theFile.close();

    QString downloadMd5 = baMd5.toHex();

    g_log.log(UpdateLog::INFO, "Check MD5, xml file md5 = " + m_md5 + ", download file md5 = " + downloadMd5, __FILE__, __LINE__);

    //check
    if(downloadMd5 != m_md5)
    {
        //download failure;
        QString errStr = m_downloadPath + " md5 compare error! download md5 = " + downloadMd5 + ", xml file md5 = " + m_md5;
        ErrorReport(errStr);
        return false;
    }

    return true;
}

void FtpManager::slotDownloadTimeout()
{
    if(m_isDownloadError)
    {
        m_downloadTimeout->stop();
        return;
    }
    g_log.log(UpdateLog::FATAL, "Download file " + m_url.path() + " time: " + QString::asprintf("%1").arg(m_timeout),
              __FILE__, __LINE__);
    if(m_timeout++ == 30)
    {
        g_log.log(UpdateLog::FATAL, "Download file " + m_url.path() + " time out", __FILE__, __LINE__);
        m_downloadTimeout->stop();

        //It is emited to AutoUpdater.
        QString timeoutMsg;
        timeoutMsg.append(m_url.path());
        timeoutMsg.append(QObject::tr(" file download time out!"));
        ErrorReport(timeoutMsg);
    }
}

void FtpManager::Error(QNetworkReply::NetworkError errorCode)
{
    switch (errorCode) {
    case QNetworkReply::ContentAccessDenied:
        RetryDownload();
        return;
    default:
        break;
    }

	QString errorMsg = "Download error: code = ";
    errorMsg.append(QString::asprintf("%1").arg(errorCode));
	errorMsg.append(". error string = ");
	errorMsg.append(m_pReply->errorString());
    g_log.log(UpdateLog::FATAL, errorMsg, __FILE__, __LINE__);

    ErrorReport(m_pReply->errorString());
}

void FtpManager::RetryDownload()
{
    //Log
    QString errorMsg = QString::asprintf("Try time = %1, Download error: code = ").arg(m_retryDownloadTimes);
    errorMsg.append(QString::asprintf("%1").arg(m_pReply->error()));
    errorMsg.append(". error string = ");
    errorMsg.append(m_pReply->errorString());
    g_log.log(UpdateLog::WARN, errorMsg, __FILE__, __LINE__);

    m_retryDownloadTimes++;
    if(m_retryDownloadTimes > 3)
    {
        //download over with fail.
        ErrorReport(m_pReply->errorString());
    }

    //Download retry
    m_pReply->disconnect(this);
    get(m_downloadPath, m_localPath);
}

void FtpManager::ErrorReport(QString error)
{
    //It is emited to AutoUpdater.
    emit sigReplyError(error);
    m_isDownloadError = true;
}






