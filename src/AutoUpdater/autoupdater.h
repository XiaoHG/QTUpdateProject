
#ifndef CAUTOUPDATER_H
#define CAUTOUPDATER_H

#include <QObject>
#include <QProcess>
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

public:
    /**
     * @brief IsUpdate
     * @return true is update, or not
     * update is.
     */
    bool IsUpdate();

    /**
     * @brief CheckUpdateFiles
     * Get All files path and name for update.
     */
    void LoadUpdateFiles();

    /**
     * @brief DownloadUpdateFiles
     * Download all update files.
     */
    void DownloadUpdateFiles();

    /**
     * @brief GetVersionInfo
     * @return New version information
     */
    QString GetVersionInfo();

    /**
     * @brief GetUpdateProcessb
     * @return Download process percentage.
     * calculate all files that need to update and current
     * finish download files.
     */
    int GetUpdateProcess();

    /**
     * @brief DownloadXMLFile
     * Download xml file that is the update control file.
     */
    void DownloadXMLFile();

    /**
     * @brief GetFtpErrorStack
     * @return The laster five error string
     *         storage in m_replyErrorStack argument.
     */
    QStringList GetFtpErrorStack();

    /**
     * @brief GetNewVersion
     * @return The new version
     * Get new version string
     */
    QString GetNewVersion();

    /**
     * @brief GetOldVersion
     * @return Old version
     */
    QString GetOldVersion();

    /**
     * @brief RestartApp
     * Update over, and restart application.
     */
    void RestartApp();

    /**
     * @brief AbnormalExit
     */
    void AbnormalExit();

    /**
     * @brief SetParentPid
     */
    void SetParentPid(QString parentPid);

private:
    /**
     * @brief makeInitXML
     * @param xml
     * If local xml file is not exist, make a init xml file
     */
    void makeInitXML();

    /**
     * @brief CreateNewLink
     * Create new version application link to desktop.
     */
    void CreateNewLink();

    /**
     * @brief MakeDeleteScript
     * Make del.bat script file for delete all
     * old version files.
     */
    void MakeDeletePathScript(const QString saveScriptPath,
                              QString delPath,
                              const QString scriptName,
                              const int delay = 0);

    /**
     * @brief FailDeleteNewVersionDir
     */
    void FailDeleteNewVersionDir();

    /**
     * @brief StopDownload
     */
    void StopDownload();

    /**
     * @brief SaveLog
     */
    void SaveLog();


protected slots:
    /**
     * @brief slotDownloadUpdaterXmlOver
     */
    void slotDownloadUpdaterXmlOver();

    /**
     * @brief slotDownloadVersionInfoFileOver
     */
    void slotDownloadVersionInfoFileOver();

    /**
     * @brief slotDownloadVersionInfoEnfileOver
     */
    void slotDownloadVersionInfoEnfileOver();

    /**
     * @brief slotReplyError
     * @param errStr error string
     * Storage download error from Ftp
     */
    void slotSaveDownloadError(QString errStr);

    /**
     * @brief slotDownloadStartPerFile
     * @param fileName
     */
    void slotDownloadStartPerFile(QString fileName);

    /**
     * @brief slotDownloadFinishPerFile
     * @param fileName
     */
    void slotDownloadFinishPerFile(QString fileName);

signals:
    /**
     * @brief sigDownloadUpdaterFileOver
     * updater.xml and versionInfo.txt download finish,
     * be emit to AutoUpdaterUI class for next step to check
     * whether update or not.
     */
    void sigDownloadInitFileOver();

    /**
     * @brief sigDownloadStartPerFile
     */
    void sigDownloadStartPerFile(QString);

    /**
     * @brief sigDownloadFinishPerFile
     */
    void sigDownloadFinishPerFile(QString);

private:
    QStringList m_listFileDir; //The list directory for update.
    QStringList m_listFileName; //The list file for update.

    QList<FtpManager*> m_ftpList; //FTP class

    //Ftp error stack
    QStringList m_replyErrorStack;

    //Version
    QString m_oldVersion;
    QString m_newVersion;
    QString m_newVersionPath;

    //The init files
    QString m_localXmlPath;
    QString m_downloadXmlPath;
    QString m_downloadVersionInfoChPath;
    QString m_downloadVersionInfoEnPath;

    //process
    QList<QProcess*> m_listProcess;

    //language
    bool m_bCh;

    //Download process
    int m_finishDownloadCount;

    //Download init files
    bool m_isCheckForUpdate;

    //parent pid
    QString m_parentPid;

};

#endif // CAUTOUPDATER_H
