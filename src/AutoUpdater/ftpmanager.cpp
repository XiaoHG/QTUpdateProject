#include "ftpmanager.h"

#include <QFile>
#include <QNetworkRequest>

int FtpManager::m_downloadCount = 0;
int FtpManager::m_finishCount = 0;
QStringList FtpManager::m_currDownloadFileList = {Q_NULLPTR};
QStringList FtpManager::m_finishDownloadFileList = {Q_NULLPTR};

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

QNetworkReply *FtpManager::get(const QString &downloadPath, const QString &localPath)
{
    m_downloadCount++;
    // 记录要保存的位置
    m_path = localPath;

    // 设置下载路径
    m_url.setPath(downloadPath);
    m_currDownloadFileList.push_back(m_url.path());

    m_pReply = m_manager.get(QNetworkRequest(m_url));
    connect(m_pReply, SIGNAL(finished()), SLOT(downloadFinished()));
    connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    return m_pReply;
}

void FtpManager::downloadFinished()
{
    m_downloadCount--;
    m_finishCount++;
    m_currDownloadFileList.removeOne(m_url.path());
    m_finishDownloadFileList.push_back(m_url.path());
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
    }
    if(m_path.contains("/download/versionInfo.txt"))
    {
        qDebug() << "Finish download versionInfo.txt file!";
        sigDownloadVersionInfoFileOver();
    }
}

void FtpManager::error(QNetworkReply::NetworkError)
{
    qDebug() << "error = " << m_pReply->errorString();
}

int FtpManager::GetFinishCount()
{
    return m_finishCount;
}

int FtpManager::GetDownloadCount()
{
    return m_downloadCount;
}

QStringList FtpManager::GetCurDownloadFileList()
{
    return m_currDownloadFileList;
}

QStringList FtpManager::GetFinishDownloadFileList()
{
    return m_finishDownloadFileList;
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






