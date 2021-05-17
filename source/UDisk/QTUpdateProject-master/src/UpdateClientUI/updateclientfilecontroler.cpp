#include "updateclientfilecontroler.h"

#include <QTextStream>
#include <QFile>

UpdateClientFileControler::UpdateClientFileControler(QString &filePath)
    :varsionFilePath(filePath)
{

}

QStringList UpdateClientFileControler::readFile()
{
    QFile varsionFile(varsionFilePath);
    if(varsionFile.open(QFile::ReadOnly))
    {
        QTextStream varsionFileStream(&varsionFile);
        while(!varsionFileStream.atEnd())
        {
            varsionInfos.push_back(varsionFileStream.readLine().toLocal8Bit());
        }
        return varsionInfos;
    }
}
