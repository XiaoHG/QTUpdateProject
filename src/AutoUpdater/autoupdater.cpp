
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
#include <QStandardPaths>
#include <QFile>
#include <QProcess>
#include <QThread>

extern UpdateLog g_log;

#ifdef Q_OS_MAC
    const QString UPDATE_SYSTEM = "mac";
#endif

#ifdef Q_OS_LINUX
    const QString UPDATE_SYSTEM = "linux";
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
	QString _strScriptPath;

#ifdef Q_OS_MAC
	//execute old version path, and delete script path self.
	_strScriptPath = QApplication::applicationDirPath() + "/del_old_path";
#endif

#ifdef Q_OS_LINUX
	//execute old version path, and delete script path self.
	_strScriptPath = QApplication::applicationDirPath() + "/del_old_path";
#endif

#ifdef Q_OS_WIN
	//execute old version path, and delete script path self.
	_strScriptPath = QApplication::applicationDirPath() + "/del_old_path.bat";
#endif

    g_log.log(UpdateLog::FATAL, "Exec delete old path script: " + _strScriptPath, __FILE__, __LINE__);
    QProcess::startDetached(_strScriptPath);
}

void AutoUpdater::downloadInitFile()
{
    QString _strDownloadXmlPath = QApplication::applicationDirPath() + "/download";
    QDir downloadDir(_strDownloadXmlPath);
    if(!downloadDir.exists())
        downloadDir.mkdir(_strDownloadXmlPath);

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

QString AutoUpdater::getVersionInfo(const QString &path)
{
    QString _strVersionInfo;
    QString _strVersionInfoName;
    //Chinese or English from parent process language is.
    if(m_bCh)
    {
        _strVersionInfoName = "versionInfoCh.txt";
    }
    else
    {
        _strVersionInfoName = "versionInfoEn.txt";
    }

    //Version information file that stroge at download directory from ftp server.
    //QString _strVersionInfoPath = QApplication::applicationDirPath() + "/download/" + _strVersionInfoName;
    QFile _file(path + "/" + _strVersionInfoName);
    if(!_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        g_log.log(UpdateLog::FATAL, "Can't open file: " + QApplication::applicationDirPath() + "/download/" + _strVersionInfoName,
                  __FILE__, __LINE__);
        return Q_NULLPTR;
    }

    QTextStream _in(&_file);
    _strVersionInfo = _in.readAll();
    _file.close();

    return _strVersionInfo;
}

AutoUpdater::UPDATER_ERROR_CODE AutoUpdater::isUpdate()
{
    //Compare two files of local updater.xml and download from ftp server.
    QString _strLocalXML = QApplication::applicationDirPath() + "/updater.xml";
    QString _strDownloadXML = QApplication::applicationDirPath() + "/download/updater.xml";

    g_log.log(UpdateLog::INFO, "Local xml: " + _strLocalXML, __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, "Download xml: " + _strDownloadXML, __FILE__, __LINE__);

    QFile _file(_strLocalXML);
    if(!_file.exists())
    {
        g_log.log(UpdateLog::FATAL, "Local xml is lost, make local xml file on init.", __FILE__, __LINE__);
        makeInitXML();
    }

    //Get the version string from down updater xml file.
    QDomNodeList _nodeNewList;
    _nodeNewList = XMLParser::parseElement(_strDownloadXML, "version");
    if(_nodeNewList.isEmpty())
    {
        g_log.log(UpdateLog::FATAL, "Parse download xml file error!", __FILE__, __LINE__);
        return LOCALXML_PARSE_ERR;
    }
    QString _strNewVersion = _nodeNewList.at(0).toElement().text();
    m_strNewVersion = _strNewVersion;
    m_strNewVersionPath = QApplication::applicationDirPath() + "/" +
                        DONWLOAD_PATH + APPLICATION_NAME + m_strNewVersion;

    //Get the version string from local updater xml file.
    QDomNodeList _nodeOldList;
    _nodeOldList = XMLParser::parseElement(_strLocalXML, "version");
    if(_nodeOldList.isEmpty())
    {
        g_log.log(UpdateLog::FATAL, "Parse local xml file error!", __FILE__, __LINE__);
        return DOWNLOADXML_PARSE_ERR;
    }
    QString _strOldVersion = _nodeOldList.at(0).toElement().text();
    m_strOldVersion = _strOldVersion;
    m_strOldVersionPath = QApplication::applicationDirPath();

    //set ini file old version path.
    QString _strOldVersionPath = QApplication::applicationDirPath();
    g_log.log(UpdateLog::INFO, QString::asprintf("Old version path: %1").arg(_strOldVersionPath),
                __FILE__, __LINE__);

    g_log.log(UpdateLog::INFO, QString::asprintf("Old version: %1, New version %2").arg(_strOldVersion).arg(_strNewVersion),
              __FILE__, __LINE__);

    //Compare.
    QStringList _strNewVersionList = _strNewVersion.split('.');
    QStringList _strOldVersionList = _strOldVersion.split('.');
    for(int i = 0; i < _strNewVersionList.size(); ++i)
    {
        if(i >= _strOldVersionList.size() || _strNewVersionList.at(i) > _strOldVersionList.at(i))
        {
            g_log.log(UpdateLog::INFO, "Server version is updater, need to update!", __FILE__, __LINE__);
            return UPDATE;
        }
    }
    g_log.log(UpdateLog::INFO, "Local Version is the laster version, it is not need to update!", __FILE__, __LINE__);
    return NOTUPDATE;
}

void AutoUpdater::makeInitXML()
{
    //Made in application path.
    QString _strXml = QApplication::applicationDirPath() + "/updater.xml";
    QFile _file(_strXml);
    if(!_file.open(QIODevice::WriteOnly))
    {
        g_log.log(UpdateLog::FATAL, "Make init xml false, can not open: " + _strXml + " file!", __FILE__, __LINE__);
        return;
    }

    //Init content.
    QString _strInitXmlContent = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                  "<autoupdate>\n"
                  "    <version>V0.0</version>\n"
                  "    <filelist>\n"
                  "    </filelist>\n"
                  "</autoupdate>";
    QTextStream _txtStream(&_file);
    _txtStream << _strInitXmlContent;
    g_log.log(UpdateLog::INFO, "Local init xml file content: " + _strInitXmlContent, __FILE__, __LINE__);
    _file.close();
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
    QString _strXmlPath = QApplication::applicationDirPath() + "/download/updater.xml";
    if(!QFile::exists(_strXmlPath))
    {
        g_log.log(UpdateLog::FATAL, "Download xml is not exist: " + _strXmlPath + " not exist!", __FILE__, __LINE__);
        return;
    }

    //Read xml file, and storage at the buffer m_listFileDir and m_listFileName,
    //for download use.
    QString _strName;
    QString _strDir;
    QString _strMd5;
    QDomNodeList _nodeList = XMLParser::parseElement(_strXmlPath, "file");
    for(int i = 0; i < _nodeList.size(); ++i)
    {
        _strName = _nodeList.at(i).toElement().attribute("name");
        _strDir = _nodeList.at(i).toElement().attribute("dir");
        _strMd5 = _nodeList.at(i).toElement().attribute("md5");

        QString logDir = _strDir.isEmpty() ? "./" : _strDir;
        g_log.log(UpdateLog::INFO, "Load: " + _strName + " file, and directory is: " + logDir, __FILE__, __LINE__);

        m_listFileDir.append(_strDir);
        m_listFileName.append(_strName);
        m_listFileMd5.append(_strMd5);
    }

    g_log.log(UpdateLog::INFO, "Load update files over.", __FILE__, __LINE__);
}

int AutoUpdater::getUpdateProcess()
{
    g_log.log(UpdateLog::INFO, QString::asprintf("Download finish count: %1").arg(m_iFinishDownloadCount),
              __FILE__, __LINE__);
    return m_iFinishDownloadCount * 100 / m_listFileName.size();
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
        QString _strLocalFileDir = m_strNewVersionPath + m_listFileDir.at(i);
        QDir _directory(_strLocalFileDir);
        if(!_directory.exists())
        {
            g_log.log(UpdateLog::DEBUG, "mkdir " + _strLocalFileDir, __FILE__, __LINE__);
            _directory.mkpath(_strLocalFileDir);
        }
        _strLocalFileDir += "/" + m_listFileName.at(i);

        //strFileDirServer is ftp server position.
        QString _strFileDirServer;
        if(m_listFileDir.at(i) == "")
            _strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion +
                               "/" + UPDATE_SYSTEM + "/" + m_listFileName.at(i);
        else
            _strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion +
                               "/" + UPDATE_SYSTEM + m_listFileDir.at(i) + "/"  + m_listFileName.at(i);

        FtpManager *ftp = new FtpManager(m_listFileMd5.at(i), this);
        m_listFtp.push_back(ftp);
        connect(ftp, SIGNAL(signal_startDownloadPerFile(QString)), this, SLOT(on_ftp_startDownloadPerFile(QString)));
        connect(ftp, SIGNAL(signal_finishDownloadPerFile(QString)), this, SLOT(on_ftp_finishDownloadPerFile(QString)));
        connect(ftp, SIGNAL(signal_replyError(QString)), this, SLOT(on_ftp_storageDownloadErrStr(QString)));

        ftp->get(_strFileDirServer, _strLocalFileDir);
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
    QString _strDelayTime = QString::asprintf("%1").arg(delay);
    QString _strFileName;
#ifdef Q_OS_MAC
    QString _strScriptContent = "#!/bin/sh\n"
                                  "sleep " + _strDelayTime + "\n"
                                  "rm -rf " + delPath;
    _strFileName = saveScriptPath + "/" + scriptName;
#endif

#ifdef Q_OS_LINUX
    QString _strScriptContent = "#!/bin/sh\n"
                                  "sleep " + _strDelayTime + "\n"
                                  "rm -rf " + delPath;
    _strFileName = saveScriptPath + "/" + scriptName;
#endif

#ifdef Q_OS_WIN
    delPath = delPath.replace(QRegExp("\\/"), "\\\\");
    QString _strScriptContent;
    QString self = saveScriptPath + "/" + scriptName;
    self = self.replace(QRegExp("\\/"), "\\");
    if(delay == 0)
    {
        _strScriptContent = "rd /s/q " + delPath + "\n" + "del /s/q " + self + ".bat";
    }
    else
    {
        _strScriptContent = "ping -n " + _strDelayTime + " 0.0.0.0>nul\n" +
                "@echo off\n"
                "rd /s/q " + delPath + "\n" + "del /s/q " + self + ".bat";
    }
    _strFileName = saveScriptPath + "/" + scriptName + ".bat";
#endif

    g_log.log(UpdateLog::INFO, delPath + "\\\\" + scriptName + " file content: " + _strScriptContent, __FILE__, __LINE__);

    //The delete script file storage in the new version path
    QFile _file(_strFileName);
    if (!_file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
        g_log.log(UpdateLog::FATAL, "File: " + _strFileName + " open fail!", __FILE__, __LINE__);
		return;
	}

    QTextStream _in(&_file);
    _in << _strScriptContent;
    _file.close();

}

void AutoUpdater::saveLog()
{
    QString _strLogPath = m_strNewVersionPath + "/log";
    QDir _dir(_strLogPath);
    if(!_dir.exists())
    {
        _dir.mkdir(_strLogPath);
    }
    _strLogPath = _strLogPath + "/old_updater.log";
    QString _strSourceLogPath = QApplication::applicationDirPath() + "/log/updater.log";
    g_log.log(UpdateLog::INFO, "Copy old log path: " + _strSourceLogPath, __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, "Copy old log to path: " + _strLogPath, __FILE__, __LINE__);
    QFile::copy(_strSourceLogPath, _strLogPath);
}

void AutoUpdater::setParentPid(QString parentPid)
{
    m_strParentPid = parentPid;
}

void AutoUpdater::restartApp()
{
    g_log.log(UpdateLog::INFO, "Restart application for run new version!", __FILE__, __LINE__);

    QString _strDelPath = QApplication::applicationDirPath();
    makeDeletePathScript(QApplication::applicationDirPath(), _strDelPath, "del_self", 3);

    //Make desktop link for new version.
    createNewLink();

    QString _strDelScriptPath;

#ifdef Q_OS_MAC
    _strDelScriptPath = QApplication::applicationDirPath() + "/del_self";
#endif

#ifdef Q_OS_LINUX
    _strDelScriptPath = QApplication::applicationDirPath() + "/del_self";
#endif

#ifdef Q_OS_WIN
    _strDelScriptPath = QApplication::applicationDirPath() + "/del_self.bat";
#endif

    g_log.log(UpdateLog::INFO, "Run " + _strDelScriptPath + " script to delete old script", __FILE__, __LINE__);
    QProcess::startDetached(_strDelScriptPath);


    QString _strKillOld;
    QString _strApplicationName;
#ifdef Q_OS_MAC
    _strKillOld = "kill -9 " + m_strParentPid;
    _strApplicationName = APPLICATION_NAME;
#endif

#ifdef Q_OS_LINUX
    _strKillOld = "kill -9 " + m_strParentPid;
    _strApplicationName = APPLICATION_NAME;
#endif

#ifdef Q_OS_WIN
    _strKillOld = "taskkill /f /t /pid " + m_strParentPid;
	//Execute delete script file, and terminal old version appliction
    //taskkill /f /t /im AutoUpdateTestV1.0.exe
    _strApplicationName = APPLICATION_NAME + ".exe";
#endif

	//Start new version application.
    QStringList _arguments;
    QString _strStartNew = m_strNewVersionPath + "/" + _strApplicationName;
    g_log.log(UpdateLog::INFO, "Start new version, path: " + _strStartNew, __FILE__, __LINE__);
    QProcess::startDetached(_strStartNew, _arguments, m_strNewVersionPath);
	
    g_log.log(UpdateLog::INFO, QString::asprintf("Kill old version process, name : %1, command: %2")
              .arg(_strApplicationName).arg(_strKillOld), __FILE__, __LINE__);

	//Save log to new version path.
	saveLog();

	//save old version parameter.
    saveOldVersionPara();

    //delete old version path, delay 3s is work.
    makeDeletePathScript(m_strNewVersionPath, m_strOldVersionPath, "del_old_path", 3);

    //set ini file old version string
    m_settingsUpdaterIni->setValue("version/old_version_path", m_strOldVersionPath);
    m_settingsUpdaterIni->setValue("version/old_version", m_strOldVersion);

	//kill the old
    QProcess::startDetached(_strKillOld);
    
    exit(0);
}

void AutoUpdater::saveOldVersionPara()
{
    QString _strTargeIniPath = m_strNewVersionPath + "/updater.ini";
    QFile::copy(m_strUpdaterIniPath, _strTargeIniPath);
}

void AutoUpdater::createNewLink()
{
	QString _strNewAppPath;

#ifdef Q_OS_MAC
	//execute old version path, and delete script path self.
	_strNewAppPath = m_strNewVersionPath + "/" + APPLICATION_NAME;
#endif

#ifdef Q_OS_LINUX
	//execute old version path, and delete script path self.
	_strNewAppPath = m_strNewVersionPath + "/" + APPLICATION_NAME;
#endif

#ifdef Q_OS_WIN
	//execute old version path, and delete script path self.
	_strNewAppPath = m_strNewVersionPath + "/" + APPLICATION_NAME + ".exe";
#endif

    g_log.log(UpdateLog::INFO, "Create new link for new version to desktop.", __FILE__, __LINE__);
    QString _strDesktopLink;
    _strDesktopLink.append(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    _strDesktopLink.append("/");
    _strDesktopLink.append(APPLICATION_NAME);
    _strDesktopLink.append(".lnk");

    g_log.log(UpdateLog::INFO, "New application path: " + _strNewAppPath, __FILE__, __LINE__);
    g_log.log(UpdateLog::INFO, "Desktop link: " + _strDesktopLink, __FILE__, __LINE__);
    QFile::link(_strNewAppPath, _strDesktopLink);
}

void AutoUpdater::failDeleteNewVersionDir()
{
    QDir _strNewVersionDir;
    if(m_strNewVersionPath.isEmpty())
        _strNewVersionDir.setPath("nothing");
    else
        _strNewVersionDir.setPath(m_strNewVersionPath);
    if(!_strNewVersionDir.exists())
    {
        return;
    }

    //Delete All already download files
    g_log.log(UpdateLog::INFO, "Make script to delete already download file!", __FILE__, __LINE__);
    QString _strScriptName = "delNewVersion.bat";
    QString _strScriptPath = QApplication::applicationDirPath();
    makeDeletePathScript(_strScriptPath, m_strNewVersionPath, _strScriptName, 0);

    //The script position.
    g_log.log(UpdateLog::INFO, "Execete script to delete already download file!", __FILE__, __LINE__);
    QString _strDelScript = _strScriptPath + "/" + _strScriptName;
    QProcess::startDetached(_strDelScript);
}

void AutoUpdater::abnormalExit()
{
    //Stop
    if(m_bCheckForUpdateError)
    {
        stopDownload();
    }

    //Delete all alreary download.
    failDeleteNewVersionDir();
}

