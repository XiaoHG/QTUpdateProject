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

    QNetworkReply *put(const QString &localPath, const QString &uploadPath);
    QNetworkReply *get(const QString &downloadPath, const QString &localPath);

    inline void setPort(int port) { m_url.setPort(port); }
    inline void setHost(const QString &host) { m_url.setHost(host); }
    inline void setPath(const QString &path) { m_url.setPath(path); }
    inline void setUserName(const QString &userName) { m_url.setUserName(userName); }
    inline void setPassword(const QString &password) { m_url.setPassword(password); }

public slots:
    void uploadFinished();
    void downloadFinished();
    void error(QNetworkReply::NetworkError);

signals:
    void sigDownloadUpdaterXmlOver();
    void sigDownloadVersionInfoFileOver();
    void sigSingleFileDownloadFinish(QString);
    void sigAllFileDownFinish();

public:
    static int m_downloadCount;
    static int m_finishCount;
    static QStringList m_currDownloadFileList;
    static QStringList m_finishDownloadFileList;

private:
    QUrl m_url;
    QString m_path;
    QNetworkAccessManager m_manager;

};

#endif // FTPMANAGER_H
