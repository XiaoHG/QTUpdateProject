#ifndef CHTTPDOWNLOADFILE_H
#define CHTTPDOWNLOADFILE_H

#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QSlider>
#include <QNetworkReply>
#include <QString>
#include <QObject>

class CHttpDownloadFile : public QObject
{
    Q_OBJECT

public:
    CHttpDownloadFile(QString url, QString fileName,
                      QString dir, QObject *parent = 0);

    void DownloadFile();


public slots:
    void slotReplyNewDataArrived();//响应m_netReply有新的数据到达
    void slotReplyFinished();//响应数据接收完成
    void slotReplyError(QNetworkReply::NetworkError);//响应接收错误
    void slotReplyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);//提示文件下载进度

signals:
    void sigDownloadFinishedSignal();

public:
   QNetworkAccessManager *m_netAccessManager;//网络参数
   QNetworkReply *m_netReply;
   QUrl m_urlAddress;//网络地址
   QString m_strFileName;//需要下载的文件名
   QString m_strDir;//文件的存储位置
   QFile *m_file;//下载的文件

   qint64 m_nReceived;//下载文件时，已经接收的文件大小和总共大小
   qint64 m_nTotal;
   bool m_blsFinished;//下载完成与否标记
};

#endif // CHTTPDOWNLOADFILE_H
