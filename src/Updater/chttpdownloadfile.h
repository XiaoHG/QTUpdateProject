#ifndef CHTTPDOWNLOADFILE_H
#define CHTTPDOWNLOADFILE_H

#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QSlider>

class CHttpDownLoadFile : public QWidget
{

    Q_OBJECT

public:
    CHttpDownloadFile(QString url, QString fileName,
                      QString dir, QWidget *parent = 0);

public slots:
    void replyNewDataArrived();//响应m_netReply有新的数据到达
    void ReplyFinished();//响应数据接收完成
    void DownLoadFile();
    void XMLParse(QString filename);
    void downloadXMLFile();
    QString getElementVersion(QString xml, QString name);
    int CheckUpdateFiles(QString name1, QString name2);
    bool CheckVersion(QString version, QString versionDownload);
    void DownLoadUpdateFiles();
    void ExitApp(QString name);

public:
   QNetworkAccessManager *m_netAccessManager;//网络参数
   QNetworkReply *m_netReply;
   QUrl m_urlAdress;//网络地址
   QString m_strFileName;//需要下载的文件名
   QString m_strDir;//文件的存储位置
   QFile *m_file;//下载的文件
   QStringList m_listFileDir; //需要更新的文件路径
   QStringList m_listFileName; //需要更新的文件

   qint64 m_nReceived;//下载文件时，已经接收的文件大小和总共大小
   qint64 m_nTotal;

   QString m_strTip;
   bool m_blsFinished;

   //更新进度条
   QSlider *m_progUpdate;
   QSlider *m_progDownload;
};

#endif // CHTTPDOWNLOADFILE_H
