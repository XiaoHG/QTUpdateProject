#include "autoupdater.h"
#include "xmlparser.h"
#include "ftpmanager.h"
#include "ftpmanager.h"
#include "filemanager.h"

#include <QDir>
#include <QDomDocument>
#include <QCoreApplication>
#include <QProcess>
#include <QMessageBox>
#include <QThread>
#include <QApplication>
#include <QStringLiteral>
#include <QStandardPaths>
#include <QFile>
#include <QProcess>
#include <QThread>

static const QString VERSION_PATH = "/version";
static const QString APPLICATION_NAME = "AutoUpdateTest";
static const QString DONWLOAD_PATH = "../";

AutoUpdater::AutoUpdater()
{
    m_localXmlPath = QApplication::applicationDirPath() + "/updater.xml";
    m_downloadXmlPath = QApplication::applicationDirPath() + "/download/updater.xml";
    m_downloadVersionInfoPath = QApplication::applicationDirPath() + "/download/versionInfo.txt";
}

AutoUpdater::~AutoUpdater()
{
    for(int i = 0; i < m_ftpList.size(); ++i)
    {
        m_ftpList.at(i)->deleteLater();
    }
    for(int i = 0; i < m_listProcess.size(); ++i)
    {
        m_listProcess.at(i)->deleteLater();
    }
}

void AutoUpdater::DownloadXMLFile()
{
    QString downloadXmlPath = QApplication::applicationDirPath() + "/download";
    QDir downloadDir(downloadXmlPath);
    if(!downloadDir.exists())
        downloadDir.mkdir(downloadXmlPath);

    FtpManager *ftp = new FtpManager();
    m_ftpList.push_back(ftp);
    ftp->get(VERSION_PATH + "/updater.xml", m_downloadXmlPath);
    connect(ftp, SIGNAL(sigDownloadUpdaterXmlOver()), this, SLOT(slotDownloadUpdaterXmlOver()));
    connect(ftp, SIGNAL(sigReplyError(QString)), this, SLOT(slotStorageDownloadError(QString)));
}

void AutoUpdater::slotDownloadUpdaterXmlOver()
{
    FtpManager *ftp = new FtpManager();
    m_ftpList.push_back(ftp);
    ftp->get(VERSION_PATH + "/versionInfo.txt", m_downloadVersionInfoPath);
    connect(ftp, SIGNAL(sigDownloadVersionInfoFileOver()), this, SLOT(slotDownloadVersionInfoFileOver()));
}

void AutoUpdater::slotDownloadVersionInfoFileOver()
{
    sigDownloadInitFileOver();
}

QString AutoUpdater::GetVersionInfo()
{
    QString strVersionInfo;
    QFile file(QApplication::applicationDirPath() + "/download/versionInfo.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "can't open file: " << QApplication::applicationDirPath() + "/download/versionInfo.txt";
        return Q_NULLPTR;
    }

    QTextStream in(&file);
    strVersionInfo = in.readAll();
    file.close();

    return strVersionInfo;
}

bool AutoUpdater::IsUpdate()
{
    QString localXML = QApplication::applicationDirPath() + "/updater.xml";
    QString downloadXML = QApplication::applicationDirPath() + "/download/updater.xml";

    QFile file(localXML);
    if(!file.exists())
    {
        makeInitXML();
    }

    QDomNodeList nodeNewList;
    nodeNewList = XMLParser::XMLParseElement(downloadXML, "version");
    QString newVersion = nodeNewList.at(0).toElement().text();
    m_newVersion = newVersion;
    m_newVersionPath = QApplication::applicationDirPath() + "/" +
                        DONWLOAD_PATH + APPLICATION_NAME + m_newVersion;

    QDomNodeList nodeOldList;
    nodeOldList = XMLParser::XMLParseElement(localXML, "version");
    QString oldVersion = nodeOldList.at(0).toElement().text();
    m_oldVersion = oldVersion;

    QStringList newVersionList = newVersion.split('.');
    QStringList oldVersionList = oldVersion.split('.');
    for(int i = 0; i < newVersionList.size(); ++i)
    {
        qDebug() << newVersionList.at(i);
        qDebug() << oldVersionList.at(i);
        if(i >= oldVersionList.size() || newVersionList.at(i) > oldVersionList.at(i))
        {
            qDebug() << QStringLiteral("Server version is updater, need to update!");
            return true;
        }
    }
    qDebug() << QStringLiteral("Local Version is the laster version, it is not need to update!");
    return false;
}

void AutoUpdater::makeInitXML()
{
    QString xml = QApplication::applicationDirPath() + "/updater.xml";
    QFile file(xml);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "makeXML false, can not open " << xml << " file!";
        return;
    }
    QString str = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<autoupdate>\n"
                  "    <version>V0.0</version>\n"
                  "    <filelist>\n"
                  "    </filelist>\n"
                  "</autoupdate>";
    QTextStream txtStream(&file);
    txtStream << str;
    file.close();
}

QString AutoUpdater::GetOldVersion()
{
    return m_oldVersion;
}

QString AutoUpdater::GetNewVersion()
{
    return m_newVersion;
}

QString AutoUpdater::GetNewVersionPath()
{
    return m_newVersionPath;
}

void AutoUpdater::LoadUpdateFiles()
{
    m_listFileDir.clear();
    m_listFileName.clear();

    QString xmlPath = QApplication::applicationDirPath() + "/download/updater.xml";
    if(!QFile::exists(xmlPath))
    {
        qDebug() << QStringLiteral("Download xml is not exist!");
        return;
    }

    QDomNodeList nodeList = XMLParser::XMLParseElement(xmlPath, "file");
    for(int i = 0; i < nodeList.size(); ++i)
    {
        QString name = nodeList.at(i).toElement().attribute("name");
        QString dir = nodeList.at(i).toElement().attribute("dir");

        m_listFileDir.append(dir);
        m_listFileName.append(name);
    }
}

QStringList AutoUpdater::GetUpdateFilesDir()
{
    return m_listFileDir;
}

QStringList AutoUpdater::GetUpdateFilesName()
{
    return m_listFileName;
}

void AutoUpdater::DownloadUpdateFiles()
{
    if(m_listFileDir.isEmpty() || m_listFileName.isEmpty())
    {
        qDebug() << QStringLiteral("It is not file thit neet to update!");
        return;
    }

    qDebug() << "Start download...";

    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        qDebug() << QStringLiteral("download ...") + m_listFileName.at(i);

        /** Download path **/
        QString localFileDir = m_newVersionPath + m_listFileDir.at(i);
        QDir directory(localFileDir);
        if(!directory.exists())
        {
            directory.mkpath(localFileDir);
        }

        QString strFileDirServer;
        if(m_listFileDir.at(i) == "")
            strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME +
                    m_newVersion + "/" + m_listFileName.at(i);
        else
            strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME + m_newVersion +
                                        m_listFileDir.at(i) + "/" + m_listFileName.at(i);

        localFileDir += "/" + m_listFileName.at(i);
        FtpManager *ftp = new FtpManager();
        m_ftpList.push_back(ftp);
        ftp->get(strFileDirServer, localFileDir);
        connect(ftp, SIGNAL(sigDownloadTimeout(QString)), this, SLOT(slotDownloadTimeout(QString)));
    }
}

int AutoUpdater::GetUpdateProcess()
{
    // - 2: updater.xml and versionInfo.txt
    //FtpManager::GetFinishCount() =  47
    //m_listFileName.size() =  46
    qDebug() <<"FtpManager::GetFinishCount() = " << FtpManager::GetFinishCount();
    qDebug() << "m_listFileName.size() = " << m_listFileName.size();
    return (FtpManager::GetFinishCount() - 2) * 100 / m_listFileName.size();
}

void AutoUpdater::slotDownloadTimeout(QString fileName)
{
    m_downloadTimeoutList.append(fileName);

    //Stop all current download
    for(int i = 0; i < m_ftpList.size(); i++)
    {
        if(m_ftpList.at(i))
            m_ftpList.at(i)->deleteLater();
    }

    //Delete All already download files
    QString scriptName = "timeoutDel.bat";
    QString scriptPath = QApplication::applicationDirPath();
    MakeDeletePathScript(scriptPath, m_newVersionPath, scriptName);

    QString delScript = scriptPath + "/" + scriptName;
    qDebug() << "scriptPath = " << delScript;
    QFileInfo scriptInfo(delScript);
    if(!scriptInfo.exists())
    {
        qDebug() << "The time out delete script is not exist!";
    }
    else
    {
        //Execute delete.
        QProcess *p = new QProcess(this);
        p->start(delScript);
        m_listProcess.push_back(p);
    }

    sigDownloadTimeout();
}

QStringList AutoUpdater::GetDownloadTimeoutList()
{
    return m_downloadTimeoutList;
}

void AutoUpdater::slotStorageDownloadError(QString errStr)
{
    if(m_replyErrorStack.size() >= 5)
        m_replyErrorStack.pop_back();
    m_replyErrorStack.push_front(errStr);
}

QStringList AutoUpdater::GetFtpErrorStack()
{
    return m_replyErrorStack;
}

QStringList AutoUpdater::GetCurDownloadFileList()
{
    return FtpManager::GetCurDownloadFileList();
}

QStringList AutoUpdater::GetFinishDownloadFileList()
{
    return FtpManager::GetFinishDownloadFileList();
}

void AutoUpdater::RestartApp()
{
    QString delScriptName = "del.bat";
    MakeDeletePathScript(QApplication::applicationDirPath(),
                         QApplication::applicationDirPath(),
                         delScriptName);
    CreateNewLink();

    //Start new version application.
    QString newApp = m_newVersionPath + "/" + APPLICATION_NAME + m_newVersion + ".exe";
    qDebug() << "newApp = " << newApp;
    QProcess::startDetached(newApp);

    //Execute delete script file
    QProcess *p = new QProcess(this);
    p->start(QApplication::applicationDirPath() + "/" + delScriptName);
    m_listProcess.push_back(p);

    exit(0);
}

void AutoUpdater::MakeDeletePathScript(const QString saveScriptPath, QString delPath,
                                       const QString scriptName)
{
    delPath = delPath.replace(QRegExp("\\/"), "\\\\");
    QString content = "ping -n 1 127.0.0.1>nul\n"
                      "@echo off\n"
                      "rd /s/q " + delPath;

    //The delete script file storage in the new version path
    QFile script(saveScriptPath + "/" + scriptName);
    if(!script.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream in(&script);
    in << content;
    script.close();
}

void AutoUpdater::CreateNewLink()
{
    QString desktopLink;
    desktopLink.append(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    desktopLink.append("/");
    desktopLink.append(APPLICATION_NAME);
    desktopLink.append(m_newVersion + ".lnk");

    QString newAppPath = m_newVersionPath + "/" + APPLICATION_NAME + m_newVersion + ".exe";
    qDebug() << "newAppPath = " << newAppPath;
    qDebug() << "desktopLink = " << desktopLink;
    QFile::link(newAppPath, desktopLink);
}

