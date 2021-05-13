#include "ftpmanager.h"

#include <QFile>
#include <QNetworkRequest>

int FtpManager::m_downloadCount = 0;
int FtpManager::m_finishCount = 0;
QStringList FtpManager::m_currDownloadFileList = {0};
QStringList FtpManager::m_finishDownloadFileList = {0};

FtpManager::FtpManager(QObject *parent) :
    QObject(parent)
{
    // 设置协议
    m_url.setScheme("ftp");
}

QNetworkReply *FtpManager::put(const QString &localPath, const QString &uploadPath)
{
    // 文件是否可以打开
    QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly))
        return NULL;

    // 设置上传路径
    m_url.setPath(uploadPath);

    QNetworkReply *pReply = m_manager.put(QNetworkRequest(m_url), file.readAll());
    connect(pReply, SIGNAL(finished()), SLOT(uploadFinished()));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    return pReply;
}

void FtpManager::uploadFinished()
{

}

QNetworkReply *FtpManager::get(const QString &downloadPath, const QString &localPath)
{
    m_downloadCount++;
    // 记录要保存的位置
    m_path = localPath;

    // 设置下载路径
    m_url.setPath(downloadPath);
    m_currDownloadFileList.push_back(m_url.path());

    QNetworkReply *pReply = m_manager.get(QNetworkRequest(m_url));
    connect(pReply, SIGNAL(finished()), SLOT(downloadFinished()));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    return pReply;
}

void FtpManager::downloadFinished()
{
    m_downloadCount--;
    m_finishCount++;
    m_currDownloadFileList.removeOne(m_url.path());
    m_finishDownloadFileList.push_back(m_url.path());

    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    if (pReply == NULL)
        return;

    // 如果有错误就不需要保存
    if (pReply->error() != QNetworkReply::NoError)
        return;

    // 写入文件
    QFile file(m_path);
    if (!file.open(QIODevice::WriteOnly))
        return;


    file.write(pReply->readAll());
    file.flush();
    file.close();
    qDebug() << "url.path = " << m_url.path();
    if(m_path.contains("updater.xml"))
    {
        sigDownloadUpdaterXmlOver();
        return;
    }
    if(m_path.contains("versionInfo.txt"))
    {
        sigDownloadVersionInfoFileOver();
        return;
    }
    qDebug() << "file: " << m_path << " download success!";
}

void FtpManager::error(QNetworkReply::NetworkError)
{
    qDebug() << "error = ";
}





