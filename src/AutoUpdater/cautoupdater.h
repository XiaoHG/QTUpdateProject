#ifndef CAUTOUPDATER_H
#define CAUTOUPDATER_H

#include <QWidget>
#include <QMainWindow>
#include <QSlider>

class CHttpDownloadFile;

class CAutoUpdater : public QMainWindow
{
    Q_OBJECT

public:
    CAutoUpdater(QWidget *parent = 0);

public:
    void downloadXMLFile();
    QString getElementVersion(QString xml, QString name);
    int CheckUpdateFiles(QString name1 = "default", QString name2 = "default");
    bool CheckVersion(QString version, QString versionDownload);
    bool CheckVersionForUpdate();
    void DownloadUpdateFiles();
    void ExitApp(QString name);
    QStringList GetVersionInfo(QString txt = "");

    QStringList GetUpdateFileDir();
    QStringList GetUpdateFileName();

protected slots:
    void slotReplyHttpFinished();

signals:
    void sigDownloadFinishedSignal();

private:
    CHttpDownloadFile *m_httpXML;
    QStringList m_listFileDir; //需要更新的文件路径
    QStringList m_listFileName; //需要更新的文件

    QString m_strTip;//下载过程提示信息

    //更新进度条
    QSlider *m_progUpdate;
    QSlider *m_progDownload;

    bool m_blsFinished;//下载完成与否标记
};

#endif // CAUTOUPDATER_H
