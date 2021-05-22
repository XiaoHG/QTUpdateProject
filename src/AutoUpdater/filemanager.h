#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>

class FileManager
{
public:
    FileManager();
    ~FileManager();

    /**
     * @brief DeleteAllPathFiles
     * @param path
     * Delete all files which in path.
     */
    static void DeleteAllPathFiles(QString path);
    static void DeleteEmptyVersionPath(QString path);
};

#endif // FILEMANAGER_H
