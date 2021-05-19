
#ifndef CAUTOUPDATER_H
#define CAUTOUPDATER_H

#include <QObject>
#include <QSlider>

class FtpManager;

class AutoUpdater : public QObject
{
    Q_OBJECT

public:
    AutoUpdater();
    ~AutoUpdater();

public:
    void DownloadUpdateFiles();
    bool CheckUpdateFiles(QString xml1, QString xml2);
    bool CheckVersion(QString version, QString versionDownload);
    bool CheckVersionForUpdate();
    bool CheckXML(QString xml);
    void makeInitXML(QString xml);
    QString GetElementVersion(QString xml, QString name);
    QString GetVersion(QString xml);
    QString GetVersionInfo();
    QStringList GetUpdateFileDir();
    QStringList GetUpdateFileName();
    int GetUpdateProcess();

    QStringList GetCurrDownloadFileList();
    QStringList GetFinishDownloadFileList();
    void DownloadXMLFile();

    QStringList GetFtpErrorStack();
protected slots:
    void slotDownloadUpdaterXmlOver();
    void slotDownloadVersionInfoFileOver();

    void slotReplyError(QString errStr);
signals:
    void sigDownloadUpdaterFileOver();

private:
    QStringList m_listFileDir; //需要更新的文件路径
    QStringList m_listFileName; //需要更新的文件
    QString m_strTip;//下载过程提示信息

    QList<FtpManager*> m_ftpList;

    //保存单个文件列表的本地拷贝路径
    QStringList m_strPlaceDirList;

    //Ftp error stack
    QStringList m_replyErrorStack;
};

#endif // CAUTOUPDATER_H
