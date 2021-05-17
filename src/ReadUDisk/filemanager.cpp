#include "filemanager.h"

#include <QDir>
#include <QDebug>
#include <QStorageInfo>
#include <QApplication>

FileManager::FileManager()
{
    UDiskFileList(QApplication::applicationDirPath() + "/../../../source/UDisk");
    InnFileList(QApplication::applicationDirPath() + "/../../../source/Inn");
}

void FileManager::GetDevice()
{
    qDebug() << "----------start----------";
    QString UDiskPath = "";
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes())
    {
        qDebug() << storage.rootPath();
        if(storage.isValid() && storage.isReady())
        {
            UDiskPath = storage.rootPath();
            if(UDiskPath.contains("media"))
            {
                qDebug() << "qstring path:" << UDiskPath;
            }
        }
    }
    //FileList(UDiskPath);
}

void FileManager::UDiskFileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i < folder_list.size(); ++i)
        UDiskFileList(folder_list.at(i).filePath());

    for(int i = 0; i < file_list.size(); ++i)
    {
        m_uDiskFileList.append(file_list.at(i).filePath());
    }
}

void FileManager::InnFileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i < folder_list.size(); ++i)
        InnFileList(folder_list.at(i).filePath());

    for(int i = 0; i < file_list.size(); ++i)
    {
        m_innFileList.append(file_list.at(i).filePath());
    }
}

void FileManager::FileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i < folder_list.size(); ++i)
        FileList(folder_list.at(i).filePath());

    for(int i = 0; i < file_list.size(); ++i)
    {
        qDebug() << file_list.at(i).filePath();
    }
}

void FileManager::Print()
{
    foreach (QString str, m_uDiskFileList) {
        qDebug() << str;
    }

    qDebug() << "=======================================";

    foreach (QString str, m_innFileList) {
        qDebug() << str;
    }
}
