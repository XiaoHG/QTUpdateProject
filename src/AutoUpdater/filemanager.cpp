#include "filemanager.h"
#include "updatelog.h"

#include <QDir>
#include <QDebug>
#include <QStorageInfo>
#include <QApplication>

extern UpdateLog g_log;

FileManager::FileManager()
{

}

FileManager::~FileManager()
{

}

void FileManager::DeleteAllPathFiles(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i < folder_list.size(); ++i)
        DeleteAllPathFiles(folder_list.at(i).filePath());

    for(int i = 0; i < file_list.size(); ++i)
    {
        QFile::remove(file_list.at(i).filePath());
    }
}

void FileManager::DeleteEmptyVersionPath(QString path)
{
    QDir dir(path);
    QString removePath;
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < folder_list.size(); ++i)
    {
        removePath = folder_list.at(i).filePath();
        qDebug() << "removePath = " << removePath;
        if(removePath.isEmpty() && removePath.contains(""))
        {
            //g_log.log(UpdateLog::INFO, "Delete path " + removePath, __FILE__, __LINE__);
            dir.remove(removePath);
        }
    }

}

