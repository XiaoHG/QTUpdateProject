#include "cautoupdater.h"
#include "chttpdownloadfile.h"
#include "cxmlparser.h"

#include <QDir>
#include <QDomDocument>
#include <QCoreApplication>
#include <QProcess>
#include <QMessageBox>
#include <QThread>

CAutoUpdater::CAutoUpdater()
{
    m_progUpdate = 1;
    m_progDownload = 1;
    m_bCopyOver = false;
}

CAutoUpdater::~CAutoUpdater()
{

}

/**
 * 下载服务器版本控制文件updater.xml
**/
void CAutoUpdater::DownloadXMLFile()
{
    QString strCurrentDir = QDir::currentPath();
    QString strDownload = strCurrentDir + "/download";
    qDebug() << "strDownload = " << strDownload;
    QDir diretory(strDownload);
    if(!diretory.exists())//如果路径不存在，则创建
    {
        diretory.mkpath(strDownload);
    }

    //http://localhost/updateClientVarsion/
    //http://localhost/updateClientversion/
    CHttpDownloadFile httpXML("http://localhost/updateClientversion/updater.xml",
                              "updater.xml", strDownload, this);//调用下载文件的类
    //connect(m_httpXML, SIGNAL(sigDownloadFinishedSignal()), this, SLOT(slotReplyHttpFinished()));//发生错误时一样会发送此信号
    httpXML.DownloadFile();
}

/**
/** * @brief xml1
/** * 比较两个版本号
 *
/** * xml1,xml2:两个XML文件，不能为空，并且文件需要存在,xml1必须是最新的xml文件（刚刚下载下来的）,xml2是本地的xml文件;
**/
int CAutoUpdater::CheckUpdateFiles(QString xml1, QString xml2)
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
            m_strTip = QStringLiteral("检查需要更新的文件...");
            qDebug() << m_strTip;

            QDomNodeList nodeList = CXMLParser::XMLParseElement(xml1, "file");
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
QString CAutoUpdater::GetElementVersion(QString xml, QString name)
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
    QDomNodeList nodeList = CXMLParser::XMLParseElement(xml, "file");
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

bool CAutoUpdater::CheckVersionForUpdate()
{

    QString strLocalXML = QDir::currentPath() + "/updater.xml";
    QString strDownloadXML = QDir::currentPath() + "/download/updater.xml";
    qDebug() << "strLocalXML = " << strLocalXML;
    qDebug() << "strDownloadXML = " << strDownloadXML;
    QString xml1Version = GetVersion(strLocalXML);
    QString xml2Version = GetVersion(strDownloadXML);
    qDebug() << QStringLiteral("两个版本对比：") << "local = " << xml1Version
             << ", download = " << xml2Version;
    return CheckVersion(xml1Version, xml2Version);
}

QString CAutoUpdater::GetVersion(QString xml)
{
    QDomNodeList nodeList = CXMLParser::XMLParseElement(xml, "version");
    QString version = nodeList.at(0).toElement().text();
    return version;
}

/**
 * 本地版本号和服务器最新版本号进行对比
 * 如果本地版本号和服务器最新版本号一致，则返回false，否则返回true
 * 返回true表示需要更新当前文件
 * 版本号暂定：V1.0 V2.0 ... , 来进行处理
**/
bool CAutoUpdater::CheckVersion(QString version, QString versionDownload)
{
    QStringList localVersionList = version.split('.');
    QStringList downloadVersionList = versionDownload.split('.');
    for(int i = 0 ; i < downloadVersionList.size(); ++i)
    {
        if(i >= localVersionList.size() || downloadVersionList.at(i) > localVersionList.at(i))
        {
            qDebug() << QStringLiteral("服务器版本比较新，需要更新！");
            return true;
        }
    }
    return false;
}

/**
 * 下载最新的版本的文件，并替换或者增加
 * 旧XML中有的文件，新XML没有的，此文件不做处理。（后期在考虑怎么处理）
 * 下载信息由m_listFileName和m_listFileDir提供
**/
void CAutoUpdater::DownloadUpdateFiles()
{
    //需要下载的文件的存储位置
    //即版本服务器存储版本的根目录
    QString strServer = "http://localhost/updateClientVersion/";

    //下载下来的文件需要保存到应用程序当前目录下的download目录下，这个是自定义的，
    //可以选择不同的目录去保存。
    QString strCurrentDir = QDir::currentPath();//当前程序运行路径

    if(m_listFileDir.isEmpty() || m_listFileName.isEmpty())
    {
        qDebug() << QStringLiteral("没有需要更新的文件！");
        return;
    }

    m_strTip = "开始下载更新文件 ...";
    qDebug() << m_strTip;
    QStringList strPlaceDirList;

    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        m_strTip = QStringLiteral("正在下载文件 ...") + m_listFileName.at(i);
        qDebug() << m_strTip;
        m_progUpdate = 100 * i / m_listFileName.size();

        /**放置下载文件的路径**/
        QString strPlaceDir = strCurrentDir + "/download/" + m_listFileDir.at(i);
        strPlaceDirList.push_back(strPlaceDir);
        QDir directory(strPlaceDir);//如果路径不存在，则创建
        if(!directory.exists())
        {
            directory.mkpath(strPlaceDir);
        }

        //文件在服务器中的存储位置
        QString strFileDirServer = strServer + m_listFileDir.at(i) + "/" + m_listFileName.at(i);
        CHttpDownloadFile http(strFileDirServer, m_listFileName.at(i), strPlaceDir, this);//调用下载文件的类
        http.DownloadFile(); //下载文件

        while(!http.GetBlsFinish())
        {
            if(http.GetTotalReceive() == -1)
            {
                m_progDownload = 1;
            }
            else
            {
                m_progDownload = 100 * http.GetReceiving() / http.GetTotalReceive();
            }
            QCoreApplication::processEvents();
        }

        //单个文件下载完成
        m_strTip = QStringLiteral("文件") + m_listFileName.at(i) + QStringLiteral("下载完成");
        qDebug() << m_strTip;
    }

    //所有文件下载完成
    m_strTip = QStringLiteral("更新完成！");
    qDebug() << m_strTip;

    //统一复制到旧的目录下。 这里的代码是复制的代码
    qDebug() << "Copy file size = " << m_listFileName.size();
    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        /**将下载好的文件复制到主目录中,先删除原先的文件**/
        QString strLocalFileName = strCurrentDir + "/" + m_listFileDir.at(i) + "/" + m_listFileName.at(i);
        if(QFile::exists(strLocalFileName))
            QFile::remove(strLocalFileName);

        //如果路径不存在，则创建
        QDir directory1(strCurrentDir + "/" + m_listFileDir.at(i));
        if(!directory1.exists())
            directory1.mkpath(strCurrentDir + "/" + m_listFileDir.at(i));

        //拷贝
        qDebug() << "copy : -----------------==============------------------";
        qDebug() << strPlaceDirList.at(i) + "/" + m_listFileName.at(i);
        qDebug() << strLocalFileName;
        if(QFile::copy(strPlaceDirList.at(i) + "/" + m_listFileName.at(i), strLocalFileName))
        {
            qDebug() << "File: " << m_listFileName.at(i) << " copy over!";
        }
    }

    /**替换旧的xml文件**/
    QString strNewXML = strCurrentDir + "/download/updater.xml"; //最新的XML文件
    QString strOldXML = strCurrentDir + "/updater.xml"; //旧的XML文件
    QFile::remove(strOldXML);
    QFile::copy(strNewXML, strOldXML);

    //拷贝结束的条件，到这里就整个更新过程结束了。
    QThread::sleep(2);
    m_bCopyOver = true;
    m_progUpdate = 100;

    //这里复制完成，考虑删掉临时下载文件。
}

int CAutoUpdater::GetDownProcess()
{
    return m_progDownload;
}

int CAutoUpdater::GetUpdateProcess()
{
    return m_progUpdate;
}

/**读取版本信息文件**/
QStringList CAutoUpdater::GetVersionInfo(QString txt)
{
    QStringList strTxtList;
    QString strLine;
    if(txt == "")
        txt = QDir::currentPath() + "/download/versionInfo.txt";
    QFile file(txt);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return strTxtList;
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

QStringList CAutoUpdater::GetUpdateFileDir()
{
    return m_listFileDir;
}

QStringList CAutoUpdater::GetUpdateFileName()
{
    return m_listFileName;
}

void CAutoUpdater::slotReplyHttpFinished()
{

}
