#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QUrl>
#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTimer>

class FtpManager : public QObject
{
    Q_OBJECT
public:
    explicit FtpManager(QObject *parent = 0);

    QNetworkReply *get(const QString &downloadPath, const QString &localPath);

    inline void setPort(int port) { m_url.setPort(port); }
    inline void setHost(const QString &host) { m_url.setHost(host); }
    inline void setPath(const QString &path) { m_url.setPath(path); }
    inline void setUserName(const QString &userName) { m_url.setUserName(userName); }
    inline void setPassword(const QString &password) { m_url.setPassword(password); }

protected slots:
    void downloadFinished();
    void error(QNetworkReply::NetworkError);
    void slotDownloadTimeout();

signals:
    void sigDownloadUpdaterXmlOver();
    void sigDownloadVersionInfoFileOver();
    void sigSingleFileDownloadFinish(QString);
    void sigAllFileDownFinish();
    void sigReplyError(QString);
    void sigDownloadTimeout(QString);

public:
    static int m_downloadCount;
    static int m_finishCount;
    static QStringList m_currDownloadFileList;
    static QStringList m_finishDownloadFileList;

    static int GetFinishCount();
    static int GetDownloadCount();
    static QStringList GetCurDownloadFileList();
    static QStringList GetFinishDownloadFileList();

private:
    QUrl m_url;
    QString m_path;
    QNetworkAccessManager m_manager;
    QNetworkReply *m_pReply;

    //Download time out
    QTimer *m_downloadTimeout;

};

#endif // FTPMANAGER_H
