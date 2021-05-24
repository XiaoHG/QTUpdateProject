#include "ftpmanager.h"
#include "updatelog.h"

#include <QFile>
#include <QNetworkRequest>

extern UpdateLog g_log;

int FtpManager::m_finishCount = 0;

FtpManager::FtpManager(QObject *parent) :
    QObject(parent),
    t(0)
{
    m_url.setScheme("ftp");
    setHost("192.168.4.132");
    m_downloadTimeout = new QTimer(this);
    connect(m_downloadTimeout, SIGNAL(timeout()), this, SLOT(slotDownloadTimeout()));
    m_downloadTimeout->start(1000);
}

void FtpManager::get(const QString &downloadPath, const QString &localPath)
{
    g_log.log(UpdateLog::INFO, "Start download file: " + downloadPath, __FILE__, __LINE__);
    m_path = localPath;

    m_url.setPath(downloadPath);
    sigDownloadStartPerFile(m_url.path());

    m_pReply = m_manager.get(QNetworkRequest(m_url));
    connect(m_pReply, SIGNAL(finished()), SLOT(downloadFinished()));
    connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
}

void FtpManager::downloadFinished()
{
    m_finishCount++;
    m_downloadTimeout->stop();

    if(m_pReply->error() != QNetworkReply::NoError)
    {
        sigReplyError(m_pReply->errorString());
        qDebug() << "error = " << m_pReply->errorString();
        g_log.log(UpdateLog::FATAL, "Download error: " + m_pReply->errorString(), __FILE__, __LINE__);
        return;
    }

    // 写入文件
    QFile file(m_path);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can't open local file : " << m_path;
        g_log.log(UpdateLog::FATAL, "Can't open local file : " + m_path, __FILE__, __LINE__);
        return;
    }

    file.write(m_pReply->readAll());
    file.flush();
    file.close();

    if(m_path.contains("/download/updater.xml"))
    {
        qDebug() << "Finish download updater.xml file!";
        g_log.log(UpdateLog::INFO, "Finish download updater.xml file!", __FILE__, __LINE__);
        sigDownloadUpdaterXmlOver();
        return;
    }
    if(m_path.contains("/download/versionInfoCh.txt"))
    {
        qDebug() << "Finish download versionInfoCh.txt file!";
        g_log.log(UpdateLog::INFO, "Finish download versionInfoCh.txt file!", __FILE__, __LINE__);
        sigDownloadVersionInfoFileOver();
        return;
    }
    if(m_path.contains("/download/versionInfoEn.txt"))
    {
        qDebug() << "Finish download versionInfoEn.txt file!";
        g_log.log(UpdateLog::INFO, "Finish download versionInfoEn.txt file!", __FILE__, __LINE__);
        sigDownloadVersionInfoEnfileOver();
        return;
    }

    g_log.log(UpdateLog::INFO, QString::asprintf("Finish download %1 file!").arg(m_url.path()), __FILE__, __LINE__);
    sigDownloadFinishPerFile(m_url.path());
}

void FtpManager::error(QNetworkReply::NetworkError)
{
    qDebug() << "error = " << m_pReply->errorString();
    g_log.log(UpdateLog::WARN, "Download error: " + m_pReply->errorString(), __FILE__, __LINE__);
    sigReplyError(m_pReply->errorString());
}

int FtpManager::GetFinishCount()
{
    return m_finishCount;
}

void FtpManager::slotDownloadTimeout()
{
    qDebug () << "t = " << t;
    if(t++ == 30)
    {
        qDebug() << "Call slotDownloadTimeout: download file " << m_url.path() << " time out";
        g_log.log(UpdateLog::FATAL, "Download file " + m_url.path() + " time out", __FILE__, __LINE__);
        m_downloadTimeout->stop();
        sigDownloadTimeout(m_url.path());
    }
}






