
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

#ifdef Q_OS_WIN
    const QString UPDATE_SYSTEM = "win32";
#endif

//#ifdef Q_OS_WIN64
//    const QString UPDATE_SYSTEM = "win64";
//#endif

const QString VERSION_PATH = "/version";
const QString APPLICATION_NAME = "AutoUpdateTest";
const QString DONWLOAD_PATH = "../";

AutoUpdater::AutoUpdater(bool bCh)
    :m_bCh(bCh),
      m_iFinishDownloadCount(0),
      m_bCheckForUpdateError(true)
{
    //The updater.xml file is in the local version path
    m_strLocalXmlPath = QApplication::applicationDirPath() + "/updater.xml";

    //The updater.xml file is download from server.
    m_strDownloadXmlPath = QApplication::applicationDirPath() + "/download/updater.xml";

    //Chinese language of version information file.
    m_strDownloadVersionInfoChPath = QApplication::applicationDirPath() + "/download/versionInfoCh.txt";

    //English language of version information file.
    m_strDownloadVersionInfoEnPath = QApplication::applicationDirPath() + "/download/versionInfoEn.txt";

    //made ini file for updater process.
    m_strUpdaterIniPath = QApplication::applicationDirPath() + "/updater.ini";
    m_settingsUpdaterIni = new QSettings(m_strUpdaterIniPath, QSettings::IniFormat);

}

AutoUpdater::~AutoUpdater()
{
    for (int i = 0; i < m_listFtp.size(); i++)
	{
        m_listFtp.at(i)->deleteLater();
	}
}

void AutoUpdater::execDeleteOldScript()
{
    //execute old version path, and delete script path self.
    QString scriptPath = QApplication::applicationDirPath() + "/deloldpath.bat";
    g_log.log(UpdateLog::FATAL, "Exec delete old path script: " + scriptPath, __FILE__, __LINE__);
    QProcess::startDetached(scriptPath);
}

void AutoUpdater::downloadInitFile()
{
    QString downloadXmlPath = QApplication::applicationDirPath() + "/download";
    QDir downloadDir(downloadXmlPath);
    if(!downloadDir.exists())
        downloadDir.mkdir(downloadXmlPath);

    FtpManager *ftp = new FtpManager("0", this);
    m_listFtp.push_back(ftp);
    ftp->get(VERSION_PATH + "/updater.xml", m_strDownloadXmlPath);
    connect(ftp, SIGNAL(signal_downloadXmlFinish()), this, SLOT(on_ftp_downloadXmlFinish()));
    connect(ftp, SIGNAL(signal_replyError(QString)), this, SLOT(on_ftp_storageDownloadErrStr(QString)));
}

void AutoUpdater::on_ftp_downloadXmlFinish()
{
    FtpManager *ftp = new FtpManager("0", this);
    m_listFtp.push_back(ftp);
    ftp->get(VERSION_PATH + "/versionInfoCh.txt", m_strDownloadVersionInfoChPath);
    connect(ftp, SIGNAL(signal_downloadChFinish()), this, SLOT(on_ftp_downloadChFinish()));
    connect(ftp, SIGNAL(signal_replyError(QString)), this, SLOT(on_ftp_storageDownloadErrStr(QString)));
}

void AutoUpdater::on_ftp_downloadChFinish()
{
    FtpManager *ftp = new FtpManager("0", this);
    m_listFtp.push_back(ftp);
    ftp->get(VERSION_PATH + "/versionInfoEn.txt", m_strDownloadVersionInfoEnPath);
    connect(ftp, SIGNAL(signal_downloadEnFinish()), this, SLOT(on_ftp_downloadEnFinish()));
    connect(ftp, SIGNAL(signal_replyError(QString)), this, SLOT(on_ftp_storageDownloadErrStr(QString)));
}

void AutoUpdater::on_ftp_downloadEnFinish()
{
    //Download init files over.
    m_bCheckForUpdateError = false;
    //It is emited to AutoUpdaterUI class for a information that init file download
    //over, and can do next step that check version for update.
    signal_initFileDownloadFinish();
}

QString AutoUpdater::getVersionInfo()
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

bool AutoUpdater::isUpdate()
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
    nodeNewList = XMLParser::parseElement(downloadXML, "version");
    QString newVersion = nodeNewList.at(0).toElement().text();
    m_strNewVersion = newVersion;
    m_strNewVersionPath = QApplication::applicationDirPath() + "/" +
                        DONWLOAD_PATH + APPLICATION_NAME + m_strNewVersion;

    //Get the version string from local updater xml file.
    QDomNodeList nodeOldList;
    nodeOldList = XMLParser::parseElement(localXML, "version");
    QString oldVersion = nodeOldList.at(0).toElement().text();
    m_strOldVersion = oldVersion;
    m_strOldVersionPath = QApplication::applicationDirPath();

    //set ini file old version path.
    QString oldVersionPath = QApplication::applicationDirPath();
	g_log.log(UpdateLog::INFO, QString::asprintf("Old version path: %1").arg(oldVersionPath),
                __FILE__, __LINE__);

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

QString AutoUpdater::getOldVersion()
{
    return m_strOldVersion;
}

QString AutoUpdater::getNewVersion()
{
    return m_strNewVersion;
}

void AutoUpdater::loadUpdateFiles()
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
    QString name;
    QString dir;
    QString md5;
    QDomNodeList nodeList = XMLParser::parseElement(xmlPath, "file");
    for(int i = 0; i < nodeList.size(); ++i)
    {
        name = nodeList.at(i).toElement().attribute("name");
        dir = nodeList.at(i).toElement().attribute("dir");
        md5 = nodeList.at(i).toElement().attribute("md5");

        QString logDir = dir.isEmpty() ? "./" : dir;
        g_log.log(UpdateLog::INFO, "Load: " + name + " file, and directory is: " + logDir, __FILE__, __LINE__);

        m_listFileDir.append(dir);
        m_listFileName.append(name);
        m_listFileMd5.append(md5);
    }

    g_log.log(UpdateLog::INFO, "Load update files over.", __FILE__, __LINE__);
}

int AutoUpdater::getUpdateProcess()
{
    g_log.log(UpdateLog::INFO, QString::asprintf("Download finish count: %1").arg(m_iFinishDownloadCount),
              __FILE__, __LINE__);
    int process = m_iFinishDownloadCount * 100 / m_listFileName.size();
    return process;
}

void AutoUpdater::downloadUpdateFiles()
{
    //If download buffer is empty, do nothing.
    if(m_listFileDir.isEmpty() || m_listFileName.isEmpty())
    {
        g_log.log(UpdateLog::INFO, "It is not file thit neet to update!", __FILE__, __LINE__);
        return;
    }

    g_log.log(UpdateLog::DEBUG, "Start download ... ", __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, QString::asprintf("Total download files: %1").arg(m_listFileName.size()),
                  __FILE__, __LINE__);
    //Download each file of ftp server.
    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        g_log.log(UpdateLog::DEBUG, "Download file: " + m_listFileName.at(i), __FILE__, __LINE__);

        //localFileDir is download storage position.
        QString localFileDir = m_strNewVersionPath + m_listFileDir.at(i);
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
            strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion +
                               "/" + UPDATE_SYSTEM + "/" + m_listFileName.at(i);
        else
            strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion +
                               "/" + UPDATE_SYSTEM + m_listFileDir.at(i) + "/"  + m_listFileName.at(i);

        FtpManager *ftp = new FtpManager(m_listFileMd5.at(i), this);
        m_listFtp.push_back(ftp);
        connect(ftp, SIGNAL(signal_startDownloadPerFile(QString)), this, SLOT(on_ftp_startDownloadPerFile(QString)));
        connect(ftp, SIGNAL(signal_finishDownloadPerFile(QString)), this, SLOT(on_ftp_finishDownloadPerFile(QString)));
        connect(ftp, SIGNAL(signal_replyError(QString)), this, SLOT(on_ftp_storageDownloadErrStr(QString)));

        ftp->get(strFileDirServer, localFileDir);
    }
}

void AutoUpdater::on_ftp_startDownloadPerFile(QString fileName)
{
    signal_startDownloadPerFile(fileName);
}

void AutoUpdater::on_ftp_finishDownloadPerFile(QString fileName)
{
    m_iFinishDownloadCount++;
    signal_finishDownloadPerFile(fileName);
}

void AutoUpdater::stopDownload()
{
    //Stop all current download
    for(int i = 0; i < m_listFtp.size(); i++)
    {
        if(m_listFtp.at(i))
            m_listFtp.at(i)->deleteLater();
    }
}

void AutoUpdater::on_ftp_storageDownloadErrStr(QString errStr)
{
    m_listFtpReplyErrorStack.push_back(errStr);
}

QStringList AutoUpdater::getFtpErrorStack()
{
    return m_listFtpReplyErrorStack;
}

void AutoUpdater::makeDeletePathScript(const QString saveScriptPath, QString delPath,
                                       const QString scriptName, const int delay)
{
    g_log.log(UpdateLog::INFO, "Make script for delete old version!", __FILE__, __LINE__);
    //ping -n 3 127.0.0.1>nul -- wait third second to remove old version path
    //third second is wait current process exit.
    QString delayTime = QString::asprintf("%1").arg(delay);

#ifdef Q_OS_MAC
    QString content = "#!/bin/sh\n"
                      "sleep " + delayTime +
                      "rm -rf " + delPath;
#endif

#ifdef Q_OS_LINUX
    QString content = "#!/bin/sh\n"
                      "sleep " + delayTime +
                      "rm -rf " + delPath;
#endif

#ifdef Q_OS_WIN
    delPath = delPath.replace(QRegExp("\\/"), "\\\\");
    QString content;
    QString self = saveScriptPath + "/" + scriptName;
    self = self.replace(QRegExp("\\/"), "\\");
    if(delay == 0)
    {
        content = "rd /s/q " + delPath + "\n" + "del /s/q " + self;
    }
    else
    {
        content = "ping -n " + delayTime + " 0.0.0.0>nul\n" +
                "@echo off\n"
                "rd /s/q " + delPath + "\n" + "del /s/q " + self;
    }
#endif

    g_log.log(UpdateLog::INFO, delPath + "\\\\" + scriptName + " file content: " + content, __FILE__, __LINE__);

    //The delete script file storage in the new version path
	QString fileName = saveScriptPath + "/" + scriptName;
    QFile script(fileName);
	if (!script.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		g_log.log(UpdateLog::FATAL, "File: " + fileName + " open fail!", __FILE__, __LINE__);
		return;
	}

    QTextStream in(&script);
    in << content;
    script.close();

}

void AutoUpdater::saveLog()
{
    QString logPath = m_strNewVersionPath + "/log";
    QDir logDir(logPath);
    if(!logDir.exists())
    {
        logDir.mkdir(logPath);
    }
    logPath = logPath + "/old_updater.log";
    QString sourceLogPath = QApplication::applicationDirPath() + "/log/updater.log";
    g_log.log(UpdateLog::INFO, "Copy old log path: " + sourceLogPath, __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, "Copy old log to path: " + logPath, __FILE__, __LINE__);
    QFile::copy(sourceLogPath, logPath);
}

void AutoUpdater::setParentPid(QString parentPid)
{
    m_strParentPid = parentPid;
}

void AutoUpdater::restartApp()
{
    g_log.log(UpdateLog::INFO, "Restart application for run new version!", __FILE__, __LINE__);

    QString delPath = QApplication::applicationDirPath();
    makeDeletePathScript(QApplication::applicationDirPath(), delPath, "del.bat", 3);

    //Make desktop link for new version.
    createNewLink();

    QString delScriptPath = QApplication::applicationDirPath() + "/del.bat";
    g_log.log(UpdateLog::INFO, "Run " + delScriptPath + " script to delete old script", __FILE__, __LINE__);
    QProcess::startDetached(delScriptPath);

    //Execute delete script file, and terminal old version appliction
    //taskkill /f /t /im AutoUpdateTestV1.0.exe
    QString applicationName = APPLICATION_NAME + ".exe";

#ifdef Q_OS_MAC
    QString killOld = "kill -9 " + m_parentPid;
#endif

#ifdef Q_OS_LINUX
    QString killOld = "kill -9 " + m_parentPid;
#endif

#ifdef Q_OS_WIN
    QString killOld = "taskkill /f /t /pid " + m_strParentPid;
#endif

	//Start new version application.
    QStringList arguments;
    QString startNew = m_strNewVersionPath + "/" + applicationName;
    g_log.log(UpdateLog::INFO, "Start new version, path: " + startNew, __FILE__, __LINE__);
    QProcess::startDetached(startNew, arguments, m_strNewVersionPath);
	
    g_log.log(UpdateLog::INFO, QString::asprintf("Kill old version process, name : %1, command: %2")
              .arg(applicationName).arg(killOld), __FILE__, __LINE__);

	//Save log to new version path.
	saveLog();

	//save old version parameter.
    saveOldVersionPara();

    //delete old version path, delay 3s is work.
    makeDeletePathScript(m_strNewVersionPath, m_strOldVersionPath, "deloldpath.bat", 3);

    //set ini file old version string
    m_settingsUpdaterIni->setValue("version/old_version_path", m_strOldVersionPath);
    m_settingsUpdaterIni->setValue("version/old_version", m_strOldVersion);

	//kill the old
    QProcess::startDetached(killOld);
    
    exit(0);
}

void AutoUpdater::saveOldVersionPara()
{
    QString targeIniPath = m_strNewVersionPath + "/updater.ini";
    QFile::copy(m_strUpdaterIniPath, targeIniPath);
}

void AutoUpdater::createNewLink()
{
    g_log.log(UpdateLog::INFO, "Create new link for new version to desktop.", __FILE__, __LINE__);
    QString desktopLink;
    desktopLink.append(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    desktopLink.append("/");
    desktopLink.append(APPLICATION_NAME);
    desktopLink.append(".lnk");

    QString newAppPath = m_strNewVersionPath + "/" + APPLICATION_NAME + ".exe";
    g_log.log(UpdateLog::INFO, "New application path: " + newAppPath, __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, "Desktop link: " + desktopLink, __FILE__, __LINE__);
    QFile::link(newAppPath, desktopLink);
}

void AutoUpdater::failDeleteNewVersionDir()
{
    QDir newVersionDir;
    if(m_strNewVersionPath.isEmpty())
        newVersionDir.setPath("nothing");
    else
        newVersionDir.setPath(m_strNewVersionPath);
    if(!newVersionDir.exists())
    {
        return;
    }

    //Delete All already download files
    g_log.log(UpdateLog::INFO, "Make script to delete already download file!", __FILE__, __LINE__);
    QString scriptName = "delNewVersion.bat";
    QString scriptPath = QApplication::applicationDirPath();
    makeDeletePathScript(scriptPath, m_strNewVersionPath, scriptName, 0);

    //The script position.
    g_log.log(UpdateLog::INFO, "Execete script to delete already download file!", __FILE__, __LINE__);
    QString delScript = scriptPath + "/" + scriptName;
//    QProcess *p = new QProcess(this);
//    p->start(delScript);
    QProcess::startDetached(delScript);
}

void AutoUpdater::abnormalExit()
{
    //Stop
    if(m_bCheckForUpdateError)
    {
        stopDownload();
    }

    //Delete all alreary download.
    //failDeleteNewVersionDir();
}

