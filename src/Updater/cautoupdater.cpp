#include "cautoupdater.h"
#include "chttpdownloadfile.h"
#include <QDir>
#include <QDomDocument>
#include <QCoreApplication>
#include <QProcess>
#include <QMessageBox>

CAutoUpdater::CAutoUpdater(QWidget *parent)
    :QMainWindow(parent)
{
    m_progUpdate = new QSlider();
    m_progDownload = new QSlider();

    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(400, 200);
    QPalette pal;
    pal.setColor(QPalette::Background, QColor(255, 245, 225));
    this->setPalette(pal);
    this->setAutoFillBackground(true);
}

/**
 * 下载服务器版本控制文件updater.xml
**/
void CAutoUpdater::downloadXMLFile()
{
    QString strCurrentDir = QDir::currentPath();
    QString strDownload = strCurrentDir + "/download";
    qDebug() << "strDownload = " << strDownload;
    QDir diretory(strDownload);
    if(!diretory.exists())//如果路径不存在，则创建
    {
        diretory.mkpath(strDownload);
    }

    m_httpXML = new CHttpDownloadFile("http://localhost/updateClientVarsion/updater.xml", "updater.xml", strDownload, this);//调用下载文件的类
    //connect(m_httpXML, SIGNAL(sigDownloadFinishedSignal()), this, SLOT(slotReplyHttpFinished()));//发生错误时一样会发送此信号
    m_httpXML->DownloadFile();
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

    if(QFile::exists(xml1))
    {
        if(QFile::exists(xml2))
        {
            m_strTip = "检查需要更新的文件...";
            qDebug() << m_strTip;
            qDebug() << xml1;
            QFile file(xml1);
            if(file.open(QIODevice::ReadOnly | QFile::Text))
            {
                QString errorStr;
                int errorLine;
                int errorColumn;
                QDomDocument doc;
                if(doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
                {
                    QDomElement root = doc.documentElement();
                    if(root.tagName() == "filelist")
                    {
                        QDomNodeList nodeList = root.elementsByTagName("file");
                        for(int i = 0; i < nodeList.size(); ++i)
                        {
                            QString name = nodeList.at(i).toElement().attribute("name");
                            QString dir = nodeList.at(i).toElement().attribute("dir");
                            QString version = nodeList.at(i).toElement().attribute("version");

                            QString versionDownload = getElementVersion(xml2, name);//获取本地xml文件对应文件（name）的版本信息
                            if(versionDownload.isEmpty())//本地XML没有此文件：下载并放到相应的目录中
                            {
                                m_listFileDir.append(dir);
                                m_listFileName.append(name);
                            }
                            else
                            {
                                /**检查版本，如果本地版本低于下载版本，则下载**/
                                if(!CheckVersion(version, versionDownload))
                                {
                                    m_listFileDir.append(dir);
                                    m_listFileName.append(name);
                                }
                                else
                                {
                                    qDebug() << "文件是最新版本，不需要更新  " << version << dir << name;
                                }
                            }
                        }
                        //debug code
                        for(int i = 0; i < m_listFileName.size(); ++i)
                        {
                            qDebug() << "File " << m_listFileName.at(i) << " need to update!";
                        }
                        return 1;
                    }
                    else
                    {
                        m_strTip = "XML 文件内容错误";
                        qDebug() << m_strTip;
                        return 0;
                    }
                }
                else
                {
                    m_strTip = "获取XML doc失败";
                    qDebug() << m_strTip;
                    return 0;
                }
            }
            else
            {
                m_strTip = "不能打开更新文件！";
                qDebug() << m_strTip;
                return 0;
            }
        }
        else
        {
            m_strTip = "下载更新文件错误！";
            qDebug() << m_strTip;
            return 0;
        }
    }
    else
    {
        m_strTip = "本地更新文件不存在";
        qDebug() << m_strTip;
        return 0;
    }
}

/**
 * 在xml中查找version，并返回版本号
 * 查找的是指定文件（name）的版本号
 * 前提是已经存在版本管理文件（xml）,对其进行解析。
**/
QString CAutoUpdater::getElementVersion(QString xml, QString name)
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

    QFile file(xml);
    if(file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QDomDocument doc;
        if(doc.setContent(&file))
        {
            QDomElement root = doc.documentElement();
            if(root.tagName() == "filelist")
            {
                int i = 0;
                QDomNodeList nodeList = root.elementsByTagName("file");
                for(; i < nodeList.size(); ++i)
                {
                    QString tmpName = nodeList.at(i).toElement().attribute("name");
                    //QString dir = nodeList.at(i).toElement().attribute("dir");
                    QString version = nodeList.at(i).toElement().attribute("version");

                    if(name == tmpName)
                    {
                        qDebug() << "find!" << name;
                        result = version;
                        break;
                    }
                }
                if(i == nodeList.size())
                {
                    qDebug() << "can't find!" << name;
                }
            }
            else
            {
                qDebug() << "root.tagname != filelist ...";
            }
        }
        else
        {
            qDebug() << "Set content error!";
        }
        file.close();
    }
    else
    {
        qDebug() << "Open for read error!";
    }
    return result;
}

/**
 * 本地版本号和服务器最新版本号进行对比
 * 如果本地版本号和服务器最新版本号一致，则返回false，否则返回true
 * 返回true表示需要更新当前文件
 * 版本号暂定：V1.0 V2.0 ... , 来进行处理
**/
bool CAutoUpdater::CheckVersion(QString version, QString versionDownload)
{
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
    QString strServer = "http://localhost/updateClientVarsion/";

    //下载下来的文件需要保存到应用程序当前目录下的download目录下，这个是自定义的，
    //可以选择不同的目录去保存。
    QString strCurrentDir = QDir::currentPath();//当前程序运行路径

    if(m_listFileDir.isEmpty() || m_listFileName.isEmpty())
    {
        qDebug() << "没有需要更新的文件！";
        ExitApp(strCurrentDir + "../../../test/UpdateTest/UpdateTest/debug/UpdateTest.exe");
        return;
    }

    m_strTip = "开始下载更新文件 ...";
    qDebug() << m_strTip;
    m_blsFinished = false;

    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        m_strTip = "正在下载文件 ..." + m_listFileName.at(i);
        qDebug() << m_strTip;
        m_progUpdate->setValue(100 * i / m_listFileName.size());

        /**放置下载文件的路径**/
        QString strPlaceDir = strCurrentDir + "/download/" + m_listFileDir.at(i);
        QDir directory(strPlaceDir);//如果路径不存在，则创建
        if(!directory.exists())
        {
            directory.mkpath(strPlaceDir);
        }

        //文件在服务器中的存储位置
        QString strFileDirServer = strServer + m_listFileDir.at(i) + "/" + m_listFileName.at(i);
        CHttpDownloadFile *http = new CHttpDownloadFile(strFileDirServer, m_listFileName.at(i), strPlaceDir, this);//调用下载文件的类
        http->DownloadFile();
//        m_urlAdress = strFileDirServer;
//        m_strFileName = "";
//        m_strDir = strPlaceDir;
//        DownLoadFile();

        while(!http->m_blsFinished)
        {
            if(http->m_nTotal == -1)
            {
                m_progDownload->setValue(1);
            }
            else
            {
                m_progDownload->setValue(100 * http->m_nReceived / http->m_nTotal);
            }
            QCoreApplication::processEvents();
        }

        m_strTip = "文件" + m_listFileName.at(i) + "下载完成";
        qDebug() << m_strTip;

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
        qDebug() << strPlaceDir + "/" + m_listFileName.at(i);
        qDebug() << strLocalFileName;
        if(QFile::copy(strPlaceDir + "/" + m_listFileName.at(i), strLocalFileName))
        {
            qDebug() << "File: " << m_listFileName.at(i) << " copy over!";
        }
    }

    m_blsFinished = true;
    m_strTip = "更新完成！";
    qDebug() << m_strTip;

    /**替换旧的xml文件**/
    QString strNewXML = strCurrentDir + "/download/updater.xml"; //最新的XML文件
    QString strOldXML = strCurrentDir + "/updater.xml"; //旧的XML文件

    QFile::remove(strOldXML);
    QFile::copy(strNewXML, strOldXML);

    //到此更新了全部该更新的文件，执行主程序。
    ExitApp(strCurrentDir + "main.exe");
}

/** * @brief name
 * 退出当前程序，并且启动需要的程序
 * name:需要启动的程序，可以使用相对位置
 * 程序不能使用exit(0),会发生线程错误，这里使用this->close()函数
**/
void CAutoUpdater::ExitApp(QString name)
{
    //F:\QT\updateClient\QTUpdateProject\test\UpdateTest\UpdateTest\debug
    name = "../../test/UpdateTest/UpdateTest/debug/UpdateTest.exe";
    if(!name.isEmpty())
    {
        qDebug() << "主程序启动：" << name;
        /**运行主程序，并且退出当前更新程序(说明：主程序在上上一级目录中)**/
        if(!QProcess::startDetached("../../test/UpdateTest/UpdateTest/debug/UpdateTest.exe"))//启动主程序，主程序在其上一级目录
        {
            QMessageBox::warning(this, "warning", name, QMessageBox::Ok, QMessageBox::NoButton);
        }
    }
    this->close();
}

void CAutoUpdater::slotReplyHttpFinished()
{

}
