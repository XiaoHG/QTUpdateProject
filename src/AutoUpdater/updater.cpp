﻿
#include "updater.h"
#include "xmlparser.h"
#include "ftpmanager.h"
#include "ftpmanager.h"
#include "log.h"
#include "errorstack.h"

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

extern Log g_log;

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
const QString LatestVERSION_FILE_NAME = "LatestV.xml";
const QString VERSIONCONTROL_FILE_NAME = "updater.xml";
const QString VERSIONINFOCH_FILE_NAME = "versionInfoCh.txt";
const QString VERSIONINFOEN_FILE_NAME = "versionInfoEn.txt";
const QString INITFILE_DOWNLOADPATH = "/updater";
//const QString UPDATERCTLFILE_DOWNLOADNODENAME = "file";

Updater::Updater(bool bCh, QObject *parent)
    : QObject(parent),
      m_bCh(bCh),
      m_iFinishDownloadCount(0),
      m_bCheckForUpdateError(true),
      m_bUpdate(false),
      m_iFinishInitFileCount(0)
{
    //The updater.xml file is in the local version path
    m_strInitFileDownloadPath = QApplication::applicationDirPath() + INITFILE_DOWNLOADPATH;
    g_log.log(Log::INFO, "Init file download path: " + m_strInitFileDownloadPath, __FILE__, __LINE__);
}

Updater::~Updater()
{
    for (int i = 0; i < m_listFtp.size(); i++)
	{
        m_listFtp.at(i)->deleteLater();
	}
}

void Updater::deleteOldVersionPath()
{
    g_log.log(Log::DEBUG, "Delete old version path.", __FILE__, __LINE__);

#ifdef Q_OS_MAC
    //execute old version path, and delete script path self.
    //QProcess::startDetached("rm -rf " + QApplication::applicationDirPath());
#endif

#ifdef Q_OS_LINUX
    //execute old version path, and delete script path self.
    //QProcess::startDetached("rm -rf " + QApplication::applicationDirPath());
#endif

#ifdef Q_OS_WIN

	//execute old version path, and delete script path self.
    QString _strScriptPath = QApplication::applicationDirPath() + "/del_old_path.bat";
    g_log.log(Log::DEBUG, "Exec delete old path script: " + _strScriptPath, __FILE__, __LINE__);
    if(QProcess::startDetached(_strScriptPath))
    {
        g_log.log(Log::WARN, "Exec delete old path script error!", __FILE__, __LINE__);
    }

#endif

}

void Updater::downloadLatestVFile()
{
    QDir _dir(m_strInitFileDownloadPath);
    if(!_dir.exists())
    {
        g_log.log(Log::DEBUG, m_strInitFileDownloadPath + " not exist, and make it.",
                  __FILE__, __LINE__);
        _dir.mkdir(m_strInitFileDownloadPath);
    }

    FtpManager *ftp = new FtpManager("0", this);
    m_listFtp.push_back(ftp);
    ftp->get(VERSION_PATH + "/" + LatestVERSION_FILE_NAME, m_strInitFileDownloadPath + "/" + LatestVERSION_FILE_NAME);
    connect(ftp, SIGNAL(signal_downloadLatestVFinish()), this, SLOT(on_ftp_downloadLatestVFileFinish()));
    connect(ftp, SIGNAL(signal_replyError(QString)), this, SLOT(on_ftp_reportError(QString)));
}

void Updater::on_ftp_downloadLatestVFileFinish()
{
    //Server updater.xml
    QString _strDownloadXmlFullPath = m_strInitFileDownloadPath + "/" + LatestVERSION_FILE_NAME;
    QDomNodeList _nodeNewList;
    _nodeNewList = XMLParser::parseElement(_strDownloadXmlFullPath, "version");
    if(_nodeNewList.isEmpty())
    {
        QString _strErrLogMsg = _strDownloadXmlFullPath + " parse download xml file error!";
        reportError(_strErrLogMsg, PARSE_DOWNLOAD_XML_ERR, g_mapError.find(PARSE_DOWNLOAD_XML_ERR).value());
        return;
    }
    m_strNewVersion = _nodeNewList.at(0).toElement().text();
    m_strLocalNewVersionPath = QApplication::applicationDirPath() + "/../" + APPLICATION_NAME + m_strNewVersion;
    g_log.log(Log::DEBUG, "Download version is: " + m_strNewVersion, __FILE__, __LINE__);

    //local updater.xml
    QString _strLocalXmlFullPath = QApplication::applicationDirPath() + "/" + VERSIONCONTROL_FILE_NAME;
    QFile _file(_strLocalXmlFullPath);
    if(!_file.exists())
    {
        g_log.log(Log::WARN, "Local xml is lost, make local xml file on init.", __FILE__, __LINE__);
        makeInitXML();
    }
    _nodeNewList = XMLParser::parseElement(_strLocalXmlFullPath, "version");
    if(_nodeNewList.isEmpty())
    {
        QString _strErrLogMsg = _strLocalXmlFullPath + " parse local xml file error!";
        reportError(_strErrLogMsg, PARSE_LOCAL_XML_ERR, g_mapError.find(PARSE_LOCAL_XML_ERR).value());
        return;
    }
    m_strOldVersion = _nodeNewList.at(0).toElement().text();
    g_log.log(Log::DEBUG, "Local version is: " + m_strOldVersion, __FILE__, __LINE__);

    //Compare, version.
    QString _strNewVersion = m_strNewVersion;
    QString _strOldVersion = m_strOldVersion;
    QStringList _strNewVersionList = _strNewVersion.remove(0, 1).split('.');
    QStringList _strOldVersionList = _strOldVersion.remove(0, 1).split('.');

    int i = 0;
    for(; i < _strNewVersionList.size(); i++)
    {
        if(i >= _strOldVersionList.size() || _strNewVersionList.at(i).toInt() > _strOldVersionList.at(i).toInt())
        {
            g_log.log(Log::INFO, "Server version is updater, need to update!", __FILE__, __LINE__);
            m_bUpdate = true;
        }
    }
    if(!m_bUpdate)
    {
        g_log.log(Log::INFO, "Local version is the Latest version, it is not need to update!", __FILE__, __LINE__);
        signal_initFileDownloadFinish(LatestVERSION_FILE_NAME);
        return;
    }

#ifdef Q_OS_WIN32
    m_strServerNewVersionPath = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion + "/win32";
#endif

//#ifdef Q_OS_WIN64
//    m_strServerNewVersionPath = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion + "/win64";
//#endif

#ifdef Q_OS_MAC
    m_strServerNewVersionPath = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion + "/mac";
#endif

#ifdef Q_OS_LINUX
    m_strServerNewVersionPath = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion + "/linux";
#endif

    QString _strServerUpdaterCtlXmlFullPath = m_strServerNewVersionPath + "/" + VERSIONCONTROL_FILE_NAME;
    QString _strServerVersionInfoChFullPath = m_strServerNewVersionPath + "/" + VERSIONINFOCH_FILE_NAME;
    QString _strServerVersionInfoEnFullPath = m_strServerNewVersionPath + "/" + VERSIONINFOEN_FILE_NAME;

    QString _strLocalUpdaterCtlXmlFullPath = m_strInitFileDownloadPath + "/" + VERSIONCONTROL_FILE_NAME;
    QString _strLocalVersionInfoChFullPath = m_strInitFileDownloadPath + "/" + VERSIONINFOCH_FILE_NAME;
    QString _strLocalVersionInfoEnFullPath = m_strInitFileDownloadPath + "/" + VERSIONINFOEN_FILE_NAME;

    g_log.log(Log::INFO, "Download 1 init file: server = " + _strServerUpdaterCtlXmlFullPath + ", local = " + _strLocalUpdaterCtlXmlFullPath,
              __FILE__, __LINE__);
    g_log.log(Log::INFO, "Download 2 init file: server = " + _strServerVersionInfoChFullPath + ", local = " + _strLocalVersionInfoChFullPath,
              __FILE__, __LINE__);
    g_log.log(Log::INFO, "Download 3 init file: server = " + _strServerVersionInfoEnFullPath + ", local = " + _strLocalVersionInfoEnFullPath,
              __FILE__, __LINE__);

    m_mapInitFile.insert(_strServerUpdaterCtlXmlFullPath, _strLocalUpdaterCtlXmlFullPath);
    m_mapInitFile.insert(_strServerVersionInfoChFullPath, _strLocalVersionInfoChFullPath);
    m_mapInitFile.insert(_strServerVersionInfoEnFullPath, _strLocalVersionInfoEnFullPath);

    //Download init file, 3 by for now : updater.xml, versionInfoCh.txt, versionInfoEn.txt
    for(QMap<QString, QString>::Iterator it = m_mapInitFile.begin(); it != m_mapInitFile.end(); it++)
    {
        FtpManager *ftp = new FtpManager("0", this);
        m_listFtp.push_back(ftp);
        ftp->get(it.key(), it.value());
        connect(ftp, SIGNAL(signal_finishDownload(QString)), this, SLOT(on_ftp_downloadInitFinish(QString)));
        connect(ftp, SIGNAL(signal_replyError(QString)), this, SLOT(on_ftp_reportError(QString)));
    }
}

void Updater::on_ftp_downloadInitFinish(QString name)
{
    m_iFinishInitFileCount++;
    g_log.log(Log::DEBUG, QString::asprintf("Download init file finish count = %1, m_mapInitFile.size() = %2")
              .arg(m_iFinishInitFileCount).arg(m_mapInitFile.size()), __FILE__, __LINE__);
    if(m_iFinishInitFileCount == m_mapInitFile.size())
    {
        //Download init files over.
        m_bCheckForUpdateError = false;
        //It is emited to AutoUpdaterUI class for a information that init file download
        //over, and can do next step that check version for update.
        signal_initFileDownloadFinish(name);
    }

}

QString Updater::getLocalNewVersionPath()
{
    return m_strLocalNewVersionPath;
}

QString Updater::getVersionInfo()
{
    QString _strVersionInfo;
    QString _strVersionInfoName;
    QString _strVersionInfoPath;
    ErrorEnum _eErrorCode;
    //Chinese or English from parent process language is.
    if(m_bCh)
    {
        _strVersionInfoName = "versionInfoCh.txt";
    }
    else
    {
        _strVersionInfoName = "versionInfoEn.txt";
    }

    if(m_bUpdate)
    {
        _strVersionInfoPath = QApplication::applicationDirPath() + INITFILE_DOWNLOADPATH;
        _eErrorCode = OPEN_DOWNLOAD_VERSIONINFO_FILE_ERR;
    }
    else
    {
        _strVersionInfoPath = QApplication::applicationDirPath();
        _eErrorCode = OPEN_LOCAL_VERSIONINFO_FILE_ERR;
    }

    //
    QString _strFile = _strVersionInfoPath + "/" + _strVersionInfoName;
    QFile _file(_strFile);
    if(!_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString _strErrLogMsg = "Can't open file: " + _strFile;
        reportError(_strErrLogMsg, _eErrorCode, g_mapError.find(_eErrorCode).value());
        return Q_NULLPTR;
    }

    QTextStream _in(&_file);
    _strVersionInfo = _in.readAll();
    _file.close();

    return _strVersionInfo;
}

void Updater::reportError(const QString &strLogMsg, const int &eErrStackCode, const QString &strErrStack)
{
    g_log.log(Log::FATAL, strLogMsg, __FILE__, __LINE__);
    ErrorStack::pushError(eErrStackCode, strErrStack);
    signal_reportError(strErrStack);
}

bool Updater::isUpdate()
{
    return m_bUpdate;
}

void Updater::makeInitXML()
{
    //Made in application path.
    QString _strXml = QApplication::applicationDirPath() + "/updater.xml";
    QFile _file(_strXml);
    if(!_file.open(QIODevice::WriteOnly))
    {
        QString _strErrLogMsg = "Make init xml false, can not open: " + _strXml + " file!";
        reportError(_strErrLogMsg, OPEN_INITUPDATER_FILE_ERR, g_mapError.find(OPEN_INITUPDATER_FILE_ERR).value());
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
    g_log.log(Log::INFO, "Local init xml file content: " + _strInitXmlContent, __FILE__, __LINE__);
    _file.close();
}

QString Updater::getOldVersion()
{
    return m_strOldVersion;
}

QString Updater::getNewVersion()
{
    return m_strNewVersion;
}

void Updater::loadUpdateFiles()
{
    //Clear buffer first.
    m_listFileDir.clear();
    m_listFileName.clear();

    //Load file that download from ftp server name updater.xml.
    QString _strDownloadXmlPath = m_strInitFileDownloadPath + "/" + VERSIONCONTROL_FILE_NAME;

    //Read xml file, and storage at the buffer m_listFileDir and m_listFileName,
    //for download use.
    QString _strName;
    QString _strDir;
    QString _strMd5;
    QDomNodeList _nodeList = XMLParser::parseElement(_strDownloadXmlPath, "file");
    for(int i = 0; i < _nodeList.size(); ++i)
    {
        _strName = _nodeList.at(i).toElement().attribute("name");
        _strDir = _nodeList.at(i).toElement().attribute("dir");
        _strMd5 = _nodeList.at(i).toElement().attribute("md5");

        QString _strServerFileDir = _strDir.isEmpty() ? "./" : _strDir;
        g_log.log(Log::INFO, "Load: " + _strName + " file, and directory is: " + _strServerFileDir, __FILE__, __LINE__);

        m_listFileName.append(_strName);
        m_listFileDir.append(_strDir);
        m_listFileMd5.append(_strMd5);
    }

    g_log.log(Log::INFO, "Load update files over.", __FILE__, __LINE__);
}

int Updater::getUpdateProcess()
{
    g_log.log(Log::DEBUG, QString::asprintf("Download finish count: %1").arg(m_iFinishDownloadCount),
              __FILE__, __LINE__);
    return m_iFinishDownloadCount * 100 / m_listFileName.size();
}

void Updater::downloadUpdateFiles()
{
    g_log.log(Log::DEBUG, "Start download ... ", __FILE__, __LINE__);
    g_log.log(Log::DEBUG, QString::asprintf("Total download files: %1").arg(m_listFileName.size()),
                 __FILE__, __LINE__);

    //Download each file of ftp server.
    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        g_log.log(Log::DEBUG, "Download file: " + m_listFileName.at(i), __FILE__, __LINE__);

        //localFileDir is download storage position.
        QString _strLocalFileDir = m_strLocalNewVersionPath + m_listFileDir.at(i);
        QDir _directory(_strLocalFileDir);
        if(!_directory.exists())
        {
            g_log.log(Log::DEBUG, "mkdir " + _strLocalFileDir, __FILE__, __LINE__);
            _directory.mkdir(_strLocalFileDir);
        }
        _strLocalFileDir += "/" + m_listFileName.at(i);

        //_strServerFileDir is ftp server position.
        QString _strServerFileDir;
        if(m_listFileDir.at(i) == "")
            _strServerFileDir = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion +
                               "/" + UPDATE_SYSTEM + "/" + m_listFileName.at(i);
        else
            _strServerFileDir = VERSION_PATH + "/" + APPLICATION_NAME + m_strNewVersion +
                               "/" + UPDATE_SYSTEM + m_listFileDir.at(i) + "/"  + m_listFileName.at(i);

        FtpManager *ftp = new FtpManager(m_listFileMd5.at(i), this);
        m_listFtp.push_back(ftp);
        connect(ftp, SIGNAL(signal_startDownload(QString)), this, SLOT(on_ftp_startDownloadPerFile(QString)));
        connect(ftp, SIGNAL(signal_finishDownload(QString)), this, SLOT(on_ftp_finishDownloadPerFile(QString)));
        connect(ftp, SIGNAL(signal_replyError(QString)), this, SLOT(on_ftp_reportError(QString)));

        //download.
        ftp->get(_strServerFileDir, _strLocalFileDir);
    }
}

void Updater::on_ftp_startDownloadPerFile(QString fileName)
{
    signal_startDownloadPerFile(fileName);
}

void Updater::on_ftp_finishDownloadPerFile(QString fileName)
{
    //record the download finish count.
    m_iFinishDownloadCount++;
    signal_finishDownloadPerFile(fileName);
}

void Updater::stopDownload()
{
    //Stop all current download, just check for update.
    for(int i = 0; i < m_listFtp.size(); i++)
    {
        if(m_listFtp.at(i))
            m_listFtp.at(i)->deleteLater();
    }
}

void Updater::on_ftp_reportError(QString errStr)
{
    signal_reportError(errStr);
}

void Updater::makeDeletePathScript(const QString &saveScriptPath, QString delPath,
                                       const QString &scriptName, const int &delay)
{
    g_log.log(Log::INFO, "Make script for delete old version!", __FILE__, __LINE__);
    //ping -n 3 127.0.0.1>nul -- wait third second to remove old version path
    //third second is wait current process exit.
    QString _strDelayTime = QString::asprintf("%1").arg(delay);

    QString _strScriptPath;
    QString _strScriptContent;

    delPath = delPath.replace(QRegExp("\\/"), "\\\\");
    QString self = saveScriptPath + "/" + scriptName;
    self = self.replace(QRegExp("\\/"), "\\");

    if(delay == 0)
    {
        _strScriptContent = "rd /s/q " + delPath + "\n" + "del /s/q " + self;
    }
    else
    {
        _strScriptContent = "ping -n " + _strDelayTime + " 0.0.0.0>nul\n" +
                            "@echo off\n"
                            "rd /s/q " + delPath + "\n" + "del /s/q " + self;
    }
    _strScriptPath = saveScriptPath + "/" + scriptName;

    g_log.log(Log::INFO, delPath + "/" + scriptName + " file content: " + _strScriptContent, __FILE__, __LINE__);

    //The delete script file storage in the new version path
    QFile _file(_strScriptPath);
    if (!_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString _strErrLogMsg = "File: " + _strScriptPath + " open fail!";
        reportError(_strErrLogMsg, OPEN_SCRIPT_FILE_ERR, g_mapError.find(OPEN_SCRIPT_FILE_ERR).value());
		return;
	}

    QTextStream _in(&_file);
    _in << _strScriptContent;
    _file.close();

}

void Updater::saveOldVersionLog()
{
    QString _strLogPath = m_strLocalNewVersionPath + "/log";
    QDir _dir(_strLogPath);
    if(!_dir.exists())
    {
        g_log.log(Log::INFO, "Mkdir: " + _strLogPath, __FILE__, __LINE__);
        _dir.mkdir(_strLogPath);
    }
    _strLogPath = _strLogPath + "/old_updater.log";
    QString _strSourceLogPath = QApplication::applicationDirPath() + "/log/updater.log";
    g_log.log(Log::INFO, "Copy old log path: " + _strSourceLogPath, __FILE__, __LINE__);
    g_log.log(Log::INFO, "Copy old log to path: " + _strLogPath, __FILE__, __LINE__);
    QFile::copy(_strSourceLogPath, _strLogPath);
}

void Updater::setParentPid(QString parentPid)
{
    m_strParentPid = parentPid;
}

void Updater::restartApp()
{
    g_log.log(Log::INFO, "Restart application for run new version!", __FILE__, __LINE__);

    QString _strDelPath = QApplication::applicationDirPath();
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

    //Make desktop link for new version.
    createNewLink();

    QString _strDelScriptName = "del_self.bat";
    makeDeletePathScript(QApplication::applicationDirPath(), _strDelPath, _strDelScriptName, 3);
    QString _strDelScriptPath = QApplication::applicationDirPath() + "/" + _strDelScriptName;
    g_log.log(Log::DEBUG, "Run " + _strDelScriptPath + " script to delete old script", __FILE__, __LINE__);
    QProcess::startDetached(_strDelScriptPath);

    _strKillOld = "taskkill /f /t /pid " + m_strParentPid;
	//Execute delete script file, and terminal old version appliction
    //taskkill /f /t /pid PID
    _strApplicationName = APPLICATION_NAME + ".exe";

    //delete old version path, delay 3s is work.
    QString _strDelOldScriptName = "del_old_path.bat";
    makeDeletePathScript(m_strLocalNewVersionPath, QApplication::applicationDirPath(), _strDelOldScriptName, 3);

#endif

	//Start new version application.
    QStringList _arguments;
    QString _strStartNew = m_strLocalNewVersionPath + "/" + _strApplicationName;
    g_log.log(Log::INFO, "Start new version, path: " + _strStartNew, __FILE__, __LINE__);
    QProcess::startDetached(_strStartNew, _arguments, m_strLocalNewVersionPath);
	
    g_log.log(Log::INFO, QString::asprintf("Kill old version process, name : %1, command: %2")
              .arg(_strApplicationName).arg(_strKillOld), __FILE__, __LINE__);

	//Save log to new version path.
    saveOldVersionLog();

	//kill the old
    QProcess::startDetached(_strKillOld);
    
    exit(0);
}

//just windows
void Updater::createNewLink()
{
	QString _strNewAppPath;

	//execute old version path, and delete script path self.
    _strNewAppPath = m_strLocalNewVersionPath + "/" + APPLICATION_NAME + ".exe";

    g_log.log(Log::INFO, "Create new link for new version to desktop.", __FILE__, __LINE__);
    QString _strDesktopLink;
    _strDesktopLink.append(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    _strDesktopLink.append("/");
    _strDesktopLink.append(APPLICATION_NAME);
    _strDesktopLink.append(".lnk");

    g_log.log(Log::INFO, "New application path: " + _strNewAppPath, __FILE__, __LINE__);
    g_log.log(Log::INFO, "Desktop link: " + _strDesktopLink, __FILE__, __LINE__);
    QFile::link(_strNewAppPath, _strDesktopLink);
}

void Updater::failDeleteNewVersionDir()
{
    if(m_strLocalNewVersionPath.isEmpty())
    {
        g_log.log(Log::DEBUG, "New version path is empty.", __FILE__, __LINE__);
        return;
    }

#ifdef Q_OS_MAC
    //The script position.
    g_log.log(UpdateLog::INFO, "Delete already download file! path = " + m_strNewVersionPath, __FILE__, __LINE__);
    QProcess::startDetached("rm -rf " + m_strNewVersionPath);
#endif

#ifdef Q_OS_LINUX
    //The script position.
    g_log.log(UpdateLog::INFO, "Delete already download file! path = " + m_strNewVersionPath, __FILE__, __LINE__);
    QProcess::startDetached("rm -rf " + m_strNewVersionPath);
#endif

#ifdef Q_OS_WIN
    //Delete All already download files
    g_log.log(Log::INFO, "Make script to delete already download file!", __FILE__, __LINE__);
    QString _strScriptName = "delNewVersion.bat";
    QString _strScriptPath = QApplication::applicationDirPath();
    makeDeletePathScript(_strScriptPath, m_strLocalNewVersionPath, _strScriptName, 0);

    //The script position.
    QString _strDelScript = _strScriptPath + "/" + _strScriptName;
    g_log.log(Log::INFO, "Execete script to delete already download file! path = " + _strDelScript, __FILE__, __LINE__);
    QProcess::startDetached(_strDelScript);
#endif

}

void Updater::abnormalExit()
{

    g_log.log(Log::DEBUG, "Abnormal exit", __FILE__, __LINE__);
    g_log.log(Log::DEBUG, "=========ERROR STACK BEGIN========", __FILE__, __LINE__);

    const QVector<PERROR_STRUCT> _sErrorStruct = ErrorStack::getErrorStack();
    for(int i = 0; i < _sErrorStruct.size(); i++)
    {
        g_log.log(Log::DEBUG, _sErrorStruct.at(i)->strError, __FILE__, __LINE__);
    }

    g_log.log(Log::DEBUG, "=========ERROR STACK END========", __FILE__, __LINE__);

    //Stop
    if(m_bCheckForUpdateError)
    {
        stopDownload();
        return;
    }

    //Delete all alreary download.
    failDeleteNewVersionDir();
}

