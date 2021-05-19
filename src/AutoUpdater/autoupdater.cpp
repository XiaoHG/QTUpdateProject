#include "autoupdater.h"
#include "xmlparser.h"
#include "ftpmanager.h"
#include "ftpmanager.h"

#include <QDir>
#include <QDomDocument>
#include <QCoreApplication>
#include <QProcess>
#include <QMessageBox>
#include <QThread>
#include <QApplication>
#include <QStringLiteral>

static const QString VERSION_PATH = "/version";
static const QString APPLICATION_NAME = "main";
static const QString DONWLOAD_PATH = "../";

AutoUpdater::AutoUpdater()
{
}

AutoUpdater::~AutoUpdater()
{
    for(int i = 0; i < m_ftpList.size(); ++i)
    {
        m_ftpList.at(i)->deleteLater();
    }
}

void AutoUpdater::DownloadXMLFile()
{
    QDir downloadDir(QApplication::applicationDirPath() + "/download");
    if(!downloadDir.exists())
        downloadDir.mkdir(QApplication::applicationDirPath() + "/download");

    FtpManager *ftp = new FtpManager();
    m_ftpList.push_back(ftp);
    ftp->get(VERSION_PATH + "/updater.xml", QApplication::applicationDirPath() + "/download/updater.xml");
    connect(ftp, SIGNAL(sigDownloadUpdaterXmlOver()), this, SLOT(slotDownloadUpdaterXmlOver()));
    connect(ftp, SIGNAL(sigReplyError(QString)), this, SLOT(slotReplyError(QString)));
}

/**
 * @brief AutoUpdater::slotReplyError
 * @param errStr
 * Just remain 5 data that the newest
**/
void AutoUpdater::slotReplyError(QString errStr)
{
    if(m_replyErrorStack.size() >= 5)
        m_replyErrorStack.pop_back();
    m_replyErrorStack.push_front(errStr);
}

QStringList AutoUpdater::GetFtpErrorStack()
{
    return m_replyErrorStack;
}

void AutoUpdater::slotDownloadUpdaterXmlOver()
{
    FtpManager *ftp = new FtpManager();
    m_ftpList.push_back(ftp);
    ftp->get(VERSION_PATH + "/versionInfo.txt", QApplication::applicationDirPath() + "/download/versionInfo.txt");
    connect(ftp, SIGNAL(sigDownloadVersionInfoFileOver()), this, SLOT(slotDownloadVersionInfoFileOver()));
}

void AutoUpdater::slotDownloadVersionInfoFileOver()
{
    sigDownloadUpdaterFileOver();
}

/**
/** * @brief xml1
/** * 比较两个版本号
 *
/** * xml1,xml2:两个XML文件，不能为空，并且文件需要存在,xml1必须是最新的xml文件（刚刚下载下来的）,xml2是本地的xml文件;
**/
bool AutoUpdater::CheckUpdateFiles(QString xml1, QString xml2)
{
    m_listFileDir.clear();
    m_listFileName.clear();

    if(xml1.isEmpty() || xml2.isEmpty())
        return false;

    qDebug() << QStringLiteral("下载的版本文件 xml1 = ") << xml1;
    qDebug() << QStringLiteral("本地版本文件 xml2 = ") << xml2;

    if(!QFile::exists(xml1))
    {
        m_strTip = QStringLiteral("本地更新文件不存在");
        qDebug() << m_strTip;
        return false;
    }

    if(!QFile::exists(xml2))
    {
        m_strTip = QStringLiteral("下载更新文件不存在！");
        qDebug() << m_strTip;
        return false;
    }

    m_strTip = QStringLiteral("检查需要更新的文件...");
    qDebug() << m_strTip;

    QDomNodeList nodeList = XMLParser::XMLParseElement(xml1, "file");
    for(int i = 0; i < nodeList.size(); ++i)
    {
        QString name = nodeList.at(i).toElement().attribute("name");
        QString dir = nodeList.at(i).toElement().attribute("dir");
        QString version = nodeList.at(i).toElement().attribute("version");

        QString localVersion = GetElementVersion(xml2, name);//获取本地xml文件对应文件（name）的版本信息
        if(localVersion.isEmpty())//本地XML没有此文件：下载并放到相应的目录中
        {
            qDebug() << QStringLiteral("Neet to update: ") << name << " version = " << localVersion << " file need to update!";
            m_listFileDir.append(dir);
            m_listFileName.append(name);
        }
        else
        {
            /**Check update**/
            qDebug() << "Checked version : " << version << ", " << localVersion;
            if(CheckVersion(version, localVersion))
            {
                qDebug() << QStringLiteral("Need to update: ") << name << " version = " << localVersion << " file need to update!";
                m_listFileDir.append(dir);
                m_listFileName.append(name);
            }
            else
            {
                qDebug() << QStringLiteral("The file is already the laster!") << version << dir << name;
            }
        }
    }
    if(!m_listFileName.isEmpty())
    {
        qDebug() << "It is files that neet to download: " << m_listFileName.size();
        return true;
    }

    else
        return false;
}


/**
 * 在xml中查找version，并返回版本号
 * 查找的是指定文件（name）的版本号
 * 前提是已经存在版本管理文件（xml）,对其进行解析。
**/
QString AutoUpdater::GetElementVersion(QString xml, QString name)
{
    QString result = "";
    if(xml.isEmpty() || name.isEmpty())
    {
        qDebug() << "xml or name is empty!";
        return result;
    }

    if(!QFile::exists(xml))
    {
        qDebug() << "xml not exist!";
        return result;
    }
    int i = 0;
    QDomNodeList nodeList = XMLParser::XMLParseElement(xml, "file");
    for(; i < nodeList.size(); ++i)
    {
        QString tmpName = nodeList.at(i).toElement().attribute("name");
        //QString dir = nodeList.at(i).toElement().attribute("dir");
        QString version = nodeList.at(i).toElement().attribute("version");

        if(name == tmpName)
        {
            qDebug() << name << " find!, version = " << version;
            result = version;
            break;
        }
    }
    if(i == nodeList.size())
    {
        qDebug() << "can't find!" << name;
    }

    return result;
}

bool AutoUpdater::CheckVersionForUpdate()
{
    //这里需要一个函数检查本版版本控制文件是否存在，如果不存在则创建一个基本内容
    //的版本文件
    QString localXML = QApplication::applicationDirPath() + "/updater.xml";
    QString downloadXML = QApplication::applicationDirPath() + "/download/updater.xml";
    if(!CheckXML(localXML))
    {
        makeInitXML(localXML);
    }
    return CheckVersion(GetVersion(downloadXML), GetVersion(localXML));
}

bool AutoUpdater::CheckXML(QString xml)
{
    QFile file(xml);
    if(file.exists())
        return true;
    return false;
}

void AutoUpdater::makeInitXML(QString xml)
{
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

QString AutoUpdater::GetVersion(QString xml)
{
    QDomNodeList nodeList = XMLParser::XMLParseElement(xml, "version");
    QString version = nodeList.at(0).toElement().text();
    return version;
}

/**
 * 本地版本号和服务器最新版本号进行对比
 * 如果本地版本号和服务器最新版本号一致，则返回false，否则返回true
 * 返回true表示需要更新当前文件
 * 版本号暂定：V1.0 V2.0 ... , 来进行处理
**/
bool AutoUpdater::CheckVersion(QString version, QString versionLocal)
{
    QStringList downloadVersionList = version.split('.');
    QStringList localVersionList = versionLocal.split('.');
    for(int i = 0 ; i < downloadVersionList.size(); ++i)
    {
        qDebug() << downloadVersionList.at(i);
        qDebug() << localVersionList.at(i);
        if(i >= localVersionList.size() || downloadVersionList.at(i) > localVersionList.at(i))
        {
            qDebug() << QStringLiteral("Server version is updater, need to update!");
            return true;
        }
    }
    qDebug() << QStringLiteral("Local Version is the laster version, it is not need to update!");
    return false;
}

/**
 * 下载最新的版本的文件，并替换或者增加
 * 旧XML中有的文件，新XML没有的，此文件不做处理。（后期在考虑怎么处理）
 * 下载信息由m_listFileName和m_listFileDir提供
**/
void AutoUpdater::DownloadUpdateFiles()
{
    if(m_listFileDir.isEmpty() || m_listFileName.isEmpty())
    {
        qDebug() << QStringLiteral("It is not file thit neet to update!");
        return;
    }

    m_strTip = "Start download...";
    qDebug() << m_strTip;

    QString downloadVersion = GetVersion(QApplication::applicationDirPath() + "/download/updater.xml");

    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        m_strTip = QStringLiteral("download ...") + m_listFileName.at(i);
        qDebug() << m_strTip;

        /** Download path **/
        QString strPlaceDir = QApplication::applicationDirPath() + "/" +
                                DONWLOAD_PATH + APPLICATION_NAME + downloadVersion +
                                m_listFileDir.at(i);
        m_strPlaceDirList.push_back(strPlaceDir);
        QDir directory(strPlaceDir);
        if(!directory.exists())
        {
            directory.mkpath(strPlaceDir);
        }

        QString strFileDirServer;
        if(m_listFileDir.at(i) == "")
            strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME +
                    downloadVersion + "/" + m_listFileName.at(i);
        else
            strFileDirServer = VERSION_PATH + "/" + APPLICATION_NAME + downloadVersion +
                                        m_listFileDir.at(i) + "/" + m_listFileName.at(i);

        strPlaceDir += "/" + m_listFileName.at(i);
        FtpManager *ftp = new FtpManager();
        m_ftpList.push_back(ftp);
        ftp->get(strFileDirServer, strPlaceDir);
    }
}

/**读取版本信息文件**/
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

QStringList AutoUpdater::GetUpdateFileDir()
{
    return m_listFileDir;
}

QStringList AutoUpdater::GetUpdateFileName()
{
    return m_listFileName;
}

int AutoUpdater::GetUpdateProcess()
{
    // - 2: updater.xml and versionInfo.txt
    return (FtpManager::GetFinishCount() - 2) * 100 / m_listFileName.size();
}

QStringList AutoUpdater::GetCurrDownloadFileList()
{
    return FtpManager::GetCurDownloadFileList();
}

QStringList AutoUpdater::GetFinishDownloadFileList()
{
    return FtpManager::GetFinishDownloadFileList();
}
