#include "ftpmanager.h"

#include <QFile>
#include <QNetworkRequest>

FtpManager::FtpManager(QObject *parent) :
    QObject(parent)
{
    // 设置协议
    url.setScheme("ftp");
}

QNetworkReply *FtpManager::put(const QString &localPath, const QString &uploadPath)
{
    // 文件是否可以打开
    QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly))
        return NULL;

    // 设置上传路径
    url.setPath(uploadPath);

    QNetworkReply *pReply = manager.put(QNetworkRequest(url), file.readAll());
    connect(pReply, SIGNAL(finished()), SLOT(uploadFinished()));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    return pReply;
}

QNetworkReply *FtpManager::get(const QString &downloadPath, const QString &localPath)
{
    // 记录要保存的位置
    path = localPath;

    // 设置下载路径
    url.setPath(downloadPath);

    qDebug() << "downloadPath = " << downloadPath;

    QNetworkReply *pReply = manager.get(QNetworkRequest(url));
    connect(pReply, SIGNAL(finished()), SLOT(downloadFinished()));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    return pReply;
}

void FtpManager::uploadFinished()
{

}

void FtpManager::downloadFinished()
{
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    if (pReply == NULL)
        return;

    // 如果有错误就不需要保存
    if (pReply->error() != QNetworkReply::NoError)
        return;

    // 写入文件
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
        return;


    file.write(pReply->readAll());
    file.flush();
    file.close();
    qDebug() << "url.path = " << url.path();
    if(path.contains("updater.xml"))
    {
        sigDownloadUpdaterXmlOver();
        qDebug() << "file: " << path << " download success!";
        return;
    }
    if(path.contains("versionInfo.txt"))
    {
        sigDownloadVersionInfoFileOver();
        qDebug() << "file: " << path << " download success!";
        return;
    }
}

void FtpManager::error(QNetworkReply::NetworkError)
{
    qDebug() << "error = ";
}
