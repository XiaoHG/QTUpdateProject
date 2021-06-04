
#ifndef CAUTOUPDATER_H
#define CAUTOUPDATER_H

#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QSlider>
#include <ftpmanager.h>

/**
 * @brief The AutoUpdater class
 * Update manager.
 */
class AutoUpdater : public QObject
{
    Q_OBJECT

public:
    AutoUpdater(bool bCh);
    ~AutoUpdater();

//    Q_PROPERTY(QStringList m_replyErrorStack READ getReplyErrorStack WRITE setReplyErrorStack NOTIFY slotReplyErrorStackChanged)

//    QStringList getReplyErrorStack(){ return m_replyErrorStack; }
//    void setReplyErrorStack(const QString &errorStr){ m_replyErrorStack.append(errorStr); }
//protected slots:
//    void slotReplyErrorStackChanged();

    typedef enum {
        UPDATE = 1,
        NOTUPDATE = 0,
        LOCALXML_PARSE_ERR = -1,
        DOWNLOADXML_PARSE_ERR = -2,
    }UPDATER_ERROR_CODE;

public:
    /**
     * @brief isUpdate
     * @return true is update, or not
     * update is.
     */
    UPDATER_ERROR_CODE isUpdate();

    /**
     * @brief loadUpdateFiles
     * Get All files path and name for update.
     */
    void loadUpdateFiles();

    /**
     * @brief downloadUpdateFiles
     * Download all update files.
     */
    void downloadUpdateFiles();

    /**
     * @brief getVersionInfo
     * @return New version information
     */
    QString getVersionInfo(const QString &path);

    /**
     * @brief getUpdateProcess
     * @return Download process percentage.
     * calculate all files that need to update and current
     * finish download files.
     */
    int getUpdateProcess();

    /**
     * @brief downloadInitFile
     * Download xml file that is the update control file.
     */
    void downloadInitFile();

    /**
     * @brief getFtpErrorStack
     * @return The laster five error string
     *         storage in m_replyErrorStack argument.
     */
    QStringList getFtpErrorStack();

    /**
     * @brief getNewVersion
     * @return The new version
     * Get new version string
     */
    QString getNewVersion();

    /**
     * @brief getOldVersion
     * @return Old version
     */
    QString getOldVersion();

    /**
     * @brief restartApp
     * Update over, and restart application.
     */
    void restartApp();

    /**
     * @brief abnormalExit
     */
    void abnormalExit();

    /**
     * @brief setParentPid
     */
    void setParentPid(QString parentPid);

    /**
     * @brief execDeleteOldScript
     */
    void execDeleteOldScript();

private:
    /**
     * @brief makeInitXML
     * @param xml
     * If local xml file is not exist, make a init xml file
     */
    void makeInitXML();

    /**
     * @brief createNewLink
     * Create new version application link to desktop.
     */
    void createNewLink();

    /**
     * @brief makeDeletePathScript
     * Make del.bat script file for delete all
     * old version files.
     */
    void makeDeletePathScript(const QString saveScriptPath,
                              QString delPath,
                              const QString scriptName,
                              const int delay = 0);

    /**
     * @brief failDeleteNewVersionDir
     */
    void failDeleteNewVersionDir();

    /**
     * @brief stopDownload
     */
    void stopDownload();

    /**
     * @brief saveLog
     */
    void saveLog();

    /**
     * @brief saveOldVersionPara
     */
    void saveOldVersionPara();

protected slots:
    /**
     * @brief on_ftp_downloadXmlFinish
     */
    void on_ftp_downloadXmlFinish();

    /**
     * @brief on_ftp_downloadChFinish
     */
    void on_ftp_downloadChFinish();

    /**
     * @brief on_ftp_downloadEnFinish
     */
    void on_ftp_downloadEnFinish();

    /**
     * @brief on_ftp_storageDownloadErrStr
     * @param errStr error string
     * Storage download error from Ftp
     */
    void on_ftp_storageDownloadErrStr(QString errStr);

    /**
     * @brief on_ftp_startDownloadPerFile
     * @param fileName
     */
    void on_ftp_startDownloadPerFile(QString fileName);

    /**
     * @brief on_ftp_finishDownloadPerFile
     * @param fileName
     */
    void on_ftp_finishDownloadPerFile(QString fileName);

signals:
    /**
     * @brief signal_initFileDownloadFinish
     * updater.xml and versionInfo.txt download finish,
     * be emit to AutoUpdaterUI class for next step to check
     * whether update or not.
     */
    void signal_initFileDownloadFinish();

    /**
     * @brief signal_startDownloadPerFile
     */
    void signal_startDownloadPerFile(QString);

    /**
     * @brief signal_finishDownloadPerFile
     */
    void signal_finishDownloadPerFile(QString);

private:
    QStringList m_listFileDir; //The list directory for update.
    QStringList m_listFileName; //The list file for update.
    QStringList m_listFileMd5; //The file md5.

    QList<FtpManager*> m_listFtp; //FTP class

    //Ftp error stack
    QStringList m_listFtpReplyErrorStack;

    //Version
    QString m_strOldVersion;
    QString m_strNewVersion;
    QString m_strNewVersionPath;
    QString m_strOldVersionPath;

    //The init files
    QString m_strLocalXmlPath;
    QString m_strDownloadXmlPath;
    QString m_strDownloadVersionInfoChPath;
    QString m_strDownloadVersionInfoEnPath;

    //language
    bool m_bCh;

    //Download process
    int m_iFinishDownloadCount;

    //Download init files
    bool m_bCheckForUpdateError;

    //parent pid
    QString m_strParentPid;

    //updater ini file, not use
    QSettings *m_settingsUpdaterIni;
    QString m_strUpdaterIniPath;

};

#endif // CAUTOUPDATER_H
