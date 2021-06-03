#include "filemanager.h"
#include "updatelog.h"

#include <QDir>
#include <QStorageInfo>
#include <QApplication>

extern UpdateLog g_log;

FileManager::FileManager()
{

}

FileManager::~FileManager()
{

}

void FileManager::deleteAllPathFiles(QString path)
{
    QDir _dir(path);
    QFileInfoList _listFile = _dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList _listFolder = _dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i < _listFolder.size(); ++i)
        deleteAllPathFiles(_listFolder.at(i).filePath());

    for(int i = 0; i < _listFile.size(); ++i)
    {
        QFile::remove(_listFile.at(i).filePath());
    }
}

void FileManager::deleteEmptyVersionPath(QString path)
{
    //Remove path.
    QDir _dir(path);
    QString _strRemovePath;
    QFileInfoList _listFolder = _dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < _listFolder.size(); ++i)
    {
        _strRemovePath = _listFolder.at(i).filePath();
        if(_strRemovePath.isEmpty() && _strRemovePath.contains(""))
        {
            g_log.log(UpdateLog::INFO, "Delete path " + _strRemovePath, __FILE__, __LINE__);
            _dir.remove(_strRemovePath);
        }
    }

}

