
#include "autoupdater.h"
#include "xmlparser.h"
#include "ftpmanager.h"
#include "ftpmanager.h"
#include "filemanager.h"
#include "updatelog.h"

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

extern UpdateLog g_log;

#ifdef Q_OS_MAC
    const QString UPDATE_SYSTEM = "mac";
#endif

#ifdef Q_OS_LINUX
// linux
#endif

#ifdef Q_OS_WIN32
    const QString UPDATE_SYSTEM = "win32";
#endif

//#ifdef Q_OS_WIN64
//    const QString UPDATE_SYSTEM = "win64";
//#endif

const QString VERSION_PATH = "/version";
const QString APPLICATION_NAME = "AutoUpdateTest";
const QString DONWLOAD_PATH = "../";

AutoUpdater::AutoUpdater(bool bCh)
    :m_bCh(bCh)
{
    //The updater.xml file that in the local version path
    m_localXmlPath = QApplication::applicationDirPath() + "/updater.xml";
    //The updater.xml file that download from server.
    m_downloadXmlPath = QApplication::applicationDirPath() + "/download/updater.xml";
    //Chinese language of version information file.
    m_downloadVersionInfoChPath = QApplication::applicationDirPath() + "/download/versionInfoCh.txt";
    //English language of version information file.
    m_downloadVersionInfoEnPath = QApplication::applicationDirPath() + "/download/versionInfoEn.txt";
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
    ftp->get(VERSION_PATH + "/versionInfoCh.txt", m_downloadVersionInfoChPath);
    connect(ftp, SIGNAL(sigDownloadVersionInfoFileOver()), this, SLOT(slotDownloadVersionInfoFileOver()));
    connect(ftp, SIGNAL(sigReplyError(QString)), this, SLOT(slotStorageDownloadError(QString)));
}

void AutoUpdater::slotDownloadVersionInfoFileOver()
{
    FtpManager *ftp = new FtpManager();
    m_ftpList.push_back(ftp);
    ftp->get(VERSION_PATH + "/versionInfoEn.txt", m_downloadVersionInfoEnPath);
    connect(ftp, SIGNAL(sigDownloadVersionInfoEnfileOver()), this, SLOT(slotDownloadVersionInfoEnfileOver()));
    connect(ftp, SIGNAL(sigReplyError(QString)), this, SLOT(slotStorageDownloadError(QString)));
}

void AutoUpdater::slotDownloadVersionInfoEnfileOver()
{
    //It is emited to AutoUpdaterUI class for a information that init file download
    //over, and can do next step that check version for update.
    sigDownloadInitFileOver();
}

QString AutoUpdater::GetVersionInfo()
{
    QString versionInfo;
    QString versionInfoName;
    //Chinese or English from parent process language is.
    if(m_bCh)
    {
        versionInfoName = "versionInfoCh.txt";
    }
    else
    {
        versionInfoName = "versionInfoEn.txt";
    }

    //Version information file that stroge at download directory from ftp server.
    QFile file(QApplication::applicationDirPath() + "/download/" + versionInfoName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        g_log.log(UpdateLog::FATAL, "Can't open file: " + QApplication::applicationDirPath() + "/download/" + versionInfoName,
                  __FILE__, __LINE__);
        return Q_NULLPTR;
    }

    QTextStream in(&file);
    versionInfo = in.readAll();
    file.close();

    return versionInfo;
}

bool AutoUpdater::IsUpdate()
{
    //Compare two files of local updater.xml and download from ftp server.
    QString localXML = QApplication::applicationDirPath() + "/updater.xml";
    QString downloadXML = QApplication::applicationDirPath() + "/download/updater.xml";

    g_log.log(UpdateLog::INFO, "Local xml: " + localXML, __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, "Download xml: " + downloadXML, __FILE__, __LINE__);

    QFile file(localXML);
    if(!file.exists())
    {
        g_log.log(UpdateLog::FATAL, "Local xml is lost, make local xml file on init.", __FILE__, __LINE__);
        makeInitXML();
    }

    //Get the version string from down updater xml file.
    QDomNodeList nodeNewList;
    nodeNewList = XMLParser::XMLParseElement(downloadXML, "version");
    QString newVersion = nodeNewList.at(0).toElement().text();
    m_newVersion = newVersion;
    m_newVersionPath = QApplication::applicationDirPath() + "/" +
                        DONWLOAD_PATH + APPLICATION_NAME + m_newVersion;

    //Get the version string from local updater xml file.
    QDomNodeList nodeOldList;
    nodeOldList = XMLParser::XMLParseElement(localXML, "version");
    QString oldVersion = nodeOldList.at(0).toElement().text();
    m_oldVersion = oldVersion;

    g_log.log(UpdateLog::INFO, QString::asprintf("Old version: %1, New version %2").arg(oldVersion).arg(newVersion),
              __FILE__, __LINE__);

    //Compare.
    QStringList newVersionList = newVersion.split('.');
    QStringList oldVersionList = oldVersion.split('.');
    for(int i = 0; i < newVersionList.size(); ++i)
    {
        if(i >= oldVersionList.size() || newVersionList.at(i) > oldVersionList.at(i))
        {
            g_log.log(UpdateLog::INFO, "Server version is updater, need to update!", __FILE__, __LINE__);
            return true;
        }
    }
    g_log.log(UpdateLog::INFO, "Local Version is the laster version, it is not need to update!", __FILE__, __LINE__);
    return false;
}

void AutoUpdater::makeInitXML()
{
    //Made in application path.
    QString xml = QApplication::applicationDirPath() + "/updater.xml";
    QFile file(xml);
    if(!file.open(QIODevice::WriteOnly))
    {
        g_log.log(UpdateLog::FATAL, "Make init xml false, can not open: " + xml + " file!", __FILE__, __LINE__);
        return;
    }

    //Init content.
    QString str = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<autoupdate>\n"
                  "    <version>V0.0</version>\n"
                  "    <filelist>\n"
                  "    </filelist>\n"
                  "</autoupdate>";
    QTextStream txtStream(&file);
    txtStream << str;
    g_log.log(UpdateLog::INFO, "Local init xml file content: " + str, __FILE__, __LINE__);
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
    //Clear buffer first.
    m_listFileDir.clear();
    m_listFileName.clear();

    //Load file that download from ftp server name updater.xml.
    QString xmlPath = QApplication::applicationDirPath() + "/download/updater.xml";
    if(!QFile::exists(xmlPath))
    {
        g_log.log(UpdateLog::FATAL, "Download xml is not exist: " + xmlPath + " not exist!", __FILE__, __LINE__);
        return;
    }

    //Read xml file, and storage at the buffer m_listFileDir and m_listFileName,
    //for download use.
    QDomNodeList nodeList = XMLParser::XMLParseElement(xmlPath, "file");
    for(int i = 0; i < nodeList.size(); ++i)
    {
        QString name = nodeList.at(i).toElement().attribute("name");
        QString dir = nodeList.at(i).toElement().attribute("dir");

        QString logDir = dir.isEmpty() ? "./" : dir;
        g_log.log(UpdateLog::INFO, "Load: " + name + " file, and directory is: " + logDir, __FILE__, __LINE__);

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

int AutoUpdater::GetUpdateProcess()
{
    // -3: updater.xml/versionInfoCh.txt/versionInfoEn.txt
    g_log.log(UpdateLog::INFO, QString::asprintf("Download finish count: %1").arg(FtpManager::GetFinishCount() - 3), __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, QString::asprintf("Download total count: %1").arg(m_listFileName.size()), __FILE__, __LINE__);
    int process = (FtpManager::GetFinishCount() - 3) * 100 / m_listFileName.size();
    return process;
}

void AutoUpdater::DownloadUpdateFiles()
{
    //If download buffer is empty, do nothing.
    if(m_listFileDir.isEmpty() || m_listFileName.isEmpty())
    {
        g_log.log(UpdateLog::INFO, "It is not file thit neet to update!", __FILE__, __LINE__);
        return;
    }

    g_log.log(UpdateLog::DEBUG, "Start download ... ", __FILE__, __LINE__);

    //Download each file of ftp server.
    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        g_log.log(UpdateLog::DEBUG, "Download file: " + m_listFileName.at(i), __FILE__, __LINE__);

        //localFileDir is download storage position.
        QString localFileDir = m_newVersionPath + m_listFileDir.at(i);
        QDir directory(localFileDir);
        if(!directory.exists())
        {
            g_log.log(UpdateLog::DEBUG, "mkdir " + localFileDir, __FILE__, __LINE__);
            directory.mkpath(localFileDir);
        }
        localFileDir += "/" + m_listFileName.at(i);

        //strFileDirServer is ftp server position.
        QString strFileDirServer;
        if(m_listFileDir.at(i) == "")
            strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME + m_newVersion +
                               "/" + UPDATE_SYSTEM + "/" + m_listFileName.at(i);
        else
            strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME + m_newVersion +
                               "/" + UPDATE_SYSTEM + m_listFileDir.at(i) + "/"  + m_listFileName.at(i);

        FtpManager *ftp = new FtpManager();
        m_ftpList.push_back(ftp);
        connect(ftp, SIGNAL(sigDownloadStartPerFile(QString)), this, SLOT(slotDownloadStartPerFile(QString)));
        connect(ftp, SIGNAL(sigDownloadFinishPerFile(QString)), this, SLOT(slotDownloadFinishPerFile(QString)));
        connect(ftp, SIGNAL(sigDownloadTimeout(QString)), this, SLOT(slotDownloadTimeout(QString)));
        connect(ftp, SIGNAL(sigReplyError(QString)), this, SLOT(slotStorageDownloadError(QString)));

        ftp->get(strFileDirServer, localFileDir);
    }
}

void AutoUpdater::slotDownloadStartPerFile(QString fileName)
{
    sigDownloadStartPerFile(fileName);
}

void AutoUpdater::slotDownloadFinishPerFile(QString fileName)
{
    sigDownloadFinishPerFile(fileName);
}

void AutoUpdater::slotDownloadTimeout(QString fileName)
{
    g_log.log(UpdateLog::INFO, "Download file: " + fileName + " time out!", __FILE__, __LINE__);
    m_downloadTimeoutList.append(fileName);

    //Stop all current download
    for(int i = 0; i < m_ftpList.size(); i++)
    {
        if(m_ftpList.at(i))
            m_ftpList.at(i)->deleteLater();
    }

    //Delete all alreary download.
    FailDeleteNewVersionDir();

    //It is emited for AutoUpdaterUI for download time out.
    sigDownloadTimeout();
}

void AutoUpdater::FailDeleteNewVersionDir()
{
    //Delete All already download files
    g_log.log(UpdateLog::INFO, "Make script to delete already download file when time out!", __FILE__, __LINE__);
    QString scriptName = "delNewVersion.bat";
    QString scriptPath = QApplication::applicationDirPath();
    MakeDeletePathScript(scriptPath, m_newVersionPath, scriptName);

    //The script position.
    QString delScript = scriptPath + "/" + scriptName;
    QProcess *p = new QProcess(this);
    p->start(delScript);
    m_listProcess.push_back(p);
}

QStringList AutoUpdater::GetDownloadTimeoutList()
{
    return m_downloadTimeoutList;
}

void AutoUpdater::slotStorageDownloadError(QString errStr)
{
    m_replyErrorStack.push_back(errStr);
}

QStringList AutoUpdater::GetFtpErrorStack()
{
    return m_replyErrorStack;
}

void AutoUpdater::RestartApp()
{
    g_log.log(UpdateLog::INFO, "Restart application for run new version!", __FILE__, __LINE__);
    QString delScriptPath;
    delScriptPath = MakeDeletePathScript(QApplication::applicationDirPath(),
                                         QApplication::applicationDirPath(),
                                         "del.bat");
    //Make desktop link for new version.
    CreateNewLink();

    g_log.log(UpdateLog::INFO, "Run " + delScriptPath + " script to delete old script", __FILE__, __LINE__);
    QProcess::startDetached(delScriptPath);

    //Execute delete script file, and terminal old version appliction
    //taskkill /f /t /im AutoUpdateTestV1.0.exe
    QString oldApp = APPLICATION_NAME + m_oldVersion + ".exe";
    QString killOldAppCommand = "taskkill /f /t /im " + oldApp;
    g_log.log(UpdateLog::INFO, QString::asprintf("Kill old version process, path : %1, command: %2").arg(oldApp).arg(killOldAppCommand),
              __FILE__, __LINE__);
    QProcess::startDetached(killOldAppCommand);

    //Start new version application.
    QString newApp = m_newVersionPath + "/" + APPLICATION_NAME + m_newVersion + ".exe";
    g_log.log(UpdateLog::INFO, "Start new version, path: " + newApp, __FILE__, __LINE__);
    QProcess::startDetached(newApp);

    exit(0);
}

QString AutoUpdater::MakeDeletePathScript(const QString saveScriptPath, QString delPath,
                                       const QString scriptName)
{
    g_log.log(UpdateLog::INFO, "Make script of delete old version!", __FILE__, __LINE__);
    //ping -n 3 127.0.0.1>nul -- wait third second to remove old version path
    //third second is wait current process exit.
    delPath = delPath.replace(QRegExp("\\/"), "\\\\");
    QString content = "ping -n 3 127.0.0.1>nul\n"
                      "@echo off\n"
                      "rd /s/q " + delPath;

    g_log.log(UpdateLog::INFO, delPath + " file content: " + content, __FILE__, __LINE__);

    //The delete script file storage in the new version path
    QFile script(saveScriptPath + "/" + scriptName);
    if(!script.open(QIODevice::WriteOnly | QIODevice::Text))
        return "";
    QTextStream in(&script);
    in << content;
    script.close();

    return saveScriptPath + "/" + scriptName;
}

void AutoUpdater::CreateNewLink()
{
    g_log.log(UpdateLog::INFO, "Create new link for new version to desktop.", __FILE__, __LINE__);
    QString desktopLink;
    desktopLink.append(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    desktopLink.append("/");
    desktopLink.append(APPLICATION_NAME);
    desktopLink.append(".lnk");

    QString newAppPath = m_newVersionPath + "/" + APPLICATION_NAME + m_newVersion + ".exe";
    g_log.log(UpdateLog::INFO, "New application path: " + newAppPath, __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, "Desktop link: " + desktopLink, __FILE__, __LINE__);
    QFile::link(newAppPath, desktopLink);
}


