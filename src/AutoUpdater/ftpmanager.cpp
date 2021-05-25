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
    m_downloadTimeout->stop();

    //Do nothing at any error.
    if(m_pReply->error() != QNetworkReply::NoError)
    {
        return;
    }

    //Finish count.
    m_finishCount++;

    //Write to file.
    QFile file(m_path);
    if (!file.open(QIODevice::WriteOnly))
    {
        g_log.log(UpdateLog::FATAL, "Can't open local file : " + m_path, __FILE__, __LINE__);
        return;
    }

    file.write(m_pReply->readAll());
    file.flush();
    file.close();

    if(m_path.contains("/download/updater.xml"))
    {
        g_log.log(UpdateLog::INFO, "Finish download updater.xml file!", __FILE__, __LINE__);
        sigDownloadUpdaterXmlOver();
        return;
    }
    if(m_path.contains("/download/versionInfoCh.txt"))
    {
        g_log.log(UpdateLog::INFO, "Finish download versionInfoCh.txt file!", __FILE__, __LINE__);
        sigDownloadVersionInfoFileOver();
        return;
    }
    if(m_path.contains("/download/versionInfoEn.txt"))
    {
        g_log.log(UpdateLog::INFO, "Finish download versionInfoEn.txt file!", __FILE__, __LINE__);
        sigDownloadVersionInfoEnfileOver();
        return;
    }

    g_log.log(UpdateLog::INFO, QString::asprintf("Finish download %1 file!").arg(m_url.path()), __FILE__, __LINE__);
    sigDownloadFinishPerFile(m_url.path());
}

void FtpManager::error(QNetworkReply::NetworkError)
{
    g_log.log(UpdateLog::WARN, "Download error: " + m_pReply->errorString(), __FILE__, __LINE__);

    //It is emited to AutoUpdater.
    sigReplyError(m_pReply->errorString());
}

int FtpManager::GetFinishCount()
{
    return m_finishCount;
}

void FtpManager::slotDownloadTimeout()
{
    if(t++ == 30)
    {
        g_log.log(UpdateLog::FATAL, "Download file " + m_url.path() + " time out", __FILE__, __LINE__);
        m_downloadTimeout->stop();

        //It is emited to AutoUpdater.
        sigDownloadTimeout(m_url.path());
    }
}






