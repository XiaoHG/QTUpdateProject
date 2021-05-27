#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>

class FileManager
{
public:
    FileManager();

    void GetDevice();

    void Print();
    void FileList(QString path);
private:
    void UDiskFileList(QString path);
    void InnFileList(QString path);

private:
    QStringList m_innFileList;
    QStringList m_uDiskFileList;
};

#endif // FILEMANAGER_H
