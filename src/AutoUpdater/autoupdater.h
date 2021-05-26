
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
     * @brief makeInitXML
     * @param xml
     * If local xml file is not exist, make a init xml file
     */
    void makeInitXML();

    /**
     * @brief GetVersionInfo
     * @return New version information
     */
    QString GetVersionInfo();

    /**
     * @brief GetUpdateFilesDir
     * @return All update files path
     */
    QStringList GetUpdateFilesDir();

    /**
     * @brief GetUpdateFileName
     * @return All update files
     */
    QStringList GetUpdateFilesName();

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
     * @brief GetDownloadTimeoutList
     * @return Download time out files
     * Get all download failure files that is time out download.
     */
    QStringList GetDownloadTimeoutList();

    /**
     * @brief GetNewVersion
     * @return The new version
     * Get new version string
     */
    QString GetNewVersion();

    /**
     * @brief CreateNewLink
     * Create new version application link to desktop.
     */
    void CreateNewLink();

    /**
     * @brief GetNewVersionPath
     * @return New version path
     * Get the new version download path.
     */
    QString GetNewVersionPath();

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
     * @brief MakeDeleteScript
     * Make del.bat script file for delete all
     * old version files.
     */
    QString MakeDeletePathScript(const QString saveScriptPath,
                              QString delPath,
                              const QString scriptName);

    /**
     * @brief FailDeleteNewVersionDir
     */
    void FailDeleteNewVersionDir();

    /**
     * @brief AbnormalExit
     */
    void AbnormalExit();

    /**
     * @brief StopDownload
     */
    void StopDownload();
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
    /**
     * @brief SaveLog
     */
    void SaveLog();

private:
    QStringList m_listFileDir; //The list directory for update.
    QStringList m_listFileName; //The list file for update.

    QList<FtpManager*> m_ftpList; //FTP class

    //Ftp error stack
    QStringList m_replyErrorStack;

    //Ftp download file time out list
    QStringList m_downloadTimeoutList;

    QString m_oldVersion;
    QString m_newVersion;
    QString m_newVersionPath;

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
    bool m_isDownloadInitFiles;

};

#endif // CAUTOUPDATER_H
