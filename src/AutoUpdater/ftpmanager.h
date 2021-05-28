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
    virtual ~FtpManager();

    /**
     * @brief get
     * @param downloadPath
     * @param localPath
     * Download file from ftp server
     */
    void get(const QString &downloadPath, const QString &localPath);

    inline void setPort(int port) { m_url.setPort(port); }
    inline void setHost(const QString &host) { m_url.setHost(host); }
    inline void setPath(const QString &path) { m_url.setPath(path); }
    inline void setUserName(const QString &userName) { m_url.setUserName(userName); }
    inline void setPassword(const QString &password) { m_url.setPassword(password); }

private:
    /**
     * @brief RetryDownload
     * It is will retry 3th.
     */
    void RetryDownload();

    /**
     * @brief ErrorReport
     * It is mean download error of the result.
     */
    void ErrorReport();

protected slots:
    /**
     * @brief downloadFinished
     * Download finish.
     */
    void downloadFinished();

    /**
     * @brief slotDownloadTimeout
     * 30s time out
     */
    void slotDownloadTimeout();

    /**
     * @brief error
     * Download error
     */
    void Error(QNetworkReply::NetworkError errorCode);

signals:
    /**
     * @brief sigDownloadUpdaterXmlOver
     * Download updater.xml file finish.
     * It is emited that updater.xml file download finish for AutoUpdater class.
     */
    void sigDownloadUpdaterXmlOver();

    /**
     * @brief sigDownloadVersionInfoFileOver
     * Download versionInfo.txt file finish.
     * It is emited that versionInfo.txt file download finish for AutoUpdater class.
     */
    void sigDownloadVersionInfoFileOver();

    /**
     * @brief sigDownloadVersionInfoEnfileOver
     */
    void sigDownloadVersionInfoEnfileOver();

    /**
     * @brief sigDownloadStartPerFile
     * start download.
     */
    void sigDownloadStartPerFile(QString);

    /**
     * @brief sigDownloadFinishPerFile
     * finish download.
     */
    void sigDownloadFinishPerFile(QString);

    /**
     * @brief sigAllFileDownFinish
     * It is finish download all file that need to update.
     * It is emited for AutoUpdater class.
     */
    void sigAllFileDownFinish();

    /**
     * @brief sigDownloadTimeout
     * It is emited if a file download timeout.
     */
    void sigDownloadTimeout(QString);

    /**
     * @brief sigReplyError
     * Report an error to AutoUpdater class.
     * Per error emit.
     */
    void sigReplyError(QString);

private:
    //Download parameter
    QUrl m_url;
    QString m_localPath;
    QString m_downloadPath;
    QNetworkReply *m_pReply;
    QNetworkAccessManager m_manager;

    //Download time out
    int m_timeout;
    QTimer *m_downloadTimeout;

    //error if
    static bool m_isDownloadError;

    //retry 3th
    int m_retryDownloadTimes;

};

#endif // FTPMANAGER_H
