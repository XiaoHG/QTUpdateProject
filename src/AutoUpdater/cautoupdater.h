
#ifndef CAUTOUPDATER_H
#define CAUTOUPDATER_H

#include <QObject>
#include <QSlider>

class CAutoUpdater : public QObject
{
    Q_OBJECT

public:
    CAutoUpdater();
    ~CAutoUpdater();

public:
    void DownloadXMLFile();
    void DownloadUpdateFiles();
    int CheckUpdateFiles(QString xml1, QString xml2);
    bool CheckVersion(QString version, QString versionDownload);
    bool CheckVersionForUpdate();
    QString GetElementVersion(QString xml, QString name);
    QStringList GetVersionInfo(QString txt = "");
    QString GetVersion(QString xml);
    QStringList GetUpdateFileDir();
    QStringList GetUpdateFileName();
    int GetDownProcess();
    int GetUpdateProcess();

protected slots:
    void slotReplyHttpFinished();

private:
    QStringList m_listFileDir; //需要更新的文件路径
    QStringList m_listFileName; //需要更新的文件
    QString m_strTip;//下载过程提示信息

    //更新进度条
    int m_progUpdate;
    int m_progDownload;

    //拷贝结束的条件,同时也是全部更新完成的标志
    bool m_bCopyOver;

};

#endif // CAUTOUPDATER_H
