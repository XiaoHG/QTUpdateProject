#include "updateclientfilecontroler.h"

#include <QTextStream>
#include <QFile>

UpdateClientFileControler::UpdateClientFileControler(QString &filePath)
    :versionFilePath(filePath)
{

}

QStringList UpdateClientFileControler::readFile()
{
    QFile versionFile(versionFilePath);
    if(versionFile.open(QFile::ReadOnly))
    {
        QTextStream versionFileStream(&versionFile);
        while(!versionFileStream.atEnd())
        {
            versionInfos.push_back(versionFileStream.readLine().toLocal8Bit());
        }
        return versionInfos;
    }
}
