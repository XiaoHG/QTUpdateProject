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
     * @brief deleteAllPathFiles
     * @param path
     * Delete all files which in path.
     */
    static void deleteAllPathFiles(QString path);
    static void deleteEmptyVersionPath(QString path);

private:
};

#endif // FILEMANAGER_H
