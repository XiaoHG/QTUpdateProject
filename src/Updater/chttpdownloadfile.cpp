#include "chttpdownloadfile.h"
#include <QDir>
#include <QNetworkReply>
#include <QDomDocument>
#include <QCoreApplication>
#include <QProcess>
#include <QMessageBox>

CHttpDownLoadFile::CHttpDownloadFile(QString url, QString fileName,
                                     QString dir, QWidget *parent)
{
    m_urlAdress = url;
    m_strFileName = fileName;
    m_strDir = dir;
    this->setParent(parent);

    m_progUpdate = new QSlider(this);
    m_progDownload = new QSlider(this);
}

void CHttpDownLoadFile::replyNewDataArrived()
{
    if(m_file)
    {
        m_file->write(m_netReply->readAll());
        m_file->flush();
    }
    else
    {
        qDebug() << m_netReply->readAll();
    }
}

void CHttpDownLoadFile::ReplyFinished()
{
    m_netAccessManager->deleteLater();
    m_netReply->deleteLater();
    m_file->close();
    m_file->deleteLater();
}

void CHttpDownLoadFile::DownLoadFile()
{
    m_netReply = m_netAccessManager->get(QNetworkRequest(m_urlAdress));
    //当有新数据到达时就会触发此信号
    connect(m_netReply, SIGNAL(readyRead()), this, SLOT(replyNewDataArrived()));
    //完成数据接收后发送此信号
    connect(m_netReply, SIGNAL(finished()), this, SLOT(ReplyFinished()));
    //出现错误时发送此信号;
    connect(m_netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(ReplyError(QNetworkReply::NetworkError)));
    //用来提示文件下载进度
    connect(m_netReply, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(ReplyDownloadProgress(qint64, qint64)));

    /***存储文件的检测及使用***/
    if(m_strFileName.isEmpty()) //文件名
    {
        QFile fileInfo(m_urlAdress.path());
        m_strFileName = fileInfo.fileName();
    }

    if(!m_strDir.isEmpty()) //文件夹
    {
        QDir directory(m_strDir);
        if(!directory.exists()) //没有此文件夹则创建
        {
            directory.mkpath(m_strDir);
        }
        m_strFileName = m_strDir + "/" + m_strFileName;
    }

    if(QFile::exists(m_strFileName))//如果文件存在，那么删除
    {
        QFile::remove(m_strFileName);
    }

    m_file = new QFile(m_strFileName);
    if(m_file->open(QIODevice::WriteOnly))
    {
        qDebug()<<"can not store file：" << m_strFileName;
        delete m_file;
        m_file = NULL;
        return;
    }

}

void CHttpDownLoadFile::XMLParse(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        qDebug() << "Open XML file for read error ... ";
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Setcontent error ... ";
        file.close();
    }

    file.close();
    QDomElement root = doc.documentElement();
    if(root.tagName() != "filelist")
    {
        qDebug() << "root.tagname != filelist";
    }
    else
    {
        QDomNodeList nodeList = root.elementsByTagName("file");
        for(int i = 0; i < nodeList.size(); ++i)
        {
            qDebug() << nodeList.at(i).toElement().attribute("name")
                     << nodeList.at(i).toElement().attribute("dir")
                     << nodeList.at(i).toElement().attribute("version");
        }
    }

}

void CHttpDownLoadFile::downloadXMLFile()
{
    QString strCurrentDir = QDir::currentPath();
    QString strDownLoad = strCurrentDir + "/download/";

    QDir diretory(strDownLoad);
    if(!diretory.exists())//如果路径不存在，则创建
    {
        diretory.mkpath(strDownLoad);
    }

    m_urlAdress = "http://localhost/updateClientVarsion/update.xml";
    m_strFileName = "";
    m_strDir = strDownLoad;

    this->DownLoadFile();
}

/**在xml中查找version，并返回版本号**/
QString CHttpDownLoadFile::getElementVersion(QString xml, QString name)
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
                    QString dir = nodeList.at(i).toElement().attribute("dir");
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
/** * @brief name1
/** * 比较两个版本号
/** * name1,name2:两个XML文件，不能为空，并且文件需要存在,name1必须是最新的xml文件（刚刚下载下来的）,name2是本地的xml文件;
**/
int CHttpDownLoadFile::CheckUpdateFiles(QString name1, QString name2)
{
    m_listFileDir.clear();
    m_listFileName.clear();

    if(name1.isEmpty() || name2.isEmpty())
        return 0;

    if(QFile::exists(name1))
    {
        if(QFile::exists(name2))
        {
            m_strTip = "检查需要更新的文件...";

            QFile file(name1);
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

                            QString versionDownload = getElementVersion(name2, name);//获取本地xml文件对应文件（name）的版本信息
                            if(versionDownload.isEmpty())//本地XML没有此文件：下载，并放到相应的目录中
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
                                    qDebug() << name << "文件是最新版本，不需要更新";
                                }
                            }
                        }
                        return 1;
                    }
                    else
                    {
                        qDebug() << "XML 内容错误!";
                        return 0;
                    }
                }
                else
                {
                    m_strTip = "不能打开更新文件！";
                    return 0;
                }
            }
        }
        else
        {
            m_strTip = "下载更新文件错误！";
            return 0;
        }
    }
    else
    {
        m_strTip = "本地更新文件不存在";
        return 0;
    }
}

bool CHttpDownLoadFile::CheckVersion(QString version, QString versionDownload)
{

}

/********下载最新的版本的文件，并替换或者增加*********************

     *旧XML中有的文件，新XML没有的，此文件不做处理。

     *下载信息由m_listFileName和m_listFileDir提供

     *****************************************************/
void CHttpDownLoadFile::DownLoadUpdateFiles()
{
    QString strServer = "http://localhost/updateClientVarsion/";//需要下载的文件的存储位置
    QString strCurrentDir = QDir::currentPath();//当前程序运行路径

    if(m_listFileDir.isEmpty() || m_listFileName.isEmpty())
    {
        qDebug() << "没有需要更新的文件！";
        ExitApp(strCurrentDir + "/main.exe");
        return;
    }

    m_strTip = "开始下载更新文件 ...";

    m_blsFinished = false;

    for(int i = 0; i < m_listFileName.size(); ++i)
    {
        m_strTip = "正在下载文件" + m_listFileName.at(i);
        m_progUpdate->setValue(100 * i / m_listFileName.size());

        /**放置下载文件的路径**/
        QString strPlaceDir = strCurrentDir + "/download" + m_listFileDir.at(i);
        QDir directory(strPlaceDir);//如果路径不存在，则创建
        if(!directory.exists())
        {
            directory.mkpath(strPlaceDir);
        }
        QString strFileDirServer = strServer + m_listFileDir.at(i) + "/" + m_listFileName.at(i); //文件在服务器中的存储位置
        m_urlAdress = strFileDirServer;
        m_strFileName = "";
        m_strDir = strPlaceDir;
        DownLoadFile();
        while(!m_blsFinished)
        {
            if(m_nTotal == -1)
            {
                m_progDownload->setValue(1);
            }
            else
            {
                m_progDownload->setValue(100 * m_nReceived / m_nTotal);
            }
            QCoreApplication::processEvents();
        }

        m_strTip = "文件" + m_listFileName.at(i) + "下载完成";

        /**将下载好的文件复制到主目录中,先删除原先的文件**/
        QString strLocalFileName = strCurrentDir + "/" + m_listFileDir.at(i) + "/" + m_listFileName.at(i);
        if(QFile::exists(strLocalFileName))
            QFile::remove(strLocalFileName);

        QDir directory1(strCurrentDir + "/" + m_listFileDir.at(i)); //如果路径不存在，则创建
        if(!directory1.exists())
            directory1.mkpath(strCurrentDir + "/" + m_listFileDir.at(i));

        QFile::copy(strPlaceDir + "/" + m_listFileName.at(i), strLocalFileName);
    }

    m_blsFinished = true;
    m_strTip = "更新完成！";

    /**替换旧的xml文件**/
    QString strNewXML = strCurrentDir + "/download/update.xml"; //最新的XML文件
    QString strOldXML = strCurrentDir + "/update.xml"; //旧的XML文件

    QFile::remove(strOldXML);
    QFile::copy(strNewXML, strOldXML);

    ExitApp(strCurrentDir + "/main.exe");
}

/********退出当前程序，并且启动需要的程序*************************

     *name:需要启动的程序，可以使用相对位置

     *程序不能使用exit(0),会发生线程错误，这里使用this->close()函数

     *****************************************************/


void CHttpDownLoadFile::ExitApp(QString name)
{
    if(!name.isEmpty())
    {
        /**运行主程序，并且退出当前更新程序(说明：主程序在上上一级目录中)**/
        if(!QProcess::startDetached(name))//启动主程序，主程序在其上一级目录
        {
            QMessageBox::warning(this, "告警信息", "启动主程序错误！\n可能主程序不存在或者被破坏！\n解决办法：重新安装程序");
        }
    }
    //this->close();
}


