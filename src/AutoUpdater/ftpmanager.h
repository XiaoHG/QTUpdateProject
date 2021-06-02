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
    /**
     * @brief The ERR_CODE enum
     * It is explient that ftp download error type.
     */
    enum ERR_CODE{
        MDERROR,
        TIMEOUT
    };

public:
    explicit FtpManager(QString md5 = "0", QObject *parent = 0);
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
     * @brief retryDownload
     * It is will retry 3th.
     */
    void retryDownload();

    /**
     * @brief errorReport
     * It is mean download error of the result.
     */
    void reportError(QString on_reply_error);

    /**
     * @brief checkMd5
     * @return
     * Check download file.
     */
    bool isMatchMd5();

protected slots:
    /**
     * @brief on_reply_downloadFinish
     * Download finish.
     */
    void on_reply_downloadFinish();

    /**
     * @brief on_timer_downloading
     * 30s time out
     */
    void on_timer_downloading();

    /**
     * @brief on_reply_error
     * Download error
     */
    void on_reply_error(QNetworkReply::NetworkError errorCode);

signals:
    /**
     * @brief signal_downloadXmlFinish
     * Download updater.xml file finish.
     * It is emited that updater.xml file download finish for AutoUpdater class.
     */
    void signal_downloadXmlFinish();

    /**
     * @brief signal_downloadChFinish
     * Download versionInfo.txt file finish.
     * It is emited that versionInfo.txt file download finish for AutoUpdater class.
     */
    void signal_downloadChFinish();

    /**
     * @brief signal_downloadEnFinish
     */
    void signal_downloadEnFinish();

    /**
     * @brief signal_startDownloadPerFile
     * start download.
     */
    void signal_startDownloadPerFile(QString);

    /**
     * @brief signal_finishDownloadPerFile
     * finish download.
     */
    void signal_finishDownloadPerFile(QString);

    /**
     * @brief signal_replyError
     * Report an error to AutoUpdater class.
     * Per error emit.
     */
    void signal_replyError(QString);

private:
    //Download parameter
    QUrl                    m_url;
    QString                 m_strLocalPath;
    QString                 m_strDownloadPath;
    QNetworkReply           *m_pReply;
    QNetworkAccessManager   m_networkManager;

    //Download time out
    int     m_iDownloadTimeSec;
    int     m_timeout;
    QTimer  *m_timerDownloading;


    //error if
    static bool m_bDownloadError;

    //retry 3th
    int     m_iRetryDownloadTimes;

    //download file md5
    QString m_strMd5;

};

#endif // FTPMANAGER_H
