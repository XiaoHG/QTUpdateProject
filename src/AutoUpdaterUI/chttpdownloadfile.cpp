#include "chttpdownloadfile.h"
#include <QDir>
#include <QNetworkReply>
#include <QDomDocument>
#include <QCoreApplication>
#include <QProcess>
#include <QMessageBox>
#include <QTextCodec>

CHttpDownloadFile::CHttpDownloadFile(QString url, QString fileName,
                                     QString dir, QObject *parent)
{
    m_urlAddress = url;
    m_strFileName = fileName;
    m_strDir = dir;
    m_netAccessManager = new QNetworkAccessManager(this);
    connect(m_netAccessManager, SIGNAL(finished(QNetworkReply*)),SLOT(slotDownloadFinishReply(QNetworkReply*)));
    m_blsFinished = false;
    m_nTotal = -1;
    this->setParent(parent);
}

void CHttpDownloadFile::slotDownloadFinishReply(QNetworkReply*)
{
    qDebug() << "slotDownloadFinishReply Finish!";
}

/**下载过程有数据到达，则往指定的下载保存文件写入**/
void CHttpDownloadFile::slotReplyNewDataArrived()
{
    if(m_file)
    {
        //这里，为了能显示下载的中文，我们使用了QTextCodec 类对象，应用utf8编码。
        //QTextCodec *codec = QTextCodec::codecForName(“utf8″);
        //QString all = codec->toUnicode(m_netReply->readAll());
        if(m_file->write(m_netReply->readAll()) == -1)
        {
            qDebug() << "文件写入错误";
            return;
        }
        m_file->flush();
    }
    else
    {
        qDebug() << m_netReply->readAll();
    }
}

/**文件下载完成，释放资源**/
void CHttpDownloadFile::slotReplyFinished()
{
    m_blsFinished = true;
    m_netAccessManager->deleteLater();
    m_netReply->deleteLater();
    m_file->close();
    m_file->deleteLater();
    qDebug() << m_strFileName << QStringLiteral(" 下载完成!");
}

/**下载过程中出现错误处理**/
void CHttpDownloadFile::slotReplyError(QNetworkReply::NetworkError)
{
    qDebug() << "slotReplyError ...";
    QNetworkReply::NetworkError error = m_netReply->error();
    switch (error) {
    case QNetworkReply::ConnectionRefusedError:
        qDebug() << QStringLiteral("远程服务器拒绝连接");
        break;
    case QNetworkReply::HostNotFoundError:
        qDebug() << QStringLiteral("远程主机名未找到（无效主机名）");
        break;
    case QNetworkReply::TooManyRedirectsError:
        qDebug() << QStringLiteral("请求超过了设定的最大重定向次数");
        break;
    default:
        qDebug() << QStringLiteral("未知错误") << error;
    }
}

/**下载文件进度提示**/
void CHttpDownloadFile::slotReplyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_nReceived = bytesReceived;
    m_nTotal = bytesTotal;
    qDebug() << "slotReplyDownloadProgress ..." <<
                "Received: " << m_nReceived <<
                " m_nTotal: " << m_nTotal;
}

/**下载文件服务器上的文件，并保存到指定目录下**/
void CHttpDownloadFile::DownloadFile()
{
    qDebug() << QStringLiteral("DownloadFile 下载地址 = ") << m_urlAddress;
    m_url = QUrl(m_urlAddress);
    m_netReply = m_netAccessManager->get(QNetworkRequest(m_url));
    //当有新数据到达时就会触发此信号
    connect(m_netReply, SIGNAL(readyRead()), this, SLOT(slotReplyNewDataArrived()));
    //完成数据接收后发送此信号
    connect(m_netReply, SIGNAL(finished()), this, SLOT(slotReplyFinished()));
    //出现错误时发送此信号;
    connect(m_netReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(slotReplyError(QNetworkReply::NetworkError)));
    //用来提示文件下载进度
    connect(m_netReply, SIGNAL(downloadProgress(qint64,qint64)),this,
            SLOT(slotReplyDownloadProgress(qint64, qint64)));

    /***存储文件的检测及使用***/
    m_blsFinished = false;
    if(m_strFileName.isEmpty()) //文件名
    {
        QFile fileInfo(m_urlAddress.path());
        m_strFileName = fileInfo.fileName();
    }

    if(!m_strDir.isEmpty()) //文件夹
    {
        QDir directory(m_strDir);
        if(!directory.exists()) //没有此文件夹则创建
        {
            qDebug() << "m_strDir1 = " << m_strDir;
            directory.mkpath(m_strDir);
        }
        qDebug() << "m_strDir2 = " << m_strDir;
        qDebug() << "add m_strDir before: m_strFileName = " << m_strFileName;
        m_strFileName = m_strDir + "/" + m_strFileName;
    }

    if(QFile::exists(m_strFileName))//如果文件存在，那么删除
    {
        QFile::remove(m_strFileName);
    }

    qDebug() << "add m_strDir after: m_strFileName = " << m_strFileName;
    m_file = new QFile(m_strFileName);
    if(!m_file->open(QIODevice::WriteOnly))
    {
        qDebug()<<"can not store file：" << m_strFileName;
        delete m_file;
        m_file = NULL;
        return;
    }
}

void CHttpDownloadFile::FtpDownloadFile()
{
    qDebug() << QStringLiteral("FtpDownloadFile 下载地址 = ") << m_urlAddress;
    m_netAccessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    m_netReply = m_netAccessManager->get(QNetworkRequest(m_urlAddress));
    connect(m_netReply, SIGNAL(readyRead()), this, SLOT(slotReplyNewDataArrived()));
    connect(m_netReply, SIGNAL(finished()), this, SLOT(slotReplyFinished()));
    connect(m_netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotReplyError(QNetworkReply::NetworkError)));
    connect(m_netReply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(slotReplyDownloadProgress(qint64 ,qint64)));

    /***存储文件的检测及使用***/
    m_blsFinished = false;
    if(m_strFileName.isEmpty()) //文件名
    {
        //m_urlAddress = http://localhost/updateClientVersion/updater.xml
        //m_strFileName = /updateClientVersion/updater.xml
        QFile fileInfo(m_urlAddress.path());
        m_strFileName = fileInfo.fileName();
    }

    if(!m_strDir.isEmpty()) //文件夹
    {
        QDir directory(m_strDir);
        if(!directory.exists()) //没有此文件夹则创建
        {
            qDebug() << "m_strDir1 = " << m_strDir;
            directory.mkpath(m_strDir);
        }
        qDebug() << "m_strDir2 = " << m_strDir;
        qDebug() << "add m_strDir before: m_strFileName = " << m_strFileName;
        m_strFileName = m_strDir + "/" + m_strFileName;
    }

    if(QFile::exists(m_strFileName))//如果文件存在，那么删除
    {
        QFile::remove(m_strFileName);
    }

    qDebug() << "add m_strDir after: m_strFileName = " << m_strFileName;
    m_file = new QFile(m_strFileName);
    if(!m_file->open(QIODevice::WriteOnly))
    {
        qDebug()<<"can not store file：" << m_strFileName;
        delete m_file;
        m_file = NULL;
        return;
    }
}

bool CHttpDownloadFile::GetBlsFinish()
{
    return m_blsFinished;
}

qint64 CHttpDownloadFile::GetReceiving()
{
    return m_nReceived;
}

qint64 CHttpDownloadFile::GetTotalReceive()
{
    return m_nTotal;
}

