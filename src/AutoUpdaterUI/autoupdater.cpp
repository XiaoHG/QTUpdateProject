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

AutoUpdater::AutoUpdater()
{
    m_bCopyOver = false;
}

AutoUpdater::~AutoUpdater()
{
    for(int i = 0; i < m_ftpList.size(); ++i)
    {
        m_ftpList.at(i)->deleteLater();
    }
}

void AutoUpdater::DownloadUpdaterXmlFile()
{
    QDir downloadDir(QApplication::applicationDirPath() + "/download");
    if(!downloadDir.exists())
        downloadDir.mkdir(QApplication::applicationDirPath() + "/download");

    FtpManager *ftp = new FtpManager();
    m_ftpList.push_back(ftp);
    ftp->get("/version/updater.xml", QApplication::applicationDirPath() + "/download/updater.xml");
    connect(ftp, SIGNAL(sigDownloadUpdaterXmlOver()), this, SLOT(slotDownloadUpdaterXmlOver()));
}

void AutoUpdater::DownloadVersionInfoFile()
{
    FtpManager *ftp = new FtpManager();
    m_ftpList.push_back(ftp);
    ftp->get("/version/versionInfo.txt", QApplication::applicationDirPath() + "/download/versionInfo.txt");
    connect(ftp, SIGNAL(sigDownloadVersionInfoFileOver()), this, SLOT(slotDownloadVersionInfoFileOver()));
}

void AutoUpdater::slotDownloadUpdaterXmlOver()
{
    DownloadVersionInfoFile();
}

void AutoUpdater::slotDownloadVersionInfoFileOver()
{
    sigDownloadUpdaterXmlOver();
}

/**
/** * @brief xml1
/** * 比较两个版本号
 *
/** * xml1,xml2:两个XML文件，不能为空，并且文件需要存在,xml1必须是最新的xml文件（刚刚下载下来的）,xml2是本地的xml文件;
**/
int AutoUpdater::CheckUpdateFiles(QString xml1, QString xml2)
{
    m_listFileDir.clear();
    m_listFileName.clear();

    if(xml1.isEmpty() || xml2.isEmpty())
        return 0;

    qDebug() << QStringLiteral("下载的版本文件 xml1 = ") << xml1;
    qDebug() << QStringLiteral("本地版本文件 xml2 = ") << xml2;

    if(QFile::exists(xml1))
    {
        if(QFile::exists(xml2))
        {
            m_strTip = QStringLiteral("检查需要更新的文件111111111111111111...");
            qDebug() << m_strTip;

            QDomNodeList nodeList = XMLParser::XMLParseElement(xml1, "file");
            for(int i = 0; i < nodeList.size(); ++i)
            {
                QString name = nodeList.at(i).toElement().attribute("name");
                QString dir = nodeList.at(i).toElement().attribute("dir");
                QString version = nodeList.at(i).toElement().attribute("version");

                qDebug() << "CheckUpdateFiles xml1 name = " << name;
                QString localVersion = GetElementVersion(xml2, name);//获取本地xml文件对应文件（name）的版本信息
                if(localVersion.isEmpty())//本地XML没有此文件：下载并放到相应的目录中
                {
                    qDebug() << QStringLiteral("需要更新：") << name << " version = " << localVersion << " file need to update!";
                    m_listFileDir.append(dir);
                    m_listFileName.append(name);
                }
                else
                {
                    /**检查版本，如果本地版本低于下载版本，则下载**/
                    qDebug() << "Checked version : " << version << ", " << localVersion;
                    qDebug() << "call CheckVersion!!!!!!!!!!!!!!!!!";
                    if(CheckVersion(version, localVersion))
                    {
                        qDebug() << QStringLiteral("需要更新：") << name << " version = " << localVersion << " file need to update!";
                        m_listFileDir.append(dir);
                        m_listFileName.append(name);
                    }
                    else
                    {
                        qDebug() << QStringLiteral("文件是最新版本，不需要更新  ") << version << dir << name;
                    }
                }
            }
            qDebug() << "m_listFileName.size() = " << m_listFileName.size();
            if(!m_listFileName.isEmpty())
                return 1;
            else
                return 0;
        }
        else
        {
            m_strTip = QStringLiteral("下载更新文件不存在！");
            qDebug() << m_strTip;
            return 0;
        }
    }
    else
    {
        m_strTip = QStringLiteral("本地更新文件不存在");
        qDebug() << m_strTip;
        return 0;
    }
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
        makeXML(localXML);
    }
    qDebug() << localXML;
    qDebug() << downloadXML;
    QString xml1Version = GetVersion(localXML);
    QString xml2Version = GetVersion(downloadXML);
    qDebug() << QStringLiteral("两个版本对比：") << "local = " << xml1Version
             << ", download = " << xml2Version;
    return CheckVersion(xml2Version, xml1Version);
}

bool AutoUpdater::CheckXML(QString xml)
{
    QFile file(xml);
    if(file.exists())
        return true;
    return false;
}

void AutoUpdater::makeXML(QString xml)
{
    QFile file(xml);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "makeXML false, can not open + " << xml;
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
    qDebug() << "downloadVersionList.size() = " << downloadVersionList.size();
    qDebug() << "localVersionList.size() = " << localVersionList.size();
    for(int i = 0 ; i < downloadVersionList.size(); ++i)
    {
        qDebug() << downloadVersionList.at(i);
        qDebug() << localVersionList.at(i);
        if(i >= localVersionList.size() || downloadVersionList.at(i) > localVersionList.at(i))
        {
            qDebug() << QStringLiteral("服务器版本比较新，需要更新！");
            return true;
        }
    }
    qDebug() << QStringLiteral("最新版本，不需要更新！");
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
        qDebug() << QStringLiteral("没有需要更新的文件！");
        return;
    }

    m_strTip = "开始下载更新文件 ...";
    qDebug() << m_strTip;

    QString downloadVersion = GetVersion(QApplication::applicationDirPath() + "/download/updater.xml");

    //这里是下载模块，现在没有服务器，暂时不进行下载动作，解决在本地download文件拷贝。
    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        m_strTip = QStringLiteral("正在下载文件 ...") + m_listFileName.at(i);
        qDebug() << m_strTip;

        /**放置下载文件的路径**/
        QString strPlaceDir = QApplication::applicationDirPath() +
                                "/../main" + downloadVersion + "/" +
                                m_listFileDir.at(i);
        m_strPlaceDirList.push_back(strPlaceDir);
        QDir directory(strPlaceDir);//如果路径不存在，则创建
        if(!directory.exists())
        {
            directory.mkpath(strPlaceDir);
        }

        //文件在服务器中的存储位置
        QString strFileDirServer = "/version/main" + downloadVersion +
                                        m_listFileDir.at(i) + "/" + m_listFileName.at(i);
        strPlaceDir += "/" + m_listFileName.at(i);
        FtpManager *ftp = new FtpManager();
        m_ftpList.push_back(ftp);
        qDebug() << "Download: ";
        qDebug() << strFileDirServer;
        qDebug() << strPlaceDir;
        ftp->get(strFileDirServer, strPlaceDir);
    }
}

/**读取版本信息文件**/
QStringList AutoUpdater::GetVersionInfo()
{
    QStringList strTxtList;
    QString strLine;
    qDebug() << "versionInfo file: " << QApplication::applicationDirPath() + "/download/versionInfo.txt";
    QFile file(QApplication::applicationDirPath() + "/download/versionInfo.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "can't open file: " << QApplication::applicationDirPath() + "/download/versionInfo.txt";
        return strTxtList;
    }

    //file.readAll();

    QTextStream in(&file);  //用文件构造流
    strLine = in.readLine();//读取一行放到字符串里
    while(!strLine.isNull())//字符串有内容
    {
        //QStringLiteral
        strTxtList.append(strLine.toLocal8Bit());
        strLine = in.readLine();//循环读取下行
    }
    return strTxtList;
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
    qDebug() << "FtpManager::m_finishCount = " << FtpManager::m_finishCount;
    qDebug() << "m_listFileName.size() = " << m_listFileName.size();
    return (FtpManager::m_finishCount - 2) * 100 / m_listFileName.size();
}

QStringList AutoUpdater::GetCurrDownloadFileList()
{
    return FtpManager::m_currDownloadFileList;
}

QStringList AutoUpdater::GetFinishDownloadFileList()
{
    return FtpManager::m_finishDownloadFileList;
}
