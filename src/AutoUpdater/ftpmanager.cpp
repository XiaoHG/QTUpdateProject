#include "ftpmanager.h"

#include <QFile>
#include <QNetworkRequest>

int FtpManager::m_finishCount = 0;

FtpManager::FtpManager(QObject *parent) :
    QObject(parent),
    t(0)
{
    // 设置协议
    m_url.setScheme("ftp");
    setHost("192.168.4.176");
    m_downloadTimeout = new QTimer(this);
    connect(m_downloadTimeout, SIGNAL(timeout()), this, SLOT(slotDownloadTimeout()));
    m_downloadTimeout->start(1000);
}

void FtpManager::get(const QString &downloadPath, const QString &localPath)
{
    // 记录要保存的位置
    m_path = localPath;

    // 设置下载路径
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
        return;
    }

    // 写入文件
    QFile file(m_path);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can't open local file : " << m_path;
        return;
    }

    file.write(m_pReply->readAll());
    file.flush();
    file.close();

    if(m_path.contains("/download/updater.xml"))
    {
        qDebug() << "Finish download updater.xml file!";
        sigDownloadUpdaterXmlOver();
        return;
    }
    if(m_path.contains("/download/versionInfo.txt"))
    {
        qDebug() << "Finish download versionInfo.txt file!";
        sigDownloadVersionInfoFileOver();
        return;
    }
    sigDownloadFinishPerFile(m_url.path());
}

void FtpManager::error(QNetworkReply::NetworkError)
{
    qDebug() << "error = " << m_pReply->errorString();
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
        sigDownloadTimeout(m_url.path());
    }
}






